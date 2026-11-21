#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agenda.h"

#define ARQUIVO_PADRAO "agenda.bin"

static void ler_texto(const char *prompt, char *destino, int tamanho) {
    printf("%s", prompt);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }

    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    }
}

static int ler_inteiro(const char *prompt) {
    char buffer[64];
    int valor = 0;

    while (1) {
        ler_texto(prompt, buffer, (int)sizeof(buffer));
        if (sscanf(buffer, "%d", &valor) == 1) {
            return valor;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
}

static Missao ler_missao(void) {
    int id = ler_inteiro("ID: ");
    char titulo[TAM_TITULO];
    ler_texto("Titulo: ", titulo, TAM_TITULO);
    int prioridade = ler_inteiro("Prioridade (1 a 5): ");

    return criar_missao(id, titulo, prioridade);
}

static void exibir_menu(void) {
    printf("\n=== MENU AGENDA DE MISSOES ===\n");
    printf("1  - Exibir agenda\n");
    printf("2  - Adicionar no inicio\n");
    printf("3  - Adicionar no fim\n");
    printf("4  - Adicionar antes de um ID\n");
    printf("5  - Adicionar apos um ID\n");
    printf("6  - Importar lote de exemplo\n");
    printf("7  - Buscar por ID\n");
    printf("8  - Alterar missao por ID\n");
    printf("9  - Concluir missao por ID\n");
    printf("10 - Remover primeira missao\n");
    printf("11 - Remover ultima missao\n");
    printf("12 - Remover por ID\n");
    printf("13 - Remover antes de um ID\n");
    printf("14 - Remover apos um ID\n");
    printf("15 - Contar missoes (total e pendentes)\n");
    printf("16 - Ordenar por prioridade\n");
    printf("17 - Salvar agenda (%s)\n", ARQUIVO_PADRAO);
    printf("18 - Carregar agenda (%s)\n", ARQUIVO_PADRAO);
    printf("19 - Limpar agenda\n");
    printf("0  - Sair\n");
}

int main(void) {
    Agenda *agenda = inicializar_agenda();
    if (agenda == NULL) {
        printf("Erro ao inicializar a agenda.\n");
        return 1;
    }

    if (carregar_agenda(agenda, ARQUIVO_PADRAO)) {
        printf("Sessao restaurada de %s (%d missoes).\n",
               ARQUIVO_PADRAO,
               contar_missoes(agenda));
    } else {
        printf("Arquivo %s nao encontrado ou invalido. Iniciando com agenda vazia.\n",
               ARQUIVO_PADRAO);
    }

    int opcao;
    do {
        exibir_menu();
        opcao = ler_inteiro("Opcao: ");

        Missao m;
        Missao removida;
        int id;

        switch (opcao) {
            case 1:
                exibir_agenda(agenda);
                break;

            case 2:
                m = ler_missao();
                if (adicionar_missao_inicio(agenda, m)) {
                    printf("Missao adicionada no inicio.\n");
                } else {
                    printf("Falha ao adicionar missao.\n");
                }
                break;

            case 3:
                m = ler_missao();
                if (adicionar_missao_fim(agenda, m)) {
                    printf("Missao adicionada no fim.\n");
                } else {
                    printf("Falha ao adicionar missao.\n");
                }
                break;

            case 4:
                id = ler_inteiro("ID de referencia: ");
                m = ler_missao();
                if (adicionar_missao_antes(agenda, id, m)) {
                    printf("Missao inserida antes do ID %d.\n", id);
                } else {
                    printf("Referencia nao encontrada ou falha de alocacao.\n");
                }
                break;

            case 5:
                id = ler_inteiro("ID de referencia: ");
                m = ler_missao();
                if (adicionar_missao_apos(agenda, id, m)) {
                    printf("Missao inserida apos o ID %d.\n", id);
                } else {
                    printf("Referencia nao encontrada ou falha de alocacao.\n");
                }
                break;

            case 6: {
                Missao lote[3];
                lote[0] = criar_missao(101, "Resgatar o cristal", 5);
                lote[1] = criar_missao(102, "Entregar mensagem", 2);
                lote[2] = criar_missao(103, "Explorar a caverna", 4);

                printf("Total antes do lote: %d\n", contar_missoes(agenda));
                if (adicionar_lote_missoes(agenda, lote, 3)) {
                    printf("Lote importado. Total depois: %d\n", contar_missoes(agenda));
                } else {
                    printf("Falha ao importar lote.\n");
                }
                break;
            }

            case 7: {
                id = ler_inteiro("ID para busca: ");
                Missao *encontrada = buscar_missao_por_id(agenda, id);
                if (encontrada != NULL) {
                    exibir_missao(encontrada);
                } else {
                    printf("Missao %d nao encontrada.\n", id);
                }
                break;
            }

            case 8: {
                id = ler_inteiro("ID da missao a alterar: ");
                char titulo[TAM_TITULO];
                ler_texto("Novo titulo: ", titulo, TAM_TITULO);
                int prioridade = ler_inteiro("Nova prioridade (1 a 5): ");
                Missao novos = criar_missao(id, titulo, prioridade);
                if (alterar_missao(agenda, id, novos)) {
                    printf("Missao %d alterada.\n", id);
                } else {
                    printf("Falha ao alterar missao.\n");
                }
                break;
            }

            case 9: {
                id = ler_inteiro("ID da missao a concluir: ");
                Missao *alvo = buscar_missao_por_id(agenda, id);
                if (alvo != NULL) {
                    concluir_missao(alvo);
                    printf("Missao %d marcada como concluida.\n", id);
                } else {
                    printf("Missao %d nao encontrada.\n", id);
                }
                break;
            }

            case 10:
                if (remover_primeira_missao(agenda, &removida)) {
                    printf("Removida: ");
                    exibir_missao(&removida);
                } else {
                    printf("Agenda vazia.\n");
                }
                break;

            case 11:
                if (remover_ultima_missao(agenda, &removida)) {
                    printf("Removida: ");
                    exibir_missao(&removida);
                } else {
                    printf("Agenda vazia.\n");
                }
                break;

            case 12:
                id = ler_inteiro("ID para remover: ");
                if (remover_missao_por_id(agenda, id, &removida)) {
                    printf("Removida: ");
                    exibir_missao(&removida);
                } else {
                    printf("Missao %d nao encontrada.\n", id);
                }
                break;

            case 13:
                id = ler_inteiro("ID de referencia: ");
                if (remover_missao_antes(agenda, id, &removida)) {
                    printf("Removida: ");
                    exibir_missao(&removida);
                } else {
                    printf("Nao foi possivel remover antes do ID %d.\n", id);
                }
                break;

            case 14:
                id = ler_inteiro("ID de referencia: ");
                if (remover_missao_apos(agenda, id, &removida)) {
                    printf("Removida: ");
                    exibir_missao(&removida);
                } else {
                    printf("Nao foi possivel remover apos o ID %d.\n", id);
                }
                break;

            case 15:
                printf("Total: %d | Pendentes: %d\n",
                       contar_missoes(agenda),
                       contar_missoes_pendentes(agenda));
                break;

            case 16:
                ordenar_agenda_por_prioridade(agenda);
                printf("Agenda ordenada por prioridade.\n");
                break;

            case 17:
                if (salvar_agenda(agenda, ARQUIVO_PADRAO)) {
                    printf("Agenda salva em %s.\n", ARQUIVO_PADRAO);
                } else {
                    printf("Erro ao salvar agenda.\n");
                }
                break;

            case 18:
                if (carregar_agenda(agenda, ARQUIVO_PADRAO)) {
                    printf("Agenda recarregada de %s (%d missoes).\n",
                           ARQUIVO_PADRAO,
                           contar_missoes(agenda));
                } else {
                    printf("Nao foi possivel carregar %s.\n", ARQUIVO_PADRAO);
                }
                break;

            case 19:
                limpar_agenda(agenda);
                printf("Agenda limpa.\n");
                break;

            case 0:
                printf("Encerrando e liberando memoria...\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    liberar_agenda(agenda);

    return 0;
}