#include "fila_noticias.h"
#include <stdio.h>

static char menu()
{
    char opcao;

    printf("-   (1) Cadastrar notícia\n");
    printf("-   (2) Fechar edição\n");
    printf("-   (3) Sair\n");
    printf("> ");
    fflush(stdout);

    opcao = getchar();

    while (getchar() != '\n');

    return opcao;
}

// retorna 0 se eh breaking news, 1 se eh informe
static unsigned char prioridade()
{
    char opcao;

    printf("Breaking news? [s/N] ");
    fflush(stdout);

    opcao = getchar();
    while (getchar() != '\n');

    switch (opcao) {
    case 's':
    case 'S':
        return 0;
    }

    return 1;
}

static void cadastrar_noticia(fn_fila *fn)
{
    char titulo[FN_TAM_TITULO], texto[FN_TAM_TEXTO];

    if (!fn)
        return;

    printf("Insira o título: ");
    fflush(stdout);
    fgets(titulo, FN_TAM_TITULO, stdin);

    printf("Insira o texto: ");
    fflush(stdout);
    fgets(texto, FN_TAM_TEXTO, stdin);

    fn_insere(fn, titulo, texto);

    return;
}

static void imprime_edicao(char **titulo, char **texto)
{
    if ( !titulo || !texto )
        return;

    if ( !(titulo[0]) || !(texto[0]) || !(titulo[1]) || !(texto[1]))
        return;

    printf("\n=======================================================\n");
    printf("\n%s\n", titulo[0]);
    printf("\n%s\n", texto[0]);
    printf("\n==\n");
    printf("\n%s\n", titulo[1]);
    printf("\n%s\n", texto[1]);
    printf("\n=======================================================\n");

    return;
}

static void fechar_edicao(fn_fila *breakn, fn_fila *infs)
{
    char titulo[2][FN_TAM_TITULO];
    char texto[2][FN_TAM_TEXTO];
    int i;

    if ( !breakn || !infs )
        return;

    for (i = 0; i < 2 && !fn_vazia(breakn); i++)
        fn_remove(breakn, titulo[i], texto[i]);

    for ( ; i < 2 && !fn_vazia(infs); i++)
        fn_remove(infs, titulo[i], texto[i]);

    if (i < 2) {
        printf("Esta edição foi pulada por falta de notícias!\n");
        return;
    }

    imprime_edicao((char**)titulo, (char**)texto);

    return;
}

int main(void)
{
    fn_fila *breaking_news, *informes;
    char opcao;

    if ( !(breaking_news = fn_cria()) || !(informes = fn_cria()) ) {
        fprintf(stderr, "FALHA DE ALOCAÇÃO EM fn_cria()\n");
        return 1;
    }

    do {
        opcao = menu();

        switch (opcao) {
        case '1':
            if (prioridade())
                cadastrar_noticia(informes);
            else
                cadastrar_noticia(breaking_news);
            break;
        case '2':
            fechar_edicao(breaking_news, informes);
            break;
        case '3':
            goto exit;
        default:
            printf("Opção inválida!\n");
            break;
        }
    } while (1);

exit:
    fn_destroi(&breaking_news);
    fn_destroi(&informes);

    return 0;
}
