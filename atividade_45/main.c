#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    printf("=== Teste em lista com um unico no ===\n");
    Missao unica = criar_missao(10, "Missao unica", 3);
    adicionar_missao_inicio(agenda, unica);
    exibir_agenda(agenda);

    Missao removida;
    if (remover_primeira_missao(agenda, &removida)) {
        printf("Removida: ");
        exibir_missao(&removida);
    }

    if (!remover_primeira_missao(agenda, &removida)) {
        printf("Remocao bloqueada: agenda vazia.\n");
    }
    exibir_agenda(agenda);

    printf("\n=== Teste em lista com varios nos ===\n");
    Missao m1 = criar_missao(1, "Resgatar o cristal", 5);
    Missao m2 = criar_missao(2, "Entregar mensagem", 2);
    Missao m3 = criar_missao(3, "Explorar a caverna", 9);
    Missao m4 = criar_missao(4, "Defender a muralha", 4);

    concluir_missao(&m2);

    adicionar_missao_fim(agenda, m1);
    adicionar_missao_inicio(agenda, m2);
    adicionar_missao_fim(agenda, m3);
    adicionar_missao_inicio(agenda, m4);

    printf("Agenda apos insercoes no inicio e no fim:\n");
    exibir_agenda(agenda);

    Missao m5 = criar_missao(5, "Mapear a saida", 3);
    adicionar_missao_apos(agenda, 1, m5);

    printf("\n=== Insercao antes de identificador ===\n");
    Missao m6 = criar_missao(6, "Alerta urgente", 5);
    if (adicionar_missao_antes(agenda, 4, m6)) {
        printf("Missao 6 inserida antes da missao 4 (novo inicio).\n");
    }

    Missao m7 = criar_missao(7, "Preparar suprimentos", 4);
    if (adicionar_missao_antes(agenda, 1, m7)) {
        printf("Missao 7 inserida antes da missao 1.\n");
    }

    Missao m8 = criar_missao(8, "Missao ignorada", 1);
    if (!adicionar_missao_antes(agenda, 99, m8)) {
        printf("Missao 99 nao encontrada. Lista mantida inalterada.\n");
    }

    exibir_agenda(agenda);

    printf("\n=== Remocao do primeiro no ===\n");
    if (remover_primeira_missao(agenda, &removida)) {
        concluir_missao(&removida);
        printf("Primeira missao concluida e removida: ");
        exibir_missao(&removida);
    }

    printf("\n=== Agenda final ===\n");
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}