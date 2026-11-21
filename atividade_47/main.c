#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    Missao removida;

    adicionar_missao_fim(agenda, criar_missao(1, "Resgatar o cristal", 5));
    adicionar_missao_fim(agenda, criar_missao(2, "Entregar mensagem", 2));
    adicionar_missao_fim(agenda, criar_missao(3, "Explorar a caverna", 9));
    adicionar_missao_fim(agenda, criar_missao(4, "Defender a muralha", 4));
    adicionar_missao_fim(agenda, criar_missao(5, "Mapear a saida", 3));

    printf("=== Agenda inicial ===\n");
    exibir_agenda(agenda);

    printf("\n=== Remover apos o ID 2 ===\n");
    if (remover_missao_apos(agenda, 2, &removida)) {
        printf("Missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Tentar remover apos o ultimo no (ID 5) ===\n");
    if (!remover_missao_apos(agenda, 5, &removida)) {
        printf("Nao existe missao apos o ID 5.\n");
    }

    printf("\n=== Tentar remover apos referencia ausente (ID 99) ===\n");
    if (!remover_missao_apos(agenda, 99, &removida)) {
        printf("Referencia 99 nao encontrada.\n");
    }

    printf("\n=== Tentar remover antes do primeiro no (ID 1) ===\n");
    if (!remover_missao_antes(agenda, 1, &removida)) {
        printf("Nao existe missao antes do primeiro no (ID 1).\n");
    }

    printf("\n=== Remover antes do ID 5 ===\n");
    if (remover_missao_antes(agenda, 5, &removida)) {
        printf("Missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Remover antes do ID 2 (remove o primeiro no) ===\n");
    if (remover_missao_antes(agenda, 2, &removida)) {
        printf("Missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Tentar remover antes de referencia ausente (ID 99) ===\n");
    if (!remover_missao_antes(agenda, 99, &removida)) {
        printf("Referencia 99 nao encontrada.\n");
    }

    printf("\n=== Agenda final ===\n");
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}