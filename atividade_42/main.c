#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    Missao m1 = criar_missao(1, "Resgatar o cristal", 5);
    Missao m2 = criar_missao(2, "Entregar mensagem", 2);
    Missao m3 = criar_missao(3, "Explorar a caverna", 9);

    concluir_missao(&m2);

    adicionar_missao(agenda, m1);
    adicionar_missao(agenda, m2);
    adicionar_missao(agenda, m3);

    printf("=== Agenda de missoes ===\n");
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}