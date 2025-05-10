#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vc/vcdef.h"
#include "vc/vcdir.h"

static void
fatal (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  perror ("vcopr");

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

// empurra todos os arquivos dos membros de índice > idx no arquivo vcfp
static void
shift_memfs (uint64_t shift, struct directory *dir, uint32_t idx, FILE *vcfp)
{
  struct member *mem;
  char *buffer;
  uint32_t i;

  if (!dir || !dir->memv || idx >= dir->memc || !vcfp)
    fatal ("shitft_memfs(): parâmetros inválidos\n");

  // itera do início ao fim para não sobrescrever dados
  for (i = dir->memc - 1; i > idx; i--)
    {
      mem = &dir->memv[i];

      // lê o arquivo do membro
      if (!(buffer = malloc (mem->dsz)))
        fatal ("shift_memfs: falha ao alocar memória para ler '%s'\n",
               mem->name);

      if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
        fatal ("shift_memfs: falha ao manipular o arquivo '%s'\n", mem->name);

      if (fread (buffer, mem->dsz, 1, vcfp) != 1)
        fatal ("shift_memfs: falha ao ler o arquivo '%s'\n", mem->name);

      mem->offset += shift;

      // escreve na nova posição
      if (fseek (vcfp, mem->offset, SEEK_SET) != 0)
        fatal ("shift_memfs: falha ao manipular o arquivo '%s'\n", mem->name);

      if (fwrite (buffer, mem->dsz, 1, vcfp) != 1)
        fatal ("shift_memfs: falha ao escrever o arquivo '%s'\n", mem->name);

      free (buffer);
    }
}

void
ip (int paramc, char **paramv)
{
  struct directory dir;
  FILE *vcfp;
  off_t x;

  if (paramc < 2)
    fatal ("erro: número insuficiente de parâmetros\n");

  // checa se podemos ler todos os membros do disco
  for (int i = 1; i < paramc; i++)
    if (access (paramv[i], R_OK) != 0)
      fatal ("erro: não é possível ler o arquivo '%s'\n", paramv[i]);

  if (!(vcfp = fopen (paramv[0], "a+b")))
    fatal ("erro: falha ao abrir o arquivo '%s'\n", paramv[0]);

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
        fatal ("erro: falha ao manipular o arquivo '%s'\n", paramv[0]);
    }

  // reorganiza os membros em vcfp
  for (int i = 1; i < paramc; i++)
    {
      uint32_t idx;
      struct member mx;
      struct stat st;
      FILE *fp;
      char *buffer;

      strncpy (mx.name, paramv[i], MAX_NAME_LEN);
      mx.name[MAX_NAME_LEN] = '\0';

      if (!(fp = fopen (paramv[i], "rb")))
        fatal ("erro: falha ao abrir o arquivo '%s'\n", paramv[i]);

      if (fstat (fileno (fp), &st) != 0)
        fatal ("erro: fstat falhou com '%s'\n", paramv[i]);

      mx.uid = st.st_uid;
      mx.osz = st.st_size;
      mx.dsz = st.st_size;
      mx.mtime = st.st_mtime;

      if (!(buffer = malloc (st.st_size)))
        fatal ("erro: falha ao alocar memória para ler '%s'\n", paramv[i]);

      if (fread (buffer, st.st_size, 1, fp) != 1)
        fatal ("erro: falha ao ler o arquivo '%s'\n", paramv[i]);

      // caso em que o membro já existe
      if ((idx = mem_index (&dir, paramv[i])) != UINT32_MAX)
        {
          struct member *pmy;
          uint64_t shift;

          pmy = &dir.memv[idx];
          mx.offset = pmy->offset;
          mx.pos = pmy->pos;
          shift = mx.dsz - pmy->dsz;

          shift_memfs (shift, &dir, idx, vcfp);

          if (fseek (vcfp, mx.offset, SEEK_SET) != 0)
            fatal ("erro: falha ao manipular o arquivo '%s'\n", paramv[i]);

          if (fwrite (buffer, mx.dsz, 1, vcfp) != 1)
            fatal ("erro: falha ao escrever o arquivo '%s'\n", paramv[i]);

          *pmy = mx;
        }

      else
        {
          mx.pos = dir.memc;

          if (fseek (vcfp, 0, SEEK_END) != 0)
            fatal ("erro: falha ao manipular o arquivo '%s'\n", paramv[i]);

          mx.offset = ftell (vcfp);

          if (add_mem (&dir, &mx) != 0)
            fatal ("erro: falha ao adicionar o membro '%s' ao diretório\n",
                   paramv[i]);

          if (fwrite (buffer, st.st_size, 1, vcfp) != 1)
            fatal ("erro: falha ao escrever no arquivo '%s'\n", paramv[i]);
        }

      free (buffer);
      fclose (fp);
    }

  // grava o diretório no fim do arquivo
  if (write_dir (&dir, vcfp) != 0)
    fatal ("erro: falha ao gravar o diretório no arquivo '%s'\n", paramv[0]);

  free (dir.memv);
  fclose (vcfp);
}