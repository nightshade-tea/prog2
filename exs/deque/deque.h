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

deque *dq_cria(void);

void dq_destroi(deque **pdq);

// insere/retira retornam a tamanho do deque apos a operacao

int dq_insere_inicio(deque *dq, int val);

int dq_retira_inicio(deque *dq, int val);

int dq_insere_fim(deque *dq, int *val);

int dq_retira_fim(deque *dq, int *val);

int dq_tamanho(deque *dq);

#endif // !DEQUE_H
