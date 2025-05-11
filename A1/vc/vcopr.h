#ifndef VCOPR_H
#define VCOPR_H 1

/* - paramv é o vetor de operandos para cada operação, ou seja, paramv[0] é o
   arquivo .vc, e o restante são membros.
   - paramc é a quantidade de elementos em paramv. */

/* insere sem compressão */
void ip (int paramc, char **paramv);

/* insere com compressão */
void ic (int paramc, char **paramv);

/* mover membros */
void m (int paramc, char **paramv);

#endif /* vcopr.h */