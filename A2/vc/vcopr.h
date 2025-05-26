#ifndef VCOPR_H
#define VCOPR_H 1

/* - paramv é o vetor de operandos para cada operação, ou seja, paramv[0] é o
   arquivo .vc, e o restante são membros.
   - paramc é a quantidade de elementos em paramv. */

/* insere sem compressão */
void ip (int paramc, char **paramv);

/* insere com compressão */
void ic (int paramc, char **paramv);

/* move membros */
void m (int paramc, char **paramv);

/* extrai membros */
void x (int paramc, char **paramv);

/* remove membros */
void r (int paramc, char **paramv);

/* imprime membros */
void c (int paramc, char **paramv);

/* derivação */
void z (int paramc, char **paramv);

#endif /* vcopr.h */
