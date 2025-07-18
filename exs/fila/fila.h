#ifndef FILA_H
#define FILA_H

typedef struct _nodo {
    int val;
    struct _nodo *prox;
} nodo;

typedef struct _fila {
    int tam;
    nodo *inicio;
    nodo *fim;
} fila;

fila *fl_cria(void);

void fl_destroi(fila **pfl);

// insere/retira retorna o tamanho da fila apos a operacao, ou -1 em caso de erro
int fl_insere(fila *fl, int val);

int fl_retira(fila *fl, int *val);

int fl_tamanho(fila *fl);

#endif // !FILA_H
