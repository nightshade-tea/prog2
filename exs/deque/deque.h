#ifndef DEQUE_H
#define DEQUE_H

typedef struct _nodo {
    int val;
    struct _nodo *ant;
    struct _nodo *prox;
} nodo;

typedef struct _deque {
    int tam;
    nodo *inicio;
    nodo *fim;
} deque;

// aloca um deque vazio
deque *dq_cria(void);

// libera a memoria do deque e seus nodos, aterra o ponteiro
void dq_destroi(deque **pdq);

// insere/retira retornam a tamanho do deque apos a operacao ou -1 em erro
int dq_insere_inicio(deque *dq, int val);

int dq_insere_fim(deque *dq, int val);

int dq_retira_inicio(deque *dq, int *val);

int dq_retira_fim(deque *dq, int *val);

// retorna o tamanho do deque ou -1 em erro
int dq_tamanho(deque *dq);

#endif // !DEQUE_H
