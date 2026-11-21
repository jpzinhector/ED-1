#include <stdio.h>
#include <stdlib.h>


#define MAX_PARTIDAS 10000


typedef struct {
    int identificador;
    char jogador[50];
    int pontuacao;
} Partida;


void cadastrarPartida(Partida **partidas, int *quantidade) {
    Partida *temporario;


    temporario = realloc(*partidas, (*quantidade + 1) * sizeof(Partida));


    if (temporario == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }


    *partidas = temporario;


    printf("\nIdentificador: ");
    scanf("%d", &(*partidas)[*quantidade].identificador);


    printf("Jogador: ");
    scanf("%49s", (*partidas)[*quantidade].jogador);


    printf("Pontuacao: ");
    scanf("%d", &(*partidas)[*quantidade].pontuacao);


    (*quantidade)++;


    printf("Partida cadastrada com sucesso.\n");
}


void listarPartidas(Partida *partidas, int quantidade) {
    if (quantidade == 0) {
        printf("\nNenhuma partida cadastrada.\n");
        return;
    }


    printf("\n===== PARTIDAS =====\n");


    for (int i = 0; i < quantidade; i++) {
        printf("\nPartida %d\n", i + 1);
        printf("Identificador: %d\n", partidas[i].identificador);
        printf("Jogador: %s\n", partidas[i].jogador);
        printf("Pontuacao: %d\n", partidas[i].pontuacao);
    }


    printf("\nTotal de partidas: %d\n", quantidade);
}


void salvarTemporada(Partida *partidas, int quantidade) {
    FILE *arquivo = fopen("temporada.bin", "wb");


    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo para salvar.\n");
        return;
    }


    size_t gravados;


    gravados = fwrite(&quantidade, sizeof(int), 1, arquivo);


    if (gravados != 1) {
        printf("Erro ao gravar a quantidade de partidas.\n");
        fclose(arquivo);
        return;
    }


    if (quantidade > 0) {
        gravados = fwrite(partidas, sizeof(Partida), quantidade, arquivo);


        if (gravados != (size_t)quantidade) {
            printf("Erro: nem todas as partidas foram gravadas.\n");
            fclose(arquivo);
            return;
        }
    }


    fclose(arquivo);


    printf("\nTemporada salva com sucesso.\n");
    printf("Partidas salvas: %d\n", quantidade);
}


int carregarTemporada(Partida **partidas, int *quantidade) {
    FILE *arquivo = fopen("temporada.bin", "rb");


    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo da temporada.\n");
        return 0;
    }


    int novaQuantidade;
    Partida *novoVetor = NULL;
    size_t lidos;


    lidos = fread(&novaQuantidade, sizeof(int), 1, arquivo);


    if (lidos != 1) {
        printf("Erro: nao foi possivel ler a quantidade de partidas.\n");
        fclose(arquivo);
        return 0;
    }


    if (novaQuantidade < 0 || novaQuantidade > MAX_PARTIDAS) {
        printf("Erro: quantidade de partidas invalida.\n");
        fclose(arquivo);
        return 0;
    }


    if (novaQuantidade > 0) {
        novoVetor = malloc(novaQuantidade * sizeof(Partida));


        if (novoVetor == NULL) {
            printf("Erro: nao foi possivel alocar memoria.\n");
            fclose(arquivo);
            return 0;
        }


        lidos = fread(novoVetor, sizeof(Partida), novaQuantidade, arquivo);


        if (lidos != (size_t)novaQuantidade) {
            printf("Erro: arquivo incompleto ou corrompido.\n");
            free(novoVetor);
            fclose(arquivo);
            return 0;
        }
    }


    fclose(arquivo);


    free(*partidas);


    *partidas = novoVetor;
    *quantidade = novaQuantidade;


    printf("\nTemporada carregada com sucesso.\n");
    printf("Partidas carregadas: %d\n", *quantidade);


    return 1;
}


int main() {
    Partida *partidas = NULL;
    int quantidade = 0;
    int opcao;


    do {
        printf("\n===== GERENCIADOR DE TEMPORADA =====\n");
        printf("1 - Cadastrar partida\n");
        printf("2 - Listar partidas\n");
        printf("3 - Salvar temporada\n");
        printf("4 - Carregar temporada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);


        switch (opcao) {
            case 1:
                cadastrarPartida(&partidas, &quantidade);
                break;


            case 2:
                listarPartidas(partidas, quantidade);
                break;


            case 3:
                salvarTemporada(partidas, quantidade);
                break;


            case 4:
                carregarTemporada(&partidas, &quantidade);
                break;


            case 0:
                printf("\nEncerrando programa...\n");
                break;


            default:
                printf("\nOpcao invalida.\n");
        }


    } while (opcao != 0);


    free(partidas);


    return 0;
}