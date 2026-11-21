#include <stdio.h>
#include "agenda.h"

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    printf("=== Contagem inicial (lista vazia) ===\n");
    printf("Total de missoes: %d | Pendentes: %d\n",
           contar_missoes(agenda),
           contar_missoes_pendentes(agenda));

    Missao m1 = criar_missao(1, "Resgatar o cristal", 5);
    Missao m2 = criar_missao(2, "Entregar mensagem", 2);
    concluir_missao(&m2);

    adicionar_missao_fim(agenda, m1);
    adicionar_missao_fim(agenda, m2);

    printf("\n=== Antes da importacao do lote ===\n");
    printf("Total de missoes: %d | Pendentes: %d\n",
           contar_missoes(agenda),
           contar_missoes_pendentes(agenda));
    exibir_agenda(agenda);

    Missao lote[3];
    lote[0] = criar_missao(3, "Explorar a caverna", 4);
    lote[1] = criar_missao(4, "Defender a muralha", 5);
    lote[2] = criar_missao(5, "Mapear a saida", 3);

    printf("\n=== Tentativa de importacao com quantidade invalida ===\n");
    if (!adicionar_lote_missoes(agenda, lote, 0)) {
        printf("Importacao recusada: quantidade invalida.\n");
    }

    printf("\n=== Importacao de lote (preservando ordem no fim) ===\n");
    if (adicionar_lote_missoes(agenda, lote, 3)) {
        printf("Lote importado com sucesso.\n");
    } else {
        printf("Falha ao importar o lote.\n");
    }

    printf("\n=== Depois da importacao do lote ===\n");
    printf("Total de missoes: %d | Pendentes: %d\n",
           contar_missoes(agenda),
           contar_missoes_pendentes(agenda));
    exibir_agenda(agenda);

    liberar_agenda(agenda);

    return 0;
}