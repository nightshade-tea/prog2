#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
      printf ("flag='%c' optarg='%s'\n", op, optarg);
      break;

    case 'p':
      ip (argc - optind, &argv[optind]);
      break;

    case 'm':
      printf ("flag='%c'\n", op);
      break;

    case 'x':
      printf ("flag='%c'\n", op);
      break;

    case 'r':
      printf ("flag='%c'\n", op);
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
