#ifndef COMMON_H
#define COMMON_H 1

#define ensure(expr)                                                          \
  do                                                                          \
    {                                                                         \
      if (!(expr))                                                            \
        fatal ("%s assertion failed (at %s:%d)", #expr, __FILE__, __LINE__);  \
    }                                                                         \
  while (0)

__attribute__ ((noreturn)) void fatal (const char *format, ...);

#endif /* common.h */
