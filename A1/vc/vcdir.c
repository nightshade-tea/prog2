#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vc/vcdef.h"

int
read_dir (struct directory *dir, FILE *vcfp)
{
  size_t dirsz;

  if (!dir || !vcfp)
    return -1;

  // lê o tamanho do diretório
  if (fseek (vcfp, -1 * sizeof dirsz, SEEK_END) != 0)
    return -1;

  if (fread (&dirsz, sizeof dirsz, 1, vcfp) != 1)
    return -1;

  // checa se o tamanho do diretório é condizente
  if (dirsz == 0 || dirsz % sizeof (struct member))
    return -1;

  dir->memc = dirsz / sizeof (struct member);

  // aloca memória para o diretório
  if (!(dir->memv = malloc (sizeof (struct member) * dir->memc)))
    return -1;

  // lê os membros
  if (fseek (vcfp, -1 * (dirsz + sizeof dirsz), SEEK_END) != 0)
    {
      free (dir->memv);
      return -1;
    }

  if (fread (dir->memv, dirsz, 1, vcfp) != 1)
    {
      free (dir->memv);
      return -1;
    }

  return 0;
}

int
write_dir (struct directory *dir, FILE *vcfp)
{
  size_t dirsz;

  if (!dir || !vcfp)
    return -1;

  if (fseek (vcfp, 0, SEEK_END) != 0)
    return -1;

  dirsz = dir->memc * sizeof (struct member);

  // escreve o conteúdo do diretório
  if (fwrite (dir->memv, dirsz, 1, vcfp) != 1)
    return -1;

  // escreve o tamanho do diretório
  if (fwrite (&dirsz, sizeof dirsz, 1, vcfp) != 1)
    return -1;

  return 0;
}

uint32_t
mem_index (struct directory *dir, const char *memname)
{
  uint32_t idx;

  if (!dir || !memname)
    return UINT32_MAX;

  for (idx = 0; idx < dir->memc; idx++)
    if (strcmp (dir->memv[idx].name, memname) == 0)
      return idx;

  return UINT32_MAX;
}

int
rm_mem (struct directory *dir, uint32_t idx)
{
  struct member *newmemv;

  if (!dir || !dir->memv || idx >= dir->memc)
    return -1;

  // remove o membro de memv
  for (uint32_t i = idx; i < dir->memc - 1; i++)
    dir->memv[i] = dir->memv[i + 1];

  // atualiza memc
  dir->memc--;

  // realoca memv
  if (dir->memc == 0)
    {
      free (dir->memv);
      dir->memv = NULL;
      return 0;
    }

  // usa newmemv para não perder o ponteiro original caso realloc falhe
  if (!(newmemv = realloc (dir->memv, sizeof (struct member) * dir->memc)))
    return -1;

  dir->memv = newmemv;

  return 0;
}

int
add_mem (struct directory *dir, struct member *mem)
{
  struct member *newmemv;

  if (!dir || !mem)
    return -1;

  // realoca memv
  newmemv = realloc (dir->memv, sizeof (struct member) * (dir->memc + 1));

  if (!newmemv)
    return -1;

  dir->memv = newmemv;

  // adiciona o novo membro em memv
  dir->memv[dir->memc] = *mem;
  dir->memc++;

  return 0;
}