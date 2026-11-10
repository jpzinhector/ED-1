#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30
#define N_NUMEROS 10
#define N_JOGADORES 8
#define TAM_TESTE 10

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

typedef struct {
    long comparacoes;
    long trocas;
    long passagens;
} MetricasBubble;

typedef enum {
    CAMPO_ID,
    CAMPO_PONTUACAO
} Campo;

typedef enum {
    CRESCENTE,
    DECRESCENTE
} Direcao;

typedef struct {
    long comparacoes;
    long copias;
} MetricasMerge;

typedef struct {
    Campo campo;
    Direcao dir;
} CriterioJogador;

typedef int (*FuncComparacao)(const void *, const void *, const void *);

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

int jogadores_ordenados(const Jogador v[], int n) {
    for (int i = 1; i < n; i++) {
        if (comparar_jogadores(&v[i - 1], &v[i]) > 0) {
            return 0;
        }
    }
    return 1;
}

void bubble_sort(int v[], int n, MetricasBubble *m) {
    int limite = n - 1;
    int houve_troca = 1;

    while (houve_troca && limite > 0) {
        houve_troca = 0;
        m->passagens++;

        for (int i = 0; i < limite; i++) {
            m->comparacoes++;
            if (v[i] > v[i + 1]) {
                int aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                m->trocas++;
                houve_troca = 1;
            }
        }

        limite--;
    }
}

int comparar_por_criterio(const Jogador *a, const Jogador *b, Campo campo, Direcao dir) {
    int r;

    if (campo == CAMPO_ID) {
        r = (a->id > b->id) - (a->id < b->id);
    } else {
        r = (a->pontuacao > b->pontuacao) - (a->pontuacao < b->pontuacao);
    }

    if (dir == DECRESCENTE) {
        r = -r;
    }

    if (r != 0) {
        return r;
    }

    return (a->id > b->id) - (a->id < b->id);
}

void trocar_jogadores(Jogador *a, Jogador *b) {
    Jogador aux = *a;
    *a = *b;
    *b = aux;
}

void bubble_sort_jogadores(Jogador v[], int n, Campo campo, Direcao dir, MetricasBubble *m) {
    int limite = n - 1;
    int houve_troca = 1;

    while (houve_troca && limite > 0) {
        houve_troca = 0;
        m->passagens++;

        for (int i = 0; i < limite; i++) {
            m->comparacoes++;
            if (comparar_por_criterio(&v[i], &v[i + 1], campo, dir) > 0) {
                trocar_jogadores(&v[i], &v[i + 1]);
                m->trocas++;
                houve_troca = 1;
            }
        }

        limite--;
    }
}

int jogadores_ordenados_por(const Jogador v[], int n, Campo campo, Direcao dir) {
    for (int i = 1; i < n; i++) {
        if (comparar_por_criterio(&v[i - 1], &v[i], campo, dir) > 0) {
            return 0;
        }
    }
    return 1;
}

const char *nome_campo(Campo campo) {
    return campo == CAMPO_ID ? "Identificador" : "Pontuacao";
}

const char *nome_direcao(Direcao dir) {
    return dir == CRESCENTE ? "Crescente" : "Decrescente";
}

int intercalar(const int a[], int na, const int b[], int nb, int saida[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    int comparacoes = 0;

    while (i < na && j < nb) {
        comparacoes++;
        if (a[i] <= b[j]) {
            saida[k++] = a[i++];
        } else {
            saida[k++] = b[j++];
        }
    }

    while (i < na) {
        saida[k++] = a[i++];
    }

    while (j < nb) {
        saida[k++] = b[j++];
    }

    return comparacoes;
}

void dividir_intervalo(int v[], int inicio, int fim, int nivel, long *divisoes) {
    printf("%*s[%d..%d] ->", nivel * 2, "", inicio, fim);
    for (int i = inicio; i <= fim; i++) {
        printf(" %d", v[i]);
    }

    if (inicio >= fim) {
        // RESOLVER: caso base, intervalo com um unico elemento ja esta ordenado
        printf("  (caso base)\n");
        return;
    }

    int meio = inicio + (fim - inicio) / 2;

    // DIVIDIR: separa o intervalo em duas metades [inicio..meio] e [meio+1..fim]
    (*divisoes)++;
    printf("  (meio = %d)\n", meio);

    dividir_intervalo(v, inicio, meio, nivel + 1, divisoes);
    dividir_intervalo(v, meio + 1, fim, nivel + 1, divisoes);

    // COMBINAR: as duas metades resolvidas sao intercaladas (ver Merge Sort)
}

int jogadores_iguais(const Jogador a[], const Jogador b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i].id != b[i].id ||
            strcmp(a[i].nome, b[i].nome) != 0 ||
            a[i].pontuacao != b[i].pontuacao) {
            return 0;
        }
    }
    return 1;
}

int comparar_inteiros_ctx(const void *a, const void *b, const void *ctx) {
    (void)ctx;
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int comparar_jogadores_ctx(const void *a, const void *b, const void *ctx) {
    const CriterioJogador *crit = ctx;
    return comparar_por_criterio((const Jogador *)a, (const Jogador *)b, crit->campo, crit->dir);
}

void intercalar_generico(char *base, char *aux, int inicio, int meio, int fim,
                         size_t tam, FuncComparacao cmp, const void *ctx, MetricasMerge *m) {
    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        m->comparacoes++;
        if (cmp(base + (size_t)i * tam, base + (size_t)j * tam, ctx) <= 0) {
            memcpy(aux + (size_t)k * tam, base + (size_t)i * tam, tam);
            i++;
        } else {
            memcpy(aux + (size_t)k * tam, base + (size_t)j * tam, tam);
            j++;
        }
        k++;
        m->copias++;
    }

    while (i <= meio) {
        memcpy(aux + (size_t)k * tam, base + (size_t)i * tam, tam);
        i++;
        k++;
        m->copias++;
    }

    while (j <= fim) {
        memcpy(aux + (size_t)k * tam, base + (size_t)j * tam, tam);
        j++;
        k++;
        m->copias++;
    }

    memcpy(base + (size_t)inicio * tam, aux + (size_t)inicio * tam, (size_t)(fim - inicio + 1) * tam);
    m->copias += fim - inicio + 1;
}

void merge_sort_rec(char *base, char *aux, int inicio, int fim,
                    size_t tam, FuncComparacao cmp, const void *ctx, MetricasMerge *m) {
    if (inicio >= fim) {
        return;
    }

    int meio = inicio + (fim - inicio) / 2;

    merge_sort_rec(base, aux, inicio, meio, tam, cmp, ctx, m);
    merge_sort_rec(base, aux, meio + 1, fim, tam, cmp, ctx, m);
    intercalar_generico(base, aux, inicio, meio, fim, tam, cmp, ctx, m);
}

int merge_sort_generico(void *base, int n, size_t tam, FuncComparacao cmp, const void *ctx, MetricasMerge *m) {
    if (n <= 1) {
        return 1;
    }

    char *aux = malloc((size_t)n * tam);
    if (aux == NULL) {
        return 0;
    }

    merge_sort_rec((char *)base, aux, 0, n - 1, tam, cmp, ctx, m);

    free(aux);
    return 1;
}

int merge_sort_inteiros(int v[], int n, MetricasMerge *m) {
    return merge_sort_generico(v, n, sizeof(int), comparar_inteiros_ctx, NULL, m);
}

int merge_sort_jogadores(Jogador v[], int n, Campo campo, Direcao dir, MetricasMerge *m) {
    CriterioJogador crit = {campo, dir};
    return merge_sort_generico(v, n, sizeof(Jogador), comparar_jogadores_ctx, &crit, m);
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

void executar_insertion_jogadores(void) {
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

    printf("Ordem por nome e desempate por id: %s\n", jogadores_ordenados(copia, n) ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", jogadores_preservados(original, copia, n) ? "OK" : "FALHOU");

    free(copia);
}

void testar_bubble(const char *titulo, int entrada[], int n) {
    int *copia = malloc(n * sizeof(int));
    if (copia == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(copia, entrada, n * sizeof(int));

    MetricasBubble m = {0, 0, 0};

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Antes", copia, n);
    bubble_sort(copia, n, &m);
    imprimir_numeros("Depois", copia, n);

    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Passagens: %ld\n", m.passagens);

    if (numeros_ordenados(copia, n) && mesmo_conjunto(entrada, copia, n)) {
        printf("Validacao: OK\n");
    } else {
        printf("Validacao: FALHOU\n");
    }
    printf("\n");

    free(copia);
}

void executar_bubble(void) {
    int ordenado[TAM_TESTE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[TAM_TESTE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int aleatorio[TAM_TESTE] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};

    testar_bubble("Vetor ja ordenado", ordenado, TAM_TESTE);
    testar_bubble("Vetor em ordem inversa", inverso, TAM_TESTE);
    testar_bubble("Vetor aleatorio", aleatorio, TAM_TESTE);
}

void executar_bubble_jogadores(Campo campo, Direcao dir) {
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

    MetricasBubble m = {0, 0, 0};

    printf("Criterio: %s | Direcao: %s\n\n", nome_campo(campo), nome_direcao(dir));

    imprimir_jogadores("Antes da ordenacao:", copia, n);
    bubble_sort_jogadores(copia, n, campo, dir, &m);
    imprimir_jogadores("Depois da ordenacao:", copia, n);

    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Passagens: %ld\n", m.passagens);

    int ordem_ok = jogadores_ordenados_por(copia, n, campo, dir);
    int assoc_ok = jogadores_preservados(original, copia, n);

    printf("Ordem final por %s (%s): %s\n", nome_campo(campo), nome_direcao(dir), ordem_ok ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", assoc_ok ? "OK" : "FALHOU");

    free(copia);
}

void menu_bubble_jogadores(void) {
    int opcao;

    do {
        printf("\n--- BUBBLE SORT DE JOGADORES ---\n");
        printf("1 - Campo: Identificador | Direcao: Crescente\n");
        printf("2 - Campo: Identificador | Direcao: Decrescente\n");
        printf("3 - Campo: Pontuacao     | Direcao: Crescente\n");
        printf("4 - Campo: Pontuacao     | Direcao: Decrescente\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida\n");
            exit(1);
        }
        printf("\n");

        switch (opcao) {
            case 1:
                executar_bubble_jogadores(CAMPO_ID, CRESCENTE);
                break;
            case 2:
                executar_bubble_jogadores(CAMPO_ID, DECRESCENTE);
                break;
            case 3:
                executar_bubble_jogadores(CAMPO_PONTUACAO, CRESCENTE);
                break;
            case 4:
                executar_bubble_jogadores(CAMPO_PONTUACAO, DECRESCENTE);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (opcao != 0);
}

void testar_intercalacao(const char *titulo, int a[], int na, int b[], int nb) {
    int total = na + nb;
    int *saida = malloc((total > 0 ? total : 1) * sizeof(int));
    int *uniao = malloc((total > 0 ? total : 1) * sizeof(int));

    if (saida == NULL || uniao == NULL) {
        printf("Erro de alocacao\n");
        free(saida);
        free(uniao);
        return;
    }

    memcpy(uniao, a, na * sizeof(int));
    memcpy(uniao + na, b, nb * sizeof(int));

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Vetor A", a, na);
    imprimir_numeros("Vetor B", b, nb);

    int comparacoes = intercalar(a, na, b, nb, saida);

    imprimir_numeros("Intercalado", saida, total);
    printf("Comparacoes: %d\n", comparacoes);

    if (numeros_ordenados(saida, total) && mesmo_conjunto(uniao, saida, total)) {
        printf("Validacao: OK (%d valores, ordem correta)\n", total);
    } else {
        printf("Validacao: FALHOU\n");
    }
    printf("\n");

    free(saida);
    free(uniao);
}

void executar_intercalacao(void) {
    int a1[] = {1, 4, 7, 10, 15};
    int b1[] = {2, 4, 6, 8, 9, 12, 20};

    int a2[] = {1, 2, 3};
    int b2[] = {10, 20, 30, 40};

    int a3[] = {5, 10, 15};
    int b3[1] = {0};

    testar_intercalacao("Entradas alternadas com valor repetido", a1, 5, b1, 7);
    testar_intercalacao("Todos de A menores que os de B", a2, 3, b2, 4);
    testar_intercalacao("Uma entrada vazia", a3, 3, b3, 0);

    int v[] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int n = sizeof(v) / sizeof(v[0]);
    long divisoes = 0;

    printf("--- Divisao recursiva do intervalo ---\n");
    dividir_intervalo(v, 0, n - 1, 0, &divisoes);
    printf("Total de divisoes: %ld\n", divisoes);
}

void testar_merge_inteiros(const char *titulo, int entrada[], int n) {
    int tam = n > 0 ? n : 1;
    int *copia = malloc(tam * sizeof(int));
    int *ref = malloc(tam * sizeof(int));

    if (copia == NULL || ref == NULL) {
        printf("Erro de alocacao\n");
        free(copia);
        free(ref);
        return;
    }

    memcpy(copia, entrada, n * sizeof(int));
    memcpy(ref, entrada, n * sizeof(int));

    MetricasMerge m = {0, 0};
    Contadores c = {0, 0};

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Antes", copia, n);

    if (!merge_sort_inteiros(copia, n, &m)) {
        printf("Erro de alocacao no vetor auxiliar\n");
        free(copia);
        free(ref);
        return;
    }

    insertion_sort(ref, n, &c);

    imprimir_numeros("Depois", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Copias: %ld\n", m.copias);

    int ordem_ok = numeros_ordenados(copia, n);
    int conjunto_ok = n == 0 || mesmo_conjunto(entrada, copia, n);
    int ref_ok = n == 0 || memcmp(ref, copia, n * sizeof(int)) == 0;

    printf("Ordem final: %s\n", ordem_ok ? "OK" : "FALHOU");
    printf("Mesmos valores da entrada (com repetidos): %s\n", conjunto_ok ? "OK" : "FALHOU");
    printf("Igual ao Insertion Sort: %s\n", ref_ok ? "OK" : "FALHOU");
    printf("\n");

    free(copia);
    free(ref);
}

void testar_merge_jogadores(Campo campo, Direcao dir) {
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
    Jogador *ref = malloc(n * sizeof(Jogador));

    if (copia == NULL || ref == NULL) {
        printf("Erro de alocacao\n");
        free(copia);
        free(ref);
        return;
    }

    memcpy(copia, original, n * sizeof(Jogador));
    memcpy(ref, original, n * sizeof(Jogador));

    MetricasMerge m = {0, 0};
    MetricasBubble mb = {0, 0, 0};

    printf("--- Criterio: %s | Direcao: %s ---\n", nome_campo(campo), nome_direcao(dir));
    imprimir_jogadores("Antes da ordenacao:", copia, n);

    if (!merge_sort_jogadores(copia, n, campo, dir, &m)) {
        printf("Erro de alocacao no vetor auxiliar\n");
        free(copia);
        free(ref);
        return;
    }

    bubble_sort_jogadores(ref, n, campo, dir, &mb);

    imprimir_jogadores("Depois da ordenacao:", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Copias: %ld\n", m.copias);

    printf("Ordem final: %s\n", jogadores_ordenados_por(copia, n, campo, dir) ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", jogadores_preservados(original, copia, n) ? "OK" : "FALHOU");
    printf("Igual ao Bubble Sort: %s\n", jogadores_iguais(ref, copia, n) ? "OK" : "FALHOU");
    printf("\n");

    free(copia);
    free(ref);
}

void executar_merge(void) {
    int aleatorio[TAM_TESTE] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int repetidos[8] = {5, 3, 5, 1, 3, 5, 1, 1};
    int ordenado[TAM_TESTE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[TAM_TESTE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int unico[1] = {7};
    int vazio[1] = {0};

    printf("===== MERGE SORT: NUMEROS =====\n\n");
    testar_merge_inteiros("Vetor aleatorio", aleatorio, TAM_TESTE);
    testar_merge_inteiros("Vetor com valores repetidos", repetidos, 8);
    testar_merge_inteiros("Vetor ja ordenado", ordenado, TAM_TESTE);
    testar_merge_inteiros("Vetor em ordem inversa", inverso, TAM_TESTE);
    testar_merge_inteiros("Um elemento", unico, 1);
    testar_merge_inteiros("Vetor vazio", vazio, 0);

    printf("===== MERGE SORT: JOGADORES =====\n\n");
    testar_merge_jogadores(CAMPO_ID, CRESCENTE);
    testar_merge_jogadores(CAMPO_ID, DECRESCENTE);
    testar_merge_jogadores(CAMPO_PONTUACAO, CRESCENTE);
    testar_merge_jogadores(CAMPO_PONTUACAO, DECRESCENTE);
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
        printf("4 - Insertion Sort (jogadores por nome)\n");
        printf("5 - Bubble Sort com parada antecipada\n");
        printf("6 - Bubble Sort (jogadores por campo e direcao)\n");
        printf("7 - Intercalacao e divisao recursiva\n");
        printf("8 - Merge Sort (numeros e jogadores)\n");
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
            case 4:
                executar_insertion_jogadores();
                break;
            case 5:
                executar_bubble();
                break;
            case 6:
                menu_bubble_jogadores();
                break;
            case 7:
                executar_intercalacao();
                break;
            case 8:
                executar_merge();
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