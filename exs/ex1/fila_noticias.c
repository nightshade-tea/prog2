#include "fila_noticias.h"
#include <stdlib.h>
#include <string.h>

static fn_nodo *cria_nodo(const char *titulo, const char *texto, fn_nodo *prox)
{
    fn_nodo *nodo;

    if ( !(nodo = malloc(sizeof(fn_nodo))) )
        return NULL;

    strncpy(nodo->noticia.titulo, titulo, sizeof(nodo->noticia.titulo));
    strncpy(nodo->noticia.texto, texto, sizeof(nodo->noticia.texto));
    nodo->noticia.idade = 0;
    nodo->prox = prox;

    return nodo;
}

static void destroi_nodo(fn_nodo **nodo)
{
    if ( !nodo || !(*nodo) )
        return;

    free(*nodo);
    *nodo = NULL;

    return;
}

fn_fila *fn_cria()
{
    fn_fila *fila;

    if ( !(fila = malloc(sizeof(fn_fila))) )
        return NULL;

    fila->prim = fila->ult = NULL;

    return fila;

}

fn_nodo *fn_insere(fn_fila *fila, const char *titulo, const char *texto)
{
    fn_nodo *nodo;

    if ( !fila || !(nodo = cria_nodo(titulo, texto, NULL)) )
        return NULL;

    if ( !(fila->prim) )
        fila->prim = nodo;

    if (fila->ult)
        fila->ult->prox = nodo;

    fila->ult = nodo;

    return nodo;
}

void fn_atualiza(fn_fila *fila)
{
    fn_nodo *nodo;

    if (!fila)
        return;

    nodo = fila->prim;

    while (nodo) {
        (nodo->noticia.idade)++;
        nodo = nodo->prox;
    }

    return;
}

int fn_remove(fn_fila *fila, char *titulo, char *texto)
{
    fn_nodo *nodo, *prox;

    if ( !fila || !(nodo = fila->prim) )
        return 1;

    while (nodo && nodo->noticia.idade > 3) {
        prox = nodo->prox;
        destroi_nodo(&nodo);
        nodo = prox;
    }

    if (!nodo) {
        fila->prim = fila->ult = NULL;
        return 1;
    }

    strcpy(titulo, nodo->noticia.titulo);
    strcpy(texto, nodo->noticia.texto);

    fila->prim = nodo->prox;

    if (fila->ult == nodo)
        fila->ult = NULL;

    destroi_nodo(&nodo);

    return 0;
}

void fn_destroi(fn_fila **fila)
{
    fn_nodo *nodo, *prox;

    if ( !fila || !(*fila))
        return;

    nodo = (*fila)->prim;

    while (nodo) {
        prox = nodo->prox;
        destroi_nodo(&nodo);
        nodo = prox;
    }

    free(*fila);
    *fila = NULL;

    return;
}
