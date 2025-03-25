#include "jukefila.h"
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 * Cria uma jukefila.
 *
 * Retorno: Ponteiro para a jukefila criada, ou NULL em caso de erro.
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

jukefila*
criar_jukefila(
    void
)
{
    jukefila *fila;

    if ( !(fila = malloc(sizeof(jukefila))) )
        return NULL;

    fila->inicio = fila->final = NULL;

    return fila;
}

/*-----------------------------------------------------------------------------
 * Insere um pedido na jukefila.
 * Os pedidos de maior valor tem maior prioridade.
 *
 * Parâmetros:
 *  - pedido *elemento: Pedido a ser inserido;
 *  - jukefila *fila: Jukefila que receberá o pedido.
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void
inserir_jukefila(
    pedido *elemento,
    jukefila *fila
)
{
    pedido *aux;

    if ( !elemento || !fila )
        return;

    // Caso 1: Fila vazia
    if ( !(aux = fila->inicio) ) {
        fila->inicio = fila->final = elemento;
        return;
    }

    // Caso 2: Inserção no início
    if (elemento->valor > fila->inicio->valor) {
        elemento->proximo = fila->inicio;
        fila->inicio = elemento;
        return;
    }

    // Caso geral
    while (aux->proximo && elemento->valor <= aux->proximo->valor)
        aux = aux->proximo;

    elemento->proximo = aux->proximo;
    elemento->anterior = aux;
    elemento->anterior->proximo = elemento;

    if (elemento->proximo)
        elemento->proximo->anterior = elemento;

    return;
}

/*-----------------------------------------------------------------------------
 * Retira um pedido da jukefila.
 *
 * Parâmetros:
 *  - jukefila *fila: Jukefila para retirar o pedido.
 *
 * Retorno: Ponteiro para o pedido ou NULL em caso de erro.
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

pedido*
consumir_jukefila(
    jukefila *fila
)
{
    pedido *pd;

    if ( !fila || !(pd = fila->inicio) )
        return NULL;

    fila->inicio = pd->proximo;

    if (fila->final == pd)
        fila->final = NULL;

    return pd;
}

/*-----------------------------------------------------------------------------
 * Conta o número de pedidos em uma jukefila.
 *
 * Parâmetros:
 *  - jukefila *fila: Jukefila com os pedidos.
 *
 * Retorno: Quantidade de pedidos na jukefila. Em caso de erro retorna 0.
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

unsigned int
contar_jukefila(
    jukefila *fila
)
{
    pedido *pd;
    unsigned int i;

    if ( !fila || !(pd = fila->inicio) )
        return 0;

    for (i = 0; pd; i++)
        pd = pd->proximo;

    return i;
}

/*-----------------------------------------------------------------------------
 * Desaloca uma jukefila e todos os seus pedidos.
 *
 * Parâmetros:
 *  - jukefila *fila: Jukefila a ser destruída.
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void
destruir_jukefila(
    jukefila *fila
)
{
    pedido *pd, *prox;

    if (!fila)
        return;

    pd = fila->inicio;

    while (pd) {
        prox = pd->proximo;
        destruir_pedido(pd);
        pd = prox;
    }

    free(fila);

    return;
}
