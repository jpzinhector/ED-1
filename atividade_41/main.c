/*
 * main.c - programa que USA o modulo agenda.
 *
 * Repare que so ha #include "agenda.h": nunca se inclui agenda.c.
 * O main usa apenas o que a interface expoe (Missao, criar_missao,
 * exibir_missao, concluir_missao e as constantes de limite).
 */

#include <stdio.h>
#include "agenda.h"

#define QTD_MISSOES 3

int main(void) {
    /* vetor local: a agenda vive dentro da main, sem variavel global */
    Missao agenda[QTD_MISSOES];

    agenda[0] = criar_missao(1, "Resgatar o cristal", 5);
    agenda[1] = criar_missao(2, "Entregar mensagem", 2);
    /* prioridade 9 passa do limite: criar_missao ajusta para PRIORIDADE_MAX */
    agenda[2] = criar_missao(3, "Explorar a caverna", 9);

    printf("=== Agenda inicial ===\n");
    for (int i = 0; i < QTD_MISSOES; i++) {
        exibir_missao(&agenda[i]);
    }

    /* conclui a segunda missao usando a funcao do modulo */
    concluir_missao(&agenda[1]);

    printf("\n=== Depois de concluir a missao 2 ===\n");
    for (int i = 0; i < QTD_MISSOES; i++) {
        exibir_missao(&agenda[i]);
    }

    return 0;
}
