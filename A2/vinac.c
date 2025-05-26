#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vc/vcopr.h"

#define GETOPT_OPTIONS "i::pmxrcz"

static __attribute__ ((noreturn)) void
fatal (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  fprintf (stderr, "\n");

  exit (1);
}

int
main (int argc, char **argv)
{
  void (*opfunc) (int, char **);
  int op;

  opterr = 0;

  if ((op = getopt (argc, argv, GETOPT_OPTIONS)) == -1)
    fatal ("erro: nenhuma opção fornecida");

  // se apenas uma opção for passada, essa segunda chamada a getopt () não
  // altera as variáveis opt*. se mais de uma opção for passada é um erro.

  if (getopt (argc, argv, GETOPT_OPTIONS) != -1)
    fatal ("erro: mais de uma opção fornecida");

  switch (op)
    {
    case 'i':
      if (optarg)
        {
          // -ic
          if (strcmp (optarg, "c") == 0)
            opfunc = ic;

          // -ip
          else if (strcmp (optarg, "p") == 0)
            opfunc = ip;

          else
            fatal ("erro: opção desconhecida '-i%s'", optarg);
        }

      // -i
      else
        opfunc = ic;

      break;

    case 'p':
      opfunc = ip;
      break;

    case 'm':
      opfunc = m;
      break;

    case 'x':
      opfunc = x;
      break;

    case 'r':
      opfunc = r;
      break;

    case 'c':
      opfunc = c;
      break;

    case 'z':
      opfunc = z;
      break;

    case '?':
      fatal ("erro: opção desconhecida '-%c'", optopt);
      break;

    default:
      fatal ("erro: opção não implementada '-%c'", op);
      break;
    }

  (*opfunc) (argc - optind, &argv[optind]);

  return 0;
}
