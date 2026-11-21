#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    Missao removida;

    printf("=== Testes em lista vazia ===\n");
    if (!remover_ultima_missao(agenda, &removida)) {
        printf("Remocao do fim bloqueada: agenda vazia.\n");
    }
    if (!remover_missao_por_id(agenda, 1, &removida)) {
        printf("Remocao por ID bloqueada: nenhum registro encontrado.\n");
    }
    exibir_agenda(agenda);

    printf("\n=== Teste em lista com um unico elemento (remover fim) ===\n");
    adicionar_missao_inicio(agenda, criar_missao(10, "Missao unica A", 3));
    exibir_agenda(agenda);
    if (remover_ultima_missao(agenda, &removida)) {
        printf("Ultima missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Teste em lista com um unico elemento (remover por ID) ===\n");
    adicionar_missao_inicio(agenda, criar_missao(11, "Missao unica B", 2));
    exibir_agenda(agenda);
    if (remover_missao_por_id(agenda, 11, &removida)) {
        printf("Missao removida por ID: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Montagem da lista com varios elementos ===\n");
    adicionar_missao_fim(agenda, criar_missao(1, "Resgatar o cristal", 5));
    adicionar_missao_inicio(agenda, criar_missao(2, "Entregar mensagem", 2));
    adicionar_missao_fim(agenda, criar_missao(3, "Explorar a caverna", 9));
    adicionar_missao_inicio(agenda, criar_missao(4, "Defender a muralha", 4));
    adicionar_missao_apos(agenda, 1, criar_missao(5, "Mapear a saida", 3));
    exibir_agenda(agenda);

    printf("\n=== Remover ultimo no ===\n");
    if (remover_ultima_missao(agenda, &removida)) {
        printf("Ultima missao cancelada: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Remover no do meio por ID (ID 1) ===\n");
    if (remover_missao_por_id(agenda, 1, &removida)) {
        printf("Missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Remover no do inicio por ID (ID 4) ===\n");
    if (remover_missao_por_id(agenda, 4, &removida)) {
        printf("Missao removida: ");
        exibir_missao(&removida);
    }
    exibir_agenda(agenda);

    printf("\n=== Tentar remover ID inexistente (ID 99) ===\n");
    if (!remover_missao_por_id(agenda, 99, &removida)) {
        printf("Nenhuma missao com ID 99 encontrada. Lista inalterada.\n");
    }
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}