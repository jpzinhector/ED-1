#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30

typedef struct {
    long comparacoes;
    long deslocamentos;
} Contadores;

typedef struct {
    int id;
    char nome[TAM_NOME];
    int pontuacao;
} Jogador;

void inserir_no_prefixo(int v[], int fim, Contadores *c) {
    int chave = v[fim];
    int j = fim - 1;

    while (j >= 0) {
        c->comparacoes++;
        if (v[j] > chave) {
            v[j + 1] = v[j];
            c->deslocamentos++;
            j--;
        } else {
            break;
        }
    }

    v[j + 1] = chave;
}

void insertion_sort(int v[], int n, Contadores *c) {
    for (int i = 1; i < n; i++) {
        inserir_no_prefixo(v, i, c);
    }
}

int esta_ordenado(int v[], int n) {
    for (int i = 1; i < n; i++) {
        if (v[i - 1] > v[i]) {
            return 0;
        }
    }
    return 1;
}

int comparar(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int mesmo_conjunto(int original[], int ordenado[], int n) {
    int *ref = malloc(n * sizeof(int));
    if (ref == NULL) {
        return 0;
    }
    memcpy(ref, original, n * sizeof(int));
    qsort(ref, n, sizeof(int), comparar);
    int ok = memcmp(ref, ordenado, n * sizeof(int)) == 0;
    free(ref);
    return ok;
}

void imprimir(const char *titulo, int v[], int n) {
    printf("%s:", titulo);
    for (int i = 0; i < n; i++) {
        printf(" %d", v[i]);
    }
    printf("\n");
}

int comparar_jogadores(const Jogador *a, const Jogador *b) {
    int r = strcmp(a->nome, b->nome);
    if (r != 0) {
        return r;
    }
    return (a->id > b->id) - (a->id < b->id);
}

void inserir_jogador_no_prefixo(Jogador v[], int fim, Contadores *c) {
    Jogador chave = v[fim];
    int j = fim - 1;

    while (j >= 0) {
        c->comparacoes++;
        if (comparar_jogadores(&v[j], &chave) > 0) {
            v[j + 1] = v[j];
            c->deslocamentos++;
            j--;
        } else {
            break;
        }
    }

    v[j + 1] = chave;
}

void insertion_sort_jogadores(Jogador v[], int n, Contadores *c) {
    for (int i = 1; i < n; i++) {
        inserir_jogador_no_prefixo(v, i, c);
    }
}

int jogadores_ordenados(Jogador v[], int n) {
    for (int i = 1; i < n; i++) {
        if (comparar_jogadores(&v[i - 1], &v[i]) > 0) {
            return 0;
        }
    }
    return 1;
}

int associacao_preservada(Jogador original[], Jogador ordenado[], int n) {
    for (int i = 0; i < n; i++) {
        int achou = 0;
        for (int k = 0; k < n; k++) {
            if (original[k].id == ordenado[i].id) {
                if (strcmp(original[k].nome, ordenado[i].nome) != 0 ||
                    original[k].pontuacao != ordenado[i].pontuacao) {
                    return 0;
                }
                achou = 1;
                break;
            }
        }
        if (!achou) {
            return 0;
        }
    }
    return 1;
}

void imprimir_jogadores(const char *titulo, Jogador v[], int n) {
    printf("%s\n", titulo);
    printf("%-5s %-15s %s\n", "ID", "Nome", "Pontos");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-15s %d\n", v[i].id, v[i].nome, v[i].pontuacao);
    }
    printf("\n");
}

void executar_inteiros(void) {
    int original[] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int n = sizeof(original) / sizeof(original[0]);

    int *copia = malloc(n * sizeof(int));
    if (copia == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(copia, original, n * sizeof(int));

    Contadores c = {0, 0};

    imprimir("Original", original, n);
    insertion_sort(copia, n, &c);
    imprimir("Ordenado", copia, n);

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);

    if (esta_ordenado(copia, n) && mesmo_conjunto(original, copia, n)) {
        printf("Validacao: OK\n");
    } else {
        printf("Validacao: FALHOU\n");
    }

    free(copia);
}

void executar_jogadores(void) {
    Jogador original[] = {
        {101, "Marcos", 850},
        {102, "Ana", 920},
        {103, "Carlos", 730},
        {104, "Ana", 810},
        {105, "Beatriz", 990},
        {106, "Marcos", 640},
        {107, "Ana", 920},
        {108, "Daniel", 505}
    };
    int n = sizeof(original) / sizeof(original[0]);

    Jogador *copia = malloc(n * sizeof(Jogador));
    if (copia == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(copia, original, n * sizeof(Jogador));

    Contadores c = {0, 0};

    imprimir_jogadores("Antes da ordenacao:", copia, n);
    insertion_sort_jogadores(copia, n, &c);
    imprimir_jogadores("Depois da ordenacao:", copia, n);

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);

    int ordem_ok = jogadores_ordenados(copia, n);
    int assoc_ok = associacao_preservada(original, copia, n);

    printf("Ordem por nome e desempate por id: %s\n", ordem_ok ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", assoc_ok ? "OK" : "FALHOU");

    free(copia);
}

int main(void) {
    int opcao;

    do {
        printf("\n===== CENTRAL =====\n");
        printf("1 - Insertion Sort (vetor de inteiros)\n");
        printf("2 - Insertion Sort (jogadores por nome)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida\n");
            return 1;
        }
        printf("\n");

        switch (opcao) {
            case 1:
                executar_inteiros();
                break;
            case 2:
                executar_jogadores();
                break;
            case 0:
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    return 0;
}