#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    printf("=== Limpeza em lista ja vazia ===\n");
    limpar_agenda(agenda);
    exibir_agenda(agenda);

    adicionar_missao_fim(agenda, criar_missao(1, "Resgatar o cristal", 5));
    adicionar_missao_fim(agenda, criar_missao(2, "Entregar mensagem", 2));
    adicionar_missao_fim(agenda, criar_missao(3, "Explorar a caverna", 4));

    printf("\n=== Agenda inicial ===\n");
    exibir_agenda(agenda);

    printf("\n=== Alteracao de missao existente (ID 2) ===\n");
    Missao atualizada = criar_missao(999, "Entregar relatorio urgente", 5);
    concluir_missao(&atualizada);
    if (alterar_missao(agenda, 2, atualizada)) {
        printf("Missao 2 alterada (ID original preservado).\n");
    }
    exibir_agenda(agenda);

    printf("\n=== Tentativa de alteracao com dados invalidos ===\n");
    Missao invalida = { 2, "", 10, 0 };
    if (!alterar_missao(agenda, 2, invalida)) {
        printf("Alteracao recusada: dados da missao invalidos.\n");
    }

    printf("\n=== Tentativa de alteracao de ID inexistente (ID 99) ===\n");
    if (!alterar_missao(agenda, 99, atualizada)) {
        printf("Missao 99 nao encontrada.\n");
    }

    printf("\n=== Limpeza completa da campanha ===\n");
    limpar_agenda(agenda);
    exibir_agenda(agenda);

    printf("\n=== Nova campanha apos limpeza ===\n");
    adicionar_missao_fim(agenda, criar_missao(10, "Iniciar nova expedicao", 3));
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}