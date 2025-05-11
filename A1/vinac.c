#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vc/vcopr.h"

#define GETOPT_OPTIONS "i::pmxrc"

static void
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
            ic (argc - optind, &argv[optind]);

          // -ip
          else if (strcmp (optarg, "p") == 0)
            ip (argc - optind, &argv[optind]);

          else
            fatal ("erro: opção desconhecida '-i%s'", optarg);
        }

      // -i
      else
        ic (argc - optind, &argv[optind]);

      break;

    case 'p':
      ip (argc - optind, &argv[optind]);
      break;

    case 'm':
      m (argc - optind, &argv[optind]);
      break;

    case 'x':
      x (argc - optind, &argv[optind]);
      break;

    case 'r':
      r (argc - optind, &argv[optind]);
      break;

    case 'c':
      printf ("flag='%c'\n", op);
      break;

    case '?':
      fatal ("erro: opção desconhecida '-%c'", optopt);
      break;

    default:
      fatal ("erro: opção não implementada '-%c'", op);
      break;
    }

  return 0;
}
