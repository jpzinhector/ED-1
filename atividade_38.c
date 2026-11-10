#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30
#define TAM_TESTE 10

typedef struct {
    long comparacoes;
    long deslocamentos;
} Contadores;

typedef struct {
    long comparacoes;
    long trocas;
    long passagens;
} MetricasBubble;

typedef struct {
    long comparacoes;
    long copias;
} MetricasMerge;

typedef struct {
    int id;
    char nome[TAM_NOME];
    int pontuacao;
} Jogador;

typedef enum {
    CAMPO_ID,
    CAMPO_PONTUACAO
} Campo;

typedef enum {
    CRESCENTE,
    DECRESCENTE
} Direcao;

typedef struct {
    Campo campo;
    Direcao dir;
} CriterioJogador;

typedef int (*FuncComparacao)(const void *, const void *, const void *);

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

int comparar_jogadores(const Jogador *a, const Jogador *b) {
    int r = strcmp(a->nome, b->nome);
    if (r != 0) {
        return r;
    }
    return (a->id > b->id) - (a->id < b->id);
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

int jogadores_ordenados(Jogador v[], int n) {
    for (int i = 1; i < n; i++) {
        if (comparar_jogadores(&v[i - 1], &v[i]) > 0) {
            return 0;
        }
    }
    return 1;
}

int jogadores_ordenados_por(Jogador v[], int n, Campo campo, Direcao dir) {
    for (int i = 1; i < n; i++) {
        if (comparar_por_criterio(&v[i - 1], &v[i], campo, dir) > 0) {
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

int jogadores_iguais(Jogador a[], Jogador b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i].id != b[i].id ||
            strcmp(a[i].nome, b[i].nome) != 0 ||
            a[i].pontuacao != b[i].pontuacao) {
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

const char *nome_campo(Campo campo) {
    return campo == CAMPO_ID ? "Identificador" : "Pontuacao";
}

const char *nome_direcao(Direcao dir) {
    return dir == CRESCENTE ? "Crescente" : "Decrescente";
}

int intercalar(int a[], int na, int b[], int nb, int saida[]) {
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

    // COMBINAR: aqui as duas metades ja resolvidas serao intercaladas (proxima atividade)
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

void testar_intercalacao(const char *titulo, int a[], int na, int b[], int nb) {
    int total = na + nb;
    int *saida = malloc(total * sizeof(int));
    int *uniao = malloc(total * sizeof(int));

    if (saida == NULL || uniao == NULL) {
        printf("Erro de alocacao\n");
        free(saida);
        free(uniao);
        return;
    }

    memcpy(uniao, a, na * sizeof(int));
    memcpy(uniao + na, b, nb * sizeof(int));

    printf("--- %s ---\n", titulo);
    imprimir("Vetor A", a, na);
    imprimir("Vetor B", b, nb);

    int comparacoes = intercalar(a, na, b, nb, saida);

    imprimir("Intercalado", saida, total);
    printf("Comparacoes: %d\n", comparacoes);

    if (esta_ordenado(saida, total) && mesmo_conjunto(uniao, saida, total)) {
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
    int b3[] = {};

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

void testar_bubble(const char *titulo, int entrada[], int n) {
    int *copia = malloc(n * sizeof(int));
    if (copia == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(copia, entrada, n * sizeof(int));

    MetricasBubble m = {0, 0, 0};

    printf("--- %s ---\n", titulo);
    imprimir("Antes", copia, n);
    bubble_sort(copia, n, &m);
    imprimir("Depois", copia, n);

    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Passagens: %ld\n", m.passagens);

    if (esta_ordenado(copia, n) && mesmo_conjunto(entrada, copia, n)) {
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
    int assoc_ok = associacao_preservada(original, copia, n);

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
    imprimir("Antes", copia, n);

    if (!merge_sort_inteiros(copia, n, &m)) {
        printf("Erro de alocacao no vetor auxiliar\n");
        free(copia);
        free(ref);
        return;
    }

    insertion_sort(ref, n, &c);

    imprimir("Depois", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Copias: %ld\n", m.copias);

    int ordem_ok = esta_ordenado(copia, n);
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

    int ordem_ok = jogadores_ordenados_por(copia, n, campo, dir);
    int assoc_ok = associacao_preservada(original, copia, n);
    int ref_ok = jogadores_iguais(ref, copia, n);

    printf("Ordem final: %s\n", ordem_ok ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", assoc_ok ? "OK" : "FALHOU");
    printf("Igual ao Bubble Sort: %s\n", ref_ok ? "OK" : "FALHOU");
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
    int opcao;

    do {
        printf("\n===== CENTRAL =====\n");
        printf("1 - Insertion Sort (vetor de inteiros)\n");
        printf("2 - Insertion Sort (jogadores por nome)\n");
        printf("3 - Bubble Sort com parada antecipada\n");
        printf("4 - Bubble Sort (jogadores por campo e direcao)\n");
        printf("5 - Intercalacao e divisao recursiva\n");
        printf("6 - Merge Sort (numeros e jogadores)\n");
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
            case 3:
                executar_bubble();
                break;
            case 4:
                menu_bubble_jogadores();
                break;
            case 5:
                executar_intercalacao();
                break;
            case 6:
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