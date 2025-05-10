#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vc/vcdef.h"
#include "vc/vcdir.h"

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

int
ip (int paramc, const char **paramv)
{
  struct directory dir;
  FILE *vcfp;
  off_t x;

  if (paramc < 2)
    return -1;

  // checa se podemos ler todos os membros do disco
  for (int i = 1; i < paramc; i++)
    if (access (paramv[i], R_OK) != 0)
      return -1;

  if (!(vcfp = fopen (paramv[0], "a+b")))
    {
      free (dir.memv);
      return -1;
    }

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
        {
          free (dir.memv);
          fclose (vcfp);
          return -1;
        }
    }

  // reorganiza os membros em vcfp
  for (int i = 1; i < paramc; i++)
    {
      uint32_t idx;
      struct member mem;
      struct stat st;
      FILE *fp;
      char *buffer;

      strncpy (mem.name, paramv[i], MAX_NAME_LEN);
      mem.name[MAX_NAME_LEN] = '\0';

      fp = fopen (paramv[i], "rb");
      fstat (fileno (fp), &st);

      mem.uid = st.st_uid;
      mem.pos = dir.memc;
      mem.osz = st.st_size;
      mem.dsz = st.st_size;
      mem.mtime = st.st_mtime;

      buffer = malloc (st.st_size);
      fread (buffer, st.st_size, 1, fp);

      if ((idx = mem_index (&dir, paramv[i])) != UINT32_MAX)
        {
          // TODO: caso em que o membro já existe
        }

      else
        {
          fseek (vcfp, 0, SEEK_END);
          mem.offset = ftell (vcfp);

          add_mem (&dir, &mem);
          fwrite (buffer, st.st_size, 1, vcfp);
        }

      free (buffer);
      fclose (fp);
    }

  // grava o diretório no fim do arquivo
  if (write_dir (&dir, vcfp) != 0)
    {
      free (dir.memv);
      fclose (vcfp);
      return -1;
    }

  free (dir.memv);
  fclose (vcfp);

  return 0;
}