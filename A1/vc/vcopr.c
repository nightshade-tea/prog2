#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "lz/lz.h"
#include "vc/vcdef.h"
#include "vc/vcdir.h"
#include "vc/vcopr.h"

static __attribute__ ((noreturn)) void
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
  int64_t i;

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
  off_t dirsz;

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
      dirsz = sizeof (struct member) * dir.memc + sizeof (size_t);

      if (trunc_last (vcfp, dirsz) != 0)
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
          mx.offset = 0;

          if (add_mem (&dir, &mx) != 0)
            fatal ("erro: falha ao adicionar o membro '%s' ao diretório",
                   paramv[i]);

          fclose (fp);
          continue;
        }

      if (!(buffer = malloc (mx.dsz)))
        fatal ("erro: falha ao alocar memória para ler '%s'", paramv[i]);

      if (fread (buffer, mx.dsz, 1, fp) != 1)
        fatal ("erro: falha ao ler o arquivo '%s'", paramv[i]);

      if (compress)
        {
          compsz = mx.dsz * 2;

          if (!(compressed = malloc (compsz)))
            fatal ("erro: falha ao alocar memória para ler '%s'", paramv[i]);

          compsz = LZ_Compress (buffer, compressed, mx.dsz);

          if (compsz <= 0 || compsz >= mx.dsz)
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
          if (fseek (vcfp, 0, SEEK_END) != 0)
            fatal ("erro: falha ao manipular o arquivo '%s'", paramv[i]);

          mx.offset = ftell (vcfp);

          if (add_mem (&dir, &mx) != 0)
            fatal ("erro: falha ao adicionar o membro '%s' ao diretório",
                   paramv[i]);

          if (fwrite (buffer, mx.dsz, 1, vcfp) != 1)
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
  int64_t i;
  size_t memvsz;

  if (!dir || !dir->memv)
    fatal ("shift_mempos: parâmetros inválidos");

  if (shift == 0)
    return;

  memvsz = (dir->memc + shift) * sizeof (struct member);

  if (shift > 0)
    {
      if (memvsz == 0)
        {
          free (dir->memv);
          dir->memv = NULL;
          return;
        }

      if (!(dir->memv = realloc (dir->memv, memvsz)))
        fatal ("shift_mempos: falha ao realocar memória para o diretório");

      for (i = dir->memc - 1; i >= idx; i--)
        {
          mem = &dir->memv[i];
          dir->memv[i + shift] = *mem;
        }

      return;
    }

  for (i = idx; i < dir->memc; i++)
    {
      mem = &dir->memv[i];
      dir->memv[i + shift] = *mem;
    }

  if (memvsz == 0)
    {
      free (dir->memv);
      dir->memv = NULL;
      return;
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
  off_t dirsz;
  uint32_t midx, tidx;
  unsigned char *buffer;
  unsigned char tnull; // flag para indicar se o target é NULL

  if (paramc < 2)
    fatal ("erro: número insuficiente de parâmetros");

  if (paramc > 3)
    fatal ("erro: número excessivo de parâmetros");

  if (paramc == 2)
    tnull = 1;
  else
    tnull = 0;

  if (!(vcfp = fopen (paramv[0], "rb+")))
    fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath
  if (read_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao ler o diretório do arquivo '%s'", paramv[0]);

  // pega os índices de membro e target
  midx = mem_index (&dir, paramv[1]);

  if (tnull)
    tidx = 0;
  else
    tidx = mem_index (&dir, paramv[2]);

  if (midx == UINT32_MAX)
    fatal ("erro: o membro '%s' não existe", paramv[1]);

  if (tidx == UINT32_MAX)
    fatal ("erro: o membro '%s' não existe", paramv[2]);

  // casos que não precisa mover nada
  if (midx == tidx || (!tnull && midx == tidx + 1))
    {
      free (dir.memv);
      fclose (vcfp);
      return;
    }

  // trunca o arquivo para remover o diretório
  dirsz = sizeof (struct member) * dir.memc + sizeof (size_t);

  if (trunc_last (vcfp, dirsz) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  // reorganiza os membros em vcfp
  // tnull = 0 -> !tnull = 1 ; tnull = 1 -> !tnull = 0

  shift_memfs (dir.memv[midx].dsz, &dir, tidx + !tnull, vcfp);
  shift_mempos (1, &dir, tidx + !tnull);
  dir.memc++;

  // se o membro foi shiftado, corrige o índice
  if (midx >= tidx + !tnull)
    midx++;

  dir.memv[tidx + !tnull] = dir.memv[midx];

  if (tnull)
    dir.memv[tidx + !tnull].offset = 0;
  else
    dir.memv[tidx + !tnull].offset
        = dir.memv[tidx].offset + dir.memv[tidx].dsz;

  // no disco, copia mem para depois do target
  if (!(buffer = malloc (dir.memv[midx].dsz)))
    fatal ("erro: falha ao alocar memória para ler '%s'", dir.memv[midx].name);

  if (fseek (vcfp, dir.memv[midx].offset, SEEK_SET) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  if (fread (buffer, dir.memv[midx].dsz, 1, vcfp) != 1)
    fatal ("erro: falha ao ler o arquivo '%s'", paramv[0]);

  if (fseek (vcfp, dir.memv[tidx + !tnull].offset, SEEK_SET) != 0)
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

void
x (int paramc, char **paramv)
{
  struct directory dir;
  FILE *vcfp;
  unsigned char xall;
  uint32_t *idxv;
  uint32_t idxvc;

  if (paramc < 1)
    fatal ("erro: número insuficiente de parâmetros");

  if (paramc == 1)
    xall = 1;
  else
    xall = 0;

  if (!(vcfp = fopen (paramv[0], "rb")))
    fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath
  if (read_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao ler o diretório do arquivo '%s'", paramv[0]);

  if (!xall)
    {
      if (!(idxv = malloc (sizeof (uint32_t) * (paramc - 1))))
        fatal ("erro: falha ao alocar memória para os índices");

      idxvc = paramc - 1;

      // preenche o vetor de índices dos membros a serem extraídos
      for (int i = 1; i < paramc; i++)
        if ((idxv[i - 1] = mem_index (&dir, paramv[i])) == UINT32_MAX)
          fatal ("erro: membro '%s' não encontrado", paramv[i]);
    }
  else
    {
      if (!(idxv = malloc (sizeof (uint32_t) * dir.memc)))
        fatal ("erro: falha ao alocar memória para os índices");

      idxvc = dir.memc;

      for (uint32_t i = 0; i < dir.memc; i++)
        idxv[i] = i;
    }

  // extrai os membros em idxv
  for (uint32_t i = 0; i < idxvc; i++)
    {
      unsigned char *compbuf, *uncompbuf;
      struct member *mem;
      FILE *mfp;

      mem = &dir.memv[idxv[i]];

      if (!(compbuf = malloc (mem->dsz)))
        fatal ("erro: falha ao alocar memória para ler '%s'", mem->name);

      if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
        fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

      if (fread (compbuf, mem->dsz, 1, vcfp) != 1)
        fatal ("erro: falha ao ler o membro '%s'", mem->name);

      if (mem->dsz != mem->osz)
        {
          if (!(uncompbuf = malloc (mem->osz)))
            fatal ("erro: falha ao alocar memória para ler '%s'", mem->name);

          LZ_Uncompress (compbuf, uncompbuf, mem->dsz);

          free (compbuf);
          compbuf = NULL;
        }
      else
        {
          uncompbuf = compbuf;
          compbuf = NULL;
        }

      if (!(mfp = fopen (mem->name, "wb")))
        fatal ("erro: falha ao abrir o arquivo '%s'", mem->name);

      if (fwrite (uncompbuf, mem->osz, 1, mfp) != 1)
        fatal ("erro: falha ao escrever o arquivo '%s'", mem->name);

      free (uncompbuf);
      fclose (mfp);
    }

  free (dir.memv);
  fclose (vcfp);
}

void
r (int paramc, char **paramv)
{
  struct directory dir;
  FILE *vcfp;
  off_t dirsz;
  uint32_t *idxv;
  uint32_t idxvc;

  if (paramc < 2)
    fatal ("erro: número insuficiente de parâmetros");

  if (!(vcfp = fopen (paramv[0], "rb+")))
    fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath
  if (read_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao ler o diretório do arquivo '%s'", paramv[0]);

  // preenche idxv
  if (!(idxv = malloc (sizeof (uint32_t) * (paramc - 1))))
    fatal ("erro: falha ao alocar memória para os índices");

  idxvc = paramc - 1;

  // preenche o vetor de índices dos membros a serem extraídos
  for (int i = 1; i < paramc; i++)
    if ((idxv[i - 1] = mem_index (&dir, paramv[i])) == UINT32_MAX)
      fatal ("erro: membro '%s' não encontrado", paramv[i]);

  // trunca o arquivo para remover o diretório
  dirsz = sizeof (struct member) * dir.memc + sizeof (size_t);

  if (trunc_last (vcfp, dirsz) != 0)
    fatal ("erro: falha ao manipular o arquivo '%s'", paramv[0]);

  // remove os membros de vcfp
  for (uint32_t i = 0; i < idxvc; i++)
    {
      struct member *mem;
      int64_t shift;

      mem = &dir.memv[idxv[i]];
      shift = -1 * mem->dsz;

      shift_memfs (shift, &dir, idxv[i] + 1, vcfp);
      shift_mempos (-1, &dir, idxv[i] + 1);
      dir.memc--;
    }

  // grava o diretório no fim do arquivo
  if (dir.memc == 0)
    {
      // se o diretório estiver vazio, remove o arquivo
      fclose (vcfp);
      unlink (paramv[0]);
    }
  else
    {
      if (write_dir (&dir, vcfp) != 0)
        fatal ("erro: falha ao gravar o diretório no arquivo '%s'", paramv[0]);

      fclose (vcfp);
    }

  free (dir.memv);
}

void
c (int paramc, char **paramv)
{
  struct directory dir;
  FILE *vcfp;

  if (paramc < 1)
    fatal ("erro: número insuficiente de parâmetros");

  if (paramc > 1)
    fatal ("erro: número excessivo de parâmetros");

  if (!(vcfp = fopen (paramv[0], "rb")))
    fatal ("erro: falha ao abrir o arquivo '%s'", paramv[0]);

  // carrega o diretório de vcpath
  if (read_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao ler o diretório do arquivo '%s'", paramv[0]);

  printf ("\narquivo %s (%u membros):\n", paramv[0], dir.memc);
  printf (
      "----------------------------------------------------------------\n");
  printf ("%-20s %-10s %-10s %-10s %-10s\n", "nome", "original", "arquivado",
          "modificado", "UID");
  printf (
      "----------------------------------------------------------------\n");
  for (uint32_t i = 0; i < dir.memc; i++)
    {
      struct member *mem;
      time_t mtime;
      struct tm *tmtime;
      char time[9];

      mem = &dir.memv[i];
      mtime = mem->mtime; // evitar warning por mtime não ser alinhado
      tmtime = localtime (&mtime);

      if (strftime (time, sizeof time, "%d/%m/%y", tmtime) == 0)
        time[0] = '\0';

      // PRIu64 é o formato correto pra imprimir uint64_t
      printf ("%-20s %-10" PRIu64 " %-10" PRIu64 " %-10s %-10u\n", mem->name,
              mem->osz, mem->dsz, time, mem->uid);
    }
  printf (
      "----------------------------------------------------------------\n\n");

  free (dir.memv);
  fclose (vcfp);
}