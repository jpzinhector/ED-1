#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    printf("=== Agenda inicial ===\n");
    exibir_agenda(agenda);

    Missao m1 = criar_missao(1, "Resgatar o cristal", 5);
    Missao m2 = criar_missao(2, "Entregar mensagem", 2);
    Missao m3 = criar_missao(3, "Explorar a caverna", 9);
    Missao m4 = criar_missao(4, "Defender a muralha", 4);

    concluir_missao(&m2);

    adicionar_missao_fim(agenda, m1);
    adicionar_missao_inicio(agenda, m2);
    adicionar_missao_fim(agenda, m3);
    adicionar_missao_inicio(agenda, m4);

    printf("\n=== Agenda apos insercoes no inicio e no fim ===\n");
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}