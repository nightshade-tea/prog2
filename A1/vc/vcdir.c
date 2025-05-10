#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vc/vcdef.h"

int
read_dir (const char *vcpath, struct directory *dir)
{
  FILE *vcfp;
  size_t dirsz;

  if (!vcpath || !dir)
    return -1;

  if (!(vcfp = fopen (vcpath, "rb")))
    return -1;

  // lê o tamanho do diretório
  if (fseek (vcfp, -1 * sizeof dirsz, SEEK_END) != 0)
    {
      fclose (vcfp);
      return -1;
    }

  if (fread (&dirsz, sizeof dirsz, 1, vcfp) != 1)
    {
      fclose (vcfp);
      return -1;
    }

  // checa se o tamanho do diretório é condizente
  if (dirsz == 0 || dirsz % sizeof (struct member))
    {
      fclose (vcfp);
      return -1;
    }

  dir->memc = dirsz / sizeof (struct member);

  // aloca memória para o diretório
  if (!(dir->memv = malloc (sizeof (struct member) * dir->memc)))
    {
      fclose (vcfp);
      return -1;
    }

  // lê os membros
  if (fseek (vcfp, -1 * (dirsz + sizeof dirsz), SEEK_END) != 0)
    {
      free (dir->memv);
      fclose (vcfp);
      return -1;
    }

  if (fread (dir->memv, dirsz, 1, vcfp) != 1)
    {
      free (dir->memv);
      fclose (vcfp);
      return -1;
    }

  fclose (vcfp);

  return 0;
}

int
write_dir (const char *vcpath, struct directory *dir)
{
  FILE *vcfp;
  size_t dirsz;

  if (!vcpath || !dir)
    return -1;

  if (!(vcfp = fopen (vcpath, "ab")))
    return -1;

  if (fseek (vcfp, 0, SEEK_END) != 0)
    {
      fclose (vcfp);
      return -1;
    }

  dirsz = dir->memc * sizeof (struct member);

  // escreve o conteúdo do diretório
  if (fwrite (dir->memv, dirsz, 1, vcfp) != 1)
    {
      fclose (vcfp);
      return -1;
    }

  // escreve o tamanho do diretório
  if (fwrite (&dirsz, sizeof dirsz, 1, vcfp) != 1)
    {
      fclose (vcfp);
      return -1;
    }

  fclose (vcfp);

  return 0;
}

int
mem_index (const char *memname, struct directory *dir)
{
  if (!memname || !dir)
    return -1;

  for (uint32_t i = 0; i < dir->memc; i++)
    if (strcmp (dir->memv[i].name, memname) == 0)
      return i;

  return -1;
}