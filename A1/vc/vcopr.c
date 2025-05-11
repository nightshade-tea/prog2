#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lz/lz.h"
#include "vc/vcdef.h"
#include "vc/vcdir.h"
#include "vc/vcopr.h"

static void
fatal (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  if (errno)
    perror (" :");
  else
    fprintf (stderr, "\n");

  exit (1);
}

// trunca os últimos x bytes do arquivo f
static int
trunc_last (FILE *f, off_t x)
{
  int fd;
  struct stat st;
  off_t sz;

  if (!f || x < 0)
    return -1;

  if ((fd = fileno (f)) < 0)
    return -1;

  if (fstat (fd, &st) != 0)
    return -1;

  if ((sz = st.st_size - x) < 0)
    return -1;

  if (ftruncate (fd, sz) != 0)
    return -1;

  return 0;
}

// empurra todos os arquivos dos membros de índice >= idx no arquivo vcfp
static void
shift_memfs (int64_t shift, struct directory *dir, uint32_t idx, FILE *vcfp)
{
  struct member *mem;
  char *buffer;
  uint32_t i;

  if (!dir || !dir->memv || !vcfp)
    fatal ("shift_memfs(): parâmetros inválidos");

  if (shift == 0)
    return;

  if (shift > 0)
    {
      for (i = dir->memc - 1; i >= idx; i--)
        {
          mem = &dir->memv[i];

          // lê o arquivo do membro
          if (!(buffer = malloc (mem->dsz)))
            fatal ("shift_memfs: falha ao alocar memória para ler '%s'",
                   mem->name);

          if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
            fatal ("shift_memfs: falha ao manipular o arquivo '%s'",
                   mem->name);

          if (fread (buffer, mem->dsz, 1, vcfp) != 1)
            fatal ("shift_memfs: falha ao ler o arquivo '%s'", mem->name);

          mem->offset += shift;

          // escreve na nova posição
          if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
            fatal ("shift_memfs: falha ao manipular o arquivo '%s'",
                   mem->name);

          if (fwrite (buffer, mem->dsz, 1, vcfp) != 1)
            fatal ("shift_memfs: falha ao escrever o arquivo '%s'", mem->name);

          free (buffer);
        }

      return;
    }

  for (i = idx; i < dir->memc; i++)
    {
      mem = &dir->memv[i];

      fprintf (stderr, "shift_memfs: (%s) shift=%lld offset=%llu o-s=%llu\n",
               mem->name, shift, mem->offset, mem->offset + shift);

      // lê o arquivo do membro
      if (!(buffer = malloc (mem->dsz)))
        fatal ("shift_memfs: falha ao alocar memória para ler '%s'",
               mem->name);

      if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
        fatal ("shift_memfs: falha ao manipular o arquivo '%s'", mem->name);

      if (fread (buffer, mem->dsz, 1, vcfp) != 1)
        fatal ("shift_memfs: falha ao ler o arquivo '%s'", mem->name);

      mem->offset += shift;

      // escreve na nova posição
      if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
        fatal ("shift_memfs: falha ao manipular o arquivo '%s'", mem->name);

      if (fwrite (buffer, mem->dsz, 1, vcfp) != 1)
        fatal ("shift_memfs: falha ao escrever o arquivo '%s'", mem->name);

      free (buffer);
    }

  // trunca o excesso
  if (trunc_last (vcfp, -1 * shift) != 0)
    fatal ("shift_memfs: falha ao truncar");
}

static void
ins (int paramc, char **paramv, int compress)
{
  struct directory dir;
  FILE *vcfp;
  off_t x;

  if (paramc < 2)
    fatal ("erro: número insuficiente de parâmetros");

  // checa se podemos ler todos os membros do disco
  for (int i = 1; i < paramc; i++)
    if (access (paramv[i], R_OK) != 0)
      fatal ("erro: não é possível ler o arquivo '%s'", paramv[i]);

  // tenta abrir o arquivo vcpath, se não existir cria
  errno = 0;
  if (!(vcfp = fopen (paramv[0], "rb+")))
    if (errno != ENOENT || !(vcfp = fopen (paramv[0], "wb+")))
      fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath, se existir
  if (read_dir (&dir, vcfp) != 0)
    {
      dir.memv = NULL;
      dir.memc = 0;
    }

  // trunca o arquivo para remover o diretório
  if (dir.memc > 0)
    {
      x = sizeof (struct member) * dir.memc + sizeof (size_t);

      if (trunc_last (vcfp, x) != 0)
        fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);
    }

  // reorganiza os membros em vcfp
  for (int i = 1; i < paramc; i++)
    {
      uint32_t idx;
      struct member mx;
      struct stat st;
      FILE *fp;
      unsigned char *buffer;
      unsigned char *compressed;
      uint64_t compsz;

      strncpy (mx.name, paramv[i], MAX_NAME_LEN);
      mx.name[MAX_NAME_LEN] = '\0';

      if (!(fp = fopen (paramv[i], "rb")))
        fatal ("erro: falha ao abrir o arquivo '%s'", paramv[i]);

      if (fstat (fileno (fp), &st) != 0)
        fatal ("erro: fstat falhou com '%s'", paramv[i]);

      mx.uid = st.st_uid;
      mx.osz = st.st_size;
      mx.dsz = st.st_size;
      mx.mtime = st.st_mtime;

      // caso o arquivo seja vazio
      if (mx.dsz == 0)
        {
          mx.pos = dir.memc;
          mx.offset = 0;

          if (add_mem (&dir, &mx) != 0)
            fatal ("erro: falha ao adicionar o membro '%s' ao diretório",
                   paramv[i]);

          fclose (fp);
          continue;
        }

      if (!(buffer = malloc (st.st_size)))
        fatal ("erro: falha ao alocar memória para ler '%s'", paramv[i]);

      if (fread (buffer, st.st_size, 1, fp) != 1)
        fatal ("erro: falha ao ler o arquivo '%s'", paramv[i]);

      if (compress)
        {
          compsz = mx.osz * 2;

          if (!(compressed = malloc (compsz)))
            fatal ("erro: falha ao alocar memória para ler '%s'", paramv[i]);

          compsz = LZ_Compress (buffer, compressed, mx.osz);

          if (compsz <= 0 || compsz >= mx.osz)
            {
              free (compressed);
              compressed = NULL;
            }
          else
            {
              free (buffer);
              buffer = compressed;
              mx.dsz = compsz;
            }
        }

      // caso em que o membro já existe
      if ((idx = mem_index (&dir, paramv[i])) != UINT32_MAX)
        {
          struct member *pmy;
          int64_t shift;

          pmy = &dir.memv[idx];
          mx.offset = pmy->offset;
          mx.pos = pmy->pos;
          shift = mx.dsz - pmy->dsz;

          shift_memfs (shift, &dir, idx + 1, vcfp);

          if (fseek (vcfp, mx.offset, SEEK_SET) != 0)
            fatal ("erro: falha ao manipular o arquivo '%s'", paramv[i]);

          if (fwrite (buffer, mx.dsz, 1, vcfp) != 1)
            fatal ("erro: falha ao escrever o arquivo '%s'", paramv[i]);

          *pmy = mx;
        }

      else
        {
          mx.pos = dir.memc;

          if (fseek (vcfp, 0, SEEK_END) != 0)
            fatal ("erro: falha ao manipular o arquivo '%s'", paramv[i]);

          mx.offset = ftell (vcfp);

          if (add_mem (&dir, &mx) != 0)
            fatal ("erro: falha ao adicionar o membro '%s' ao diretório",
                   paramv[i]);

          if (fwrite (buffer, st.st_size, 1, vcfp) != 1)
            fatal ("erro: falha ao escrever no arquivo '%s'", paramv[i]);
        }

      free (buffer);
      fclose (fp);
    }

  // grava o diretório no fim do arquivo
  if (write_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao gravar o diretório no arquivo '%s'", paramv[0]);

  free (dir.memv);
  fclose (vcfp);
}

// empurra os índices dos membros de índice >= idx em dir->memv
static void
shift_mempos (int shift, struct directory *dir, uint32_t idx)
{
  struct member *mem;
  uint32_t i;
  size_t memvsz;

  if (!dir || !dir->memv)
    fatal ("shift_mempos: parâmetros inválidos");

  if (shift == 0)
    return;

  memvsz = (dir->memc + shift) * sizeof (struct member);

  if (shift > 0)
    {
      if (!(dir->memv = realloc (dir->memv, memvsz)))
        fatal ("shift_mempos: falha ao realocar memória para o diretório");

      for (i = dir->memc - 1; i >= idx; i--)
        {
          mem = &dir->memv[i];
          mem->pos += shift;
          dir->memv[i + shift] = *mem;
        }

      return;
    }

  for (i = idx; i < dir->memc; i++)
    {
      mem = &dir->memv[i];
      mem->pos += shift;
      dir->memv[i + shift] = *mem;
    }

  if (!(dir->memv = realloc (dir->memv, memvsz)))
    fatal ("shift_mempos: falha ao realocar memória para o diretório");
}

void
ip (int paramc, char **paramv)
{
  ins (paramc, paramv, 0);
}

void
ic (int paramc, char **paramv)
{
  ins (paramc, paramv, 1);
}

void
m (int paramc, char **paramv)
{
  struct directory dir;
  FILE *vcfp;
  off_t x;
  uint32_t midx, tidx;
  unsigned char *buffer;

  if (paramc != 3)
    fatal ("erro: a opção -m requer exatamente um arquivo e dois membros");

  if (!(vcfp = fopen (paramv[0], "rb+")))
    fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath
  if (read_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao ler o diretório do arquivo '%s'", paramv[0]);

  // pega os índices de membro e target
  midx = mem_index (&dir, paramv[1]);
  tidx = mem_index (&dir, paramv[2]);

  if (midx == UINT32_MAX)
    fatal ("erro: o membro '%s' não existe", paramv[1]);

  if (tidx == UINT32_MAX)
    fatal ("erro: o membro '%s' não existe", paramv[2]);

  // casos que não precisa mover nada
  if (midx == tidx || midx == tidx + 1)
    {
      free (dir.memv);
      fclose (vcfp);
      return;
    }

  // trunca o arquivo para remover o diretório
  x = sizeof (struct member) * dir.memc + sizeof (size_t);

  if (trunc_last (vcfp, x) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  // reorganiza os membros em vcfp
  shift_memfs (dir.memv[midx].dsz, &dir, tidx + 1, vcfp);
  shift_mempos (1, &dir, tidx + 1);
  dir.memc++;

  // se o membro foi shiftado, corrige o índice
  if (midx > tidx)
    midx++;

  dir.memv[tidx + 1] = dir.memv[midx];
  dir.memv[tidx + 1].pos = tidx + 1;
  dir.memv[tidx + 1].offset = dir.memv[tidx].offset + dir.memv[tidx].dsz;

  // no disco, copia mem para depois do target
  if (!(buffer = malloc (dir.memv[midx].dsz)))
    fatal ("erro: falha ao alocar memória para ler '%s'", dir.memv[midx].name);

  if (fseek (vcfp, dir.memv[midx].offset, SEEK_SET) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  if (fread (buffer, dir.memv[midx].dsz, 1, vcfp) != 1)
    fatal ("erro: falha ao ler o arquivo '%s'", paramv[0]);

  if (fseek (vcfp, dir.memv[tidx + 1].offset, SEEK_SET) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  if (fwrite (buffer, dir.memv[midx].dsz, 1, vcfp) != 1)
    fatal ("erro: falha ao escrever o arquivo '%s'", paramv[0]);

  free (buffer);

  shift_memfs (-1 * dir.memv[midx].dsz, &dir, midx + 1, vcfp);
  shift_mempos (-1, &dir, midx + 1);
  dir.memc--;

  // grava o diretório no fim do arquivo
  if (write_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao gravar o diretório no arquivo '%s'", paramv[0]);

  free (dir.memv);
  fclose (vcfp);
}