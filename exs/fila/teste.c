#include "fila.h"
#include <stdio.h>

#define MAX_INDEX 10

int main(void)
{
    fila *fl;
    int i, val;

    printf("\nCRIA FILA");

    if ( !(fl = fl_cria()) ) {
        fprintf(stderr, "ERRO NA ALOCACAO EM fl_cria()\n");
        return 1;
    }

    printf("\tTAMANHO: %d\n", fl_tamanho(fl));

    printf("\n=== INSERE ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        fl_insere(fl, i);
        printf("INSERE %d\tTAMANHO: %d\n", i, fl_tamanho(fl));
    }

    printf("\n=== RETIRA ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        fl_retira(fl, &val);
        printf("RETIRA %d\tTAMANHO: %d\n", val, fl_tamanho(fl));
    }

    printf("\n=== INSERE ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        fl_insere(fl, i);
        printf("INSERE %d\tTAMANHO: %d\n", i, fl_tamanho(fl));
    }

    printf("\nDESTROI FILA\n\n");

    fl_destroi(&fl);

    return 0;
}
