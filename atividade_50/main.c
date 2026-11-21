#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    Missao lote[5];
    lote[0] = criar_missao(4, "Defender a muralha", 3);
    lote[1] = criar_missao(2, "Entregar mensagem", 5);
    lote[2] = criar_missao(5, "Mapear a saida", 1);
    lote[3] = criar_missao(1, "Resgatar o cristal", 5);
    lote[4] = criar_missao(3, "Explorar a caverna", 3);

    adicionar_lote_missoes(agenda, lote, 5);

    printf("=== Agenda antes da ordenacao (total: %d) ===\n", contar_missoes(agenda));
    exibir_agenda(agenda);

    ordenar_agenda_por_prioridade(agenda);

    printf("\n=== Agenda ordenada por prioridade (total: %d) ===\n", contar_missoes(agenda));
    exibir_agenda(agenda);

    printf("\n=== Salvamento em arquivo ===\n");
    if (salvar_agenda(agenda, "agenda.bin")) {
        printf("Agenda salva com sucesso em agenda.bin.\n");
    } else {
        printf("Erro ao salvar a agenda em agenda.bin.\n");
    }

    if (!salvar_agenda(agenda, "")) {
        printf("Falha esperada ao tentar salvar em caminho invalido.\n");
    }

    liberar_agenda(agenda);

    return 0;
}