#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30
#define N_NUMEROS 10
#define N_JOGADORES 8

typedef struct {
    long comparacoes;
    long trocas;
} Metricas;

typedef struct {
    int id;
    char nome[TAM_NOME];
    int pontuacao;
} Jogador;

typedef struct {
    long comparacoes;
    long deslocamentos;
} Contadores;

int comparar_jogadores_ranking(const Jogador *a, const Jogador *b) {
    if (a->pontuacao != b->pontuacao) {
        return b->pontuacao - a->pontuacao;
    }
    int r = strcmp(a->nome, b->nome);
    if (r != 0) {
        return r;
    }
    return (a->id > b->id) - (a->id < b->id);
}

int indice_do_menor(int v[], int n, int inicio, Metricas *m) {
    int menor = inicio;

    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++;
        if (v[i] < v[menor]) {
            menor = i;
        }
    }

    return menor;
}

void selection_sort(int v[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int menor = indice_do_menor(v, n, i, m);

        if (menor != i) {
            int aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
            m->trocas++;
        }
    }
}

int indice_do_melhor_jogador(Jogador v[], int n, int inicio, Metricas *m) {
    int melhor = inicio;

    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++;
        if (comparar_jogadores_ranking(&v[i], &v[melhor]) < 0) {
            melhor = i;
        }
    }

    return melhor;
}

void selection_sort_jogadores(Jogador v[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int melhor = indice_do_melhor_jogador(v, n, i, m);

        if (melhor != i) {
            Jogador aux = v[i];
            v[i] = v[melhor];
            v[melhor] = aux;
            m->trocas++;
        }
    }
}

int numeros_ordenados(const int v[], int n) {
    for (int i = 1; i < n; i++) {
        if (v[i - 1] > v[i]) {
            return 0;
        }
    }
    return 1;
}

int jogadores_ordenados_ranking(const Jogador v[], int n) {
    for (int i = 1; i < n; i++) {
        if (comparar_jogadores_ranking(&v[i - 1], &v[i]) > 0) {
            return 0;
        }
    }
    return 1;
}

int mesma_soma(const int a[], const int b[], int n) {
    long sa = 0;
    long sb = 0;
    for (int i = 0; i < n; i++) {
        sa += a[i];
        sb += b[i];
    }
    return sa == sb;
}

int jogadores_preservados(const Jogador original[], const Jogador ordenado[], int n) {
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

void imprimir_numeros(const char *titulo, const int v[], int n) {
    printf("%s:", titulo);
    for (int i = 0; i < n; i++) {
        printf(" %d", v[i]);
    }
    printf("\n");
}

void imprimir_jogadores(const char *titulo, const Jogador v[], int n) {
    printf("%s\n", titulo);
    printf("%-5s %-12s %s\n", "ID", "Nome", "Pontos");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-12s %d\n", v[i].id, v[i].nome, v[i].pontuacao);
    }
    printf("\n");
}

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

int comparar_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int mesmo_conjunto(const int original[], const int ordenado[], int n) {
    int *ref = malloc((n > 0 ? n : 1) * sizeof(int));
    if (ref == NULL) {
        return 0;
    }
    memcpy(ref, original, n * sizeof(int));
    qsort(ref, n, sizeof(int), comparar_int);
    int ok = memcmp(ref, ordenado, n * sizeof(int)) == 0;
    free(ref);
    return ok;
}

void executar_selection_numeros(const int original[]) {
    int copia[N_NUMEROS];
    int guarda[N_NUMEROS];
    Metricas m = {0, 0};

    memcpy(copia, original, sizeof(copia));
    memcpy(guarda, original, sizeof(guarda));

    imprimir_numeros("Original", original, N_NUMEROS);
    selection_sort(copia, N_NUMEROS, &m);
    imprimir_numeros("Copia ordenada", copia, N_NUMEROS);

    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Ordem crescente: %s\n", numeros_ordenados(copia, N_NUMEROS) ? "OK" : "FALHOU");
    printf("Mesmos valores: %s\n", mesma_soma(original, copia, N_NUMEROS) ? "OK" : "FALHOU");
    printf("Original preservado: %s\n", memcmp(original, guarda, sizeof(guarda)) == 0 ? "OK" : "FALHOU");
}

void executar_selection_jogadores(const Jogador original[]) {
    Jogador copia[N_JOGADORES];
    Jogador guarda[N_JOGADORES];
    Metricas m = {0, 0};

    memcpy(copia, original, sizeof(copia));
    memcpy(guarda, original, sizeof(guarda));

    imprimir_jogadores("Original:", original, N_JOGADORES);
    selection_sort_jogadores(copia, N_JOGADORES, &m);
    imprimir_jogadores("Copia ordenada por pontuacao (maior primeiro):", copia, N_JOGADORES);

    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Ordem por pontuacao, nome e id: %s\n", jogadores_ordenados_ranking(copia, N_JOGADORES) ? "OK" : "FALHOU");
    printf("Registros preservados: %s\n", jogadores_preservados(original, copia, N_JOGADORES) ? "OK" : "FALHOU");
    printf("Original preservado: %s\n", memcmp(original, guarda, sizeof(guarda)) == 0 ? "OK" : "FALHOU");
}

void executar_insertion_inteiros(void) {
    int original[] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int n = sizeof(original) / sizeof(original[0]);

    int *copia = malloc(n * sizeof(int));
    if (copia == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(copia, original, n * sizeof(int));

    Contadores c = {0, 0};

    imprimir_numeros("Original", original, n);
    insertion_sort(copia, n, &c);
    imprimir_numeros("Ordenado", copia, n);

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);

    if (numeros_ordenados(copia, n) && mesmo_conjunto(original, copia, n)) {
        printf("Validacao: OK\n");
    } else {
        printf("Validacao: FALHOU\n");
    }

    free(copia);
}

int main(void) {
    const int numeros_originais[N_NUMEROS] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};

    const Jogador jogadores_originais[N_JOGADORES] = {
        {101, "Marcos", 850},
        {102, "Ana", 920},
        {103, "Carlos", 730},
        {104, "Ana", 920},
        {105, "Beatriz", 990},
        {106, "Marcos", 850},
        {107, "Daniel", 505},
        {108, "Ana", 920}
    };

    int opcao;

    do {
        printf("\n===== CENTRAL =====\n");
        printf("1 - Selection Sort (vetor numerico)\n");
        printf("2 - Selection Sort (jogadores por pontuacao)\n");
        printf("3 - Insertion Sort (vetor de inteiros)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida\n");
            return 1;
        }
        printf("\n");

        switch (opcao) {
            case 1:
                executar_selection_numeros(numeros_originais);
                break;
            case 2:
                executar_selection_jogadores(jogadores_originais);
                break;
            case 3:
                executar_insertion_inteiros();
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