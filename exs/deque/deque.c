#include "deque.h"
#include <stdlib.h>

static nodo *cria_nodo(int val, nodo *ant, nodo *prox)
{
    nodo *nd;

    if ( !(nd = malloc(sizeof(nodo))) )
        return NULL;

    nd->val = val;
    nd->ant = ant;
    nd->prox = prox;

    return nd;
}

static void destroi_nodo(nodo **pnd)
{
    if ( !pnd || !(*pnd) )
        return;

    free(*pnd);
    *pnd = NULL;

    return;
}

deque *dq_cria(void)
{
    deque *dq;

    if ( !(dq = malloc(sizeof(deque))) )
        return NULL;

    dq->inicio = dq->fim = NULL;
    dq->tam = 0;

    return dq;
}

void dq_destroi(deque **pdq)
{
    nodo *nd, *prox;

    if ( !pdq || !(*pdq) )
        return;

    nd = (*pdq)->inicio;
    
    while (nd) {
        prox = nd->prox;
        destroi_nodo(&nd);
        nd = prox;
    }

    free(*pdq);
    *pdq = NULL;

    return;
}

int dq_insere_inicio(deque *dq, int val)
{
    nodo *nd;

    if ( !dq || !(nd = cria_nodo(val, NULL, dq->inicio)) )
        return -1;

    if ( !(dq->fim) )
        dq->fim = nd;

    if (dq->inicio)
        dq->inicio->ant = nd;

    dq->inicio = nd;

    return ++(dq->tam);
}

int dq_insere_fim(deque *dq, int val)
{
    nodo *nd;

    if ( !dq || !(nd = cria_nodo(val, dq->fim, NULL)) )
        return -1;

    if ( !(dq->inicio) )
        dq->inicio = nd;

    if (dq->fim)
        dq->fim->prox = nd;

    dq->fim = nd;

    return ++(dq->tam);
}

int dq_retira_inicio(deque *dq, int *val)
{
    nodo *nd;

    if ( !dq || !val || !(nd = dq->inicio) )
        return -1;

    *val = nd->val;

    dq->inicio = nd->prox;
    
    if (dq->inicio)
        dq->inicio->ant = NULL;

    if (dq->fim == nd)
        dq->fim = NULL;

    destroi_nodo(&nd);

    return --(dq->tam);
}

int dq_retira_fim(deque *dq, int *val)
{
    nodo *nd;

    if ( !dq || !val || !(nd = dq->fim) )
        return -1;

    *val = nd->val;

    dq->fim = nd->ant;
    
    if (dq->fim)
        dq->fim->prox = NULL;

    if (dq->inicio == nd)
        dq->inicio = NULL;

    destroi_nodo(&nd);

    return --(dq->tam);
}

int dq_tamanho(deque *dq)
{
    if (!dq)
        return -1;

    return dq->tam;
}
