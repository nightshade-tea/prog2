#include "fila.h"
#include <stdlib.h>

static nodo *cria_nodo(int val, nodo *prox)
{
    nodo *nd;

    if ( !(nd = malloc(sizeof(nodo))) )
        return NULL;

    nd->val = val;
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

fila *fl_cria(void)
{
    fila *fl;

    if ( !(fl = malloc(sizeof(fila))) )
        return NULL;

    fl->tam = 0;
    fl->inicio = fl->fim = NULL;

    return fl;
}

void fl_destroi(fila **pfl)
{
    nodo *nd, *prox;

    if ( !pfl || !(*pfl) )
        return;

    nd = (*pfl)->inicio;

    while (nd) {
        prox = nd->prox;
        destroi_nodo(&nd);
        nd = prox;
    }

    free(*pfl);
    *pfl = NULL;

    return;
}

int fl_insere(fila *fl, int val)
{
    nodo *nd;

    if ( !fl || !(nd = cria_nodo(val, NULL)) )
        return -1;

    if ( !(fl->inicio) )
        fl->inicio = nd;

    if (fl->fim)
        fl->fim->prox = nd;

    fl->fim = nd;

    return ++(fl->tam);
}

int fl_retira(fila *fl, int *val)
{
    nodo *nd;

    if ( !fl || !val || !(nd = fl->inicio) )
        return -1;

    *val = nd->val;
    fl->inicio = nd->prox;

    if (fl->fim == nd)
        fl->fim = NULL;

    destroi_nodo(&nd);

    return --(fl->tam);
}

int fl_tamanho(fila *fl)
{
    if (!fl)
        return -1;

    return fl->tam;
}
