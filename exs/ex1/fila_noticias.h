#ifndef FILA_NOTICIAS_H
#define FILA_NOTICIAS_H

#define FN_TAM_TITULO 33
#define FN_TAM_TEXTO  513

typedef struct _fn_noticia {

    char titulo[FN_TAM_TITULO];
    char texto[FN_TAM_TEXTO];
    unsigned char idade;

} fn_noticia;

typedef struct _fn_nodo {

    fn_noticia noticia;
    struct _fn_nodo *prox;

} fn_nodo;

typedef struct _fn_fila {

    fn_nodo *prim;
    fn_nodo *ult;

} fn_fila;

fn_fila *fn_cria();

fn_nodo *fn_insere(fn_fila *fila, const char *titulo, const char *texto);

void fn_atualiza(fn_fila *fila);

int fn_qte_noticias_validas(fn_fila *fila);

int fn_remove(fn_fila *fila, char *titulo, char *texto);

int fn_vazia(fn_fila *fila);

void fn_destroi(fn_fila **fila);

#endif // !FILA_NOTICIAS_H
