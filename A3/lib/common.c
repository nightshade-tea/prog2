// #include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void
fatal (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  //  if (errno)
  //    perror (" :");
  //  else

  fprintf (stderr, "\n");

  exit (1);
}
