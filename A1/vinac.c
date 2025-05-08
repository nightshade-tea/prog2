#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GETOPT_OPTIONS "i::pmxrc"

static void
fatal (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  exit (1);
}

int
main (int argc, char **argv)
{
  int op;

  opterr = 0;

  if ((op = getopt (argc, argv, GETOPT_OPTIONS)) == -1)
    fatal ("erro: nenhuma opção fornecida\n");

  // se apenas uma opção for passada, essa segunda chamada a getopt () não
  // altera as variáveis opt*. se mais de uma opção for passada é um erro.

  if (getopt (argc, argv, GETOPT_OPTIONS) != -1)
    fatal ("erro: mais de uma opção fornecida\n");

  switch (op)
    {
    case 'i':
      printf ("flag='%c' optarg='%s'\n", op, optarg);
      break;

    case 'p':
      printf ("flag='%c'\n", op);
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
      fatal ("erro: opção desconhecida '-%c'\n", optopt);
      break;

    default:
      fatal ("erro: opção não implementada '-%c'\n", op);
      break;
    }

  printf ("optind=%d\n", optind);

  for (int i = optind; i < argc; i++)
    printf ("argv[%i]='%s'\n", i, argv[i]);

  return 0;
}
