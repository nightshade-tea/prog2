#include <unistd.h>

#include "vc/vcdef.h"
#include "vc/vcdir.h"

int
ip (int paramc, const char **paramv)
{
  struct directory dir;
  const char *vcpath;

  if (paramc < 2)
    return -1;

  // checa se podemos ler todos os membros do disco
  for (int i = 1; i < paramc; i++)
    if (access (paramv[i], R_OK) != 0)
      return -1;

  vcpath = paramv[0];

  // carrega o diretório de vcpath, se existir
  if (read_dir (vcpath, &dir) != 0)
    {
      dir.memv = NULL;
      dir.memc = 0;
    }

  // ...

  return 0;
}