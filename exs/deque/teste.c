#include "deque.h"
#include <stdio.h>

#define MAX_INDEX 10

int main(void)
{
    deque *dq;
    int i, val;

    printf("\nCRIA DEQUE");

    if ( !(dq = dq_cria()) ) {
        fprintf(stderr, "ERRO NA ALOCACAO EM dq_cria()\n");
        return 1;
    }

    printf("\tTAMANHO: %d\n", dq_tamanho(dq));

    printf("\n=== INSERE NO INICIO ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        dq_insere_inicio(dq, i);
        printf("INSERE %d\tTAMANHO: %d\n", i, dq_tamanho(dq));
    }

    printf("\n=== RETIRA DO INICIO ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        dq_retira_inicio(dq, &val);
        printf("RETIRA %d\tTAMANHO: %d\n", val, dq_tamanho(dq));
    }

    printf("\n=== INSERE NO FIM ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        dq_insere_fim(dq, i);
        printf("INSERE %d\tTAMANHO: %d\n", i, dq_tamanho(dq));
    }

    printf("\n=== RETIRA DO FIM ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        dq_retira_fim(dq, &val);
        printf("RETIRA %d\tTAMANHO: %d\n", val, dq_tamanho(dq));
    }

    printf("\n=== INSERE NO INICIO ===\n\n");

    for (i = 1; i <= MAX_INDEX; i++) {
        dq_insere_inicio(dq, i);
        printf("INSERE %d\tTAMANHO: %d\n", i, dq_tamanho(dq));
    }

    printf("\nDESTROI DEQUE\n\n");

    dq_destroi(&dq);

    return 0;
}
