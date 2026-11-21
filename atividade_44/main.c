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

    printf("\n=== Busca por identificador ===\n");
    Missao *encontrada = buscar_missao_por_id(agenda, 1);
    if (encontrada != NULL) {
        printf("Missao encontrada: ");
        exibir_missao(encontrada);
    } else {
        printf("Missao 1 nao encontrada.\n");
    }

    if (buscar_missao_por_id(agenda, 99) == NULL) {
        printf("Missao 99 nao encontrada.\n");
    }

    printf("\n=== Insercao apos missao de referencia ===\n");
    Missao m5 = criar_missao(5, "Mapear a saida", 3);
    if (adicionar_missao_apos(agenda, 1, m5)) {
        printf("Missao 5 inserida apos a missao 1.\n");
    } else {
        printf("Nao foi possivel inserir a missao 5 apos a missao 1.\n");
    }

    Missao m6 = criar_missao(6, "Montar acampamento", 2);
    if (adicionar_missao_apos(agenda, 3, m6)) {
        printf("Missao 6 inserida apos a missao 3 (ultimo no).\n");
    } else {
        printf("Nao foi possivel inserir a missao 6 apos a missao 3.\n");
    }

    Missao m7 = criar_missao(7, "Reforcar o portao", 4);
    if (adicionar_missao_apos(agenda, 99, m7)) {
        printf("Missao 7 inserida apos a missao 99.\n");
    } else {
        printf("Missao 99 nao encontrada. Lista mantida inalterada.\n");
    }

    printf("\n=== Agenda final ===\n");
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}