#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_NOME 30
#define N_NUMEROS 10
#define N_JOGADORES 8
#define TAM_TESTE 10
#define MAX_DADOS 5000
#define NUM_ALGORITMOS 5

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
    CAMPO_PONTUACAO,
    CAMPO_NOME
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

typedef struct {
    long comparacoes;
    long trocas;
} MetricasQuick;

typedef struct {
    long comparacoes;
    long trocas;
    long deslocamentos;
    double tempo_ms;
    int valido;
} Resultado;

typedef struct {
    int tipo;
    int n;
    int *ints;
    Jogador *jogs;
    Campo campo;
    Direcao dir;
} Dados;

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
    } else if (campo == CAMPO_NOME) {
        int s = strcmp(a->nome, b->nome);
        r = (s > 0) - (s < 0);
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
    switch (campo) {
        case CAMPO_ID:
            return "Identificador";
        case CAMPO_NOME:
            return "Nome";
        default:
            return "Pontuacao";
    }
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

int mesmos_elementos(const int a[], const int b[], int n) {
    int tam = n > 0 ? n : 1;
    int *ca = malloc(tam * sizeof(int));
    int *cb = malloc(tam * sizeof(int));
    int ok = 0;

    if (ca != NULL && cb != NULL) {
        memcpy(ca, a, n * sizeof(int));
        memcpy(cb, b, n * sizeof(int));
        qsort(ca, n, sizeof(int), comparar_int);
        qsort(cb, n, sizeof(int), comparar_int);
        ok = memcmp(ca, cb, n * sizeof(int)) == 0;
    }

    free(ca);
    free(cb);
    return ok;
}

int particao_auxiliar(int v[], int inicio, int fim, MetricasQuick *m) {
    int tam = fim - inicio + 1;
    int *aux = malloc(tam * sizeof(int));
    if (aux == NULL) {
        return -1;
    }

    int pivo = v[fim];
    int esq = 0;
    int dir = tam - 1;

    for (int i = inicio; i < fim; i++) {
        m->comparacoes++;
        if (v[i] < pivo) {
            aux[esq++] = v[i];
        } else {
            aux[dir--] = v[i];
        }
    }

    aux[esq] = pivo;

    memcpy(v + inicio, aux, tam * sizeof(int));
    free(aux);

    return inicio + esq;
}

int particao_inplace(int v[], int inicio, int fim, MetricasQuick *m) {
    int pivo = v[fim];
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        m->comparacoes++;
        if (v[j] < pivo) {
            if (i != j) {
                int aux = v[i];
                v[i] = v[j];
                v[j] = aux;
                m->trocas++;
            }
            i++;
        }
    }

    if (i != fim) {
        int aux = v[i];
        v[i] = v[fim];
        v[fim] = aux;
        m->trocas++;
    }

    return i;
}

void quick_sort_rec(int v[], int inicio, int fim, MetricasQuick *m) {
    if (inicio >= fim) {
        return;
    }

    int p = particao_inplace(v, inicio, fim, m);

    if (p - 1 > inicio) {
        quick_sort_rec(v, inicio, p - 1, m);
    }
    if (p + 1 < fim) {
        quick_sort_rec(v, p + 1, fim, m);
    }
}

void quick_sort(int v[], int n, MetricasQuick *m) {
    if (n > 1) {
        quick_sort_rec(v, 0, n - 1, m);
    }
}

int particao_valida(const int v[], int inicio, int fim, int p, int pivo) {
    if (p < inicio || p > fim || v[p] != pivo) {
        return 0;
    }
    for (int i = inicio; i < p; i++) {
        if (v[i] >= pivo) {
            return 0;
        }
    }
    for (int i = p + 1; i <= fim; i++) {
        if (v[i] < pivo) {
            return 0;
        }
    }
    return 1;
}

static const char *NOMES_ALGORITMOS[NUM_ALGORITMOS] = {
    "Selection Sort",
    "Insertion Sort",
    "Bubble Sort",
    "Merge Sort",
    "Quick Sort aleatorio"
};

void escolher_pivo_aleatorio(int v[], int inicio, int fim, MetricasQuick *m) {
    int p = inicio + rand() % (fim - inicio + 1);

    if (p != fim) {
        int aux = v[p];
        v[p] = v[fim];
        v[fim] = aux;
        m->trocas++;
    }
}

void quick_sort_aleatorio_rec(int v[], int inicio, int fim, MetricasQuick *m) {
    if (inicio >= fim) {
        return;
    }

    escolher_pivo_aleatorio(v, inicio, fim, m);
    int p = particao_inplace(v, inicio, fim, m);

    if (p - 1 > inicio) {
        quick_sort_aleatorio_rec(v, inicio, p - 1, m);
    }
    if (p + 1 < fim) {
        quick_sort_aleatorio_rec(v, p + 1, fim, m);
    }
}

void quick_sort_aleatorio(int v[], int n, MetricasQuick *m) {
    if (n > 1) {
        quick_sort_aleatorio_rec(v, 0, n - 1, m);
    }
}

int particao_inplace_jogadores(Jogador v[], int inicio, int fim, Campo campo, Direcao dir, MetricasQuick *m) {
    Jogador pivo = v[fim];
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        m->comparacoes++;
        if (comparar_por_criterio(&v[j], &pivo, campo, dir) < 0) {
            if (i != j) {
                trocar_jogadores(&v[i], &v[j]);
                m->trocas++;
            }
            i++;
        }
    }

    if (i != fim) {
        trocar_jogadores(&v[i], &v[fim]);
        m->trocas++;
    }

    return i;
}

void escolher_pivo_aleatorio_jogadores(Jogador v[], int inicio, int fim, MetricasQuick *m) {
    int p = inicio + rand() % (fim - inicio + 1);

    if (p != fim) {
        trocar_jogadores(&v[p], &v[fim]);
        m->trocas++;
    }
}

void quick_sort_jogadores_rec(Jogador v[], int inicio, int fim, Campo campo, Direcao dir, MetricasQuick *m) {
    if (inicio >= fim) {
        return;
    }

    escolher_pivo_aleatorio_jogadores(v, inicio, fim, m);
    int p = particao_inplace_jogadores(v, inicio, fim, campo, dir, m);

    if (p - 1 > inicio) {
        quick_sort_jogadores_rec(v, inicio, p - 1, campo, dir, m);
    }
    if (p + 1 < fim) {
        quick_sort_jogadores_rec(v, p + 1, fim, campo, dir, m);
    }
}

void quick_sort_jogadores(Jogador v[], int n, Campo campo, Direcao dir, MetricasQuick *m) {
    if (n > 1) {
        quick_sort_jogadores_rec(v, 0, n - 1, campo, dir, m);
    }
}

void insertion_sort_jogadores_criterio(Jogador v[], int n, Campo campo, Direcao dir, Contadores *c) {
    for (int i = 1; i < n; i++) {
        Jogador chave = v[i];
        int j = i - 1;

        while (j >= 0) {
            c->comparacoes++;
            if (comparar_por_criterio(&v[j], &chave, campo, dir) > 0) {
                v[j + 1] = v[j];
                c->deslocamentos++;
                j--;
            } else {
                break;
            }
        }

        v[j + 1] = chave;
    }
}

void selection_sort_jogadores_criterio(Jogador v[], int n, Campo campo, Direcao dir, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int melhor = i;

        for (int j = i + 1; j < n; j++) {
            m->comparacoes++;
            if (comparar_por_criterio(&v[j], &v[melhor], campo, dir) < 0) {
                melhor = j;
            }
        }

        if (melhor != i) {
            trocar_jogadores(&v[i], &v[melhor]);
            m->trocas++;
        }
    }
}

int ler_opcao(void) {
    int x;
    int r = scanf("%d", &x);

    if (r == EOF) {
        return 0;
    }
    if (r != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        return -1;
    }
    return x;
}

void liberar_dados(Dados *d) {
    free(d->ints);
    free(d->jogs);
    d->ints = NULL;
    d->jogs = NULL;
    d->n = 0;
    d->tipo = 0;
}

int ler_quantidade(void) {
    printf("Quantidade de elementos (1 a %d): ", MAX_DADOS);
    int n = ler_opcao();
    if (n < 1 || n > MAX_DADOS) {
        printf("Quantidade invalida\n");
        return 0;
    }
    return n;
}

void gerar_inteiros(Dados *d) {
    int n = ler_quantidade();
    if (n == 0) {
        return;
    }

    int *v = malloc(n * sizeof(int));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        v[i] = rand() % 1000;
    }

    liberar_dados(d);
    d->ints = v;
    d->n = n;
    d->tipo = 1;
    printf("%d inteiros gerados\n", n);
}

void gerar_jogadores(Dados *d) {
    static const char *nomes[] = {
        "Ana", "Bruno", "Carlos", "Daniela", "Eduardo", "Fernanda",
        "Gabriel", "Helena", "Igor", "Julia", "Lucas", "Marcos"
    };
    int qtd_nomes = sizeof(nomes) / sizeof(nomes[0]);

    int n = ler_quantidade();
    if (n == 0) {
        return;
    }

    Jogador *v = malloc(n * sizeof(Jogador));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        v[i].id = i + 1;
        snprintf(v[i].nome, TAM_NOME, "%s", nomes[rand() % qtd_nomes]);
        v[i].pontuacao = rand() % 1000;
    }

    liberar_dados(d);
    d->jogs = v;
    d->n = n;
    d->tipo = 2;
    printf("%d jogadores gerados\n", n);
}

void carregar_exemplo_inteiros(Dados *d) {
    int exemplo[] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12, 17, 42};
    int n = sizeof(exemplo) / sizeof(exemplo[0]);

    int *v = malloc(n * sizeof(int));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(v, exemplo, n * sizeof(int));

    liberar_dados(d);
    d->ints = v;
    d->n = n;
    d->tipo = 1;
    printf("Exemplo de %d inteiros carregado\n", n);
}

void carregar_exemplo_jogadores(Dados *d) {
    Jogador exemplo[] = {
        {101, "Marcos", 850},
        {102, "Ana", 920},
        {103, "Carlos", 730},
        {104, "Ana", 810},
        {105, "Beatriz", 990},
        {106, "Marcos", 640},
        {107, "Ana", 920},
        {108, "Daniel", 505}
    };
    int n = sizeof(exemplo) / sizeof(exemplo[0]);

    Jogador *v = malloc(n * sizeof(Jogador));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    memcpy(v, exemplo, n * sizeof(Jogador));

    liberar_dados(d);
    d->jogs = v;
    d->n = n;
    d->tipo = 2;
    printf("Exemplo de %d jogadores carregado\n", n);
}

void carregar_arquivo_inteiros(Dados *d) {
    char caminho[256];

    printf("Arquivo (numeros separados por espaco ou linha): ");
    if (scanf("%255s", caminho) != 1) {
        return;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        return;
    }

    int *v = malloc(MAX_DADOS * sizeof(int));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        fclose(f);
        return;
    }

    int n = 0;
    while (n < MAX_DADOS && fscanf(f, "%d", &v[n]) == 1) {
        n++;
    }
    fclose(f);

    if (n == 0) {
        printf("Nenhum numero valido no arquivo\n");
        free(v);
        return;
    }

    liberar_dados(d);
    d->ints = v;
    d->n = n;
    d->tipo = 1;
    printf("%d inteiros carregados\n", n);
}

void carregar_arquivo_jogadores(Dados *d) {
    char caminho[256];

    printf("Arquivo (uma linha por jogador: id nome pontuacao): ");
    if (scanf("%255s", caminho) != 1) {
        return;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        return;
    }

    Jogador *v = malloc(MAX_DADOS * sizeof(Jogador));
    if (v == NULL) {
        printf("Erro de alocacao\n");
        fclose(f);
        return;
    }

    int n = 0;
    while (n < MAX_DADOS &&
           fscanf(f, "%d %29s %d", &v[n].id, v[n].nome, &v[n].pontuacao) == 3) {
        n++;
    }
    fclose(f);

    if (n == 0) {
        printf("Nenhum jogador valido no arquivo\n");
        free(v);
        return;
    }

    liberar_dados(d);
    d->jogs = v;
    d->n = n;
    d->tipo = 2;
    printf("%d jogadores carregados\n", n);
}

void escolher_criterio(Dados *d) {
    printf("Campo: 1 - Identificador | 2 - Nome | 3 - Pontuacao\n");
    printf("Opcao: ");
    int c = ler_opcao();

    printf("Direcao: 1 - Crescente | 2 - Decrescente\n");
    printf("Opcao: ");
    int s = ler_opcao();

    if (c < 1 || c > 3 || s < 1 || s > 2) {
        printf("Opcao invalida, criterio mantido\n");
        return;
    }

    d->campo = c == 1 ? CAMPO_ID : (c == 2 ? CAMPO_NOME : CAMPO_PONTUACAO);
    d->dir = s == 1 ? CRESCENTE : DECRESCENTE;
    printf("Criterio: %s | Direcao: %s\n", nome_campo(d->campo), nome_direcao(d->dir));
}

void mostrar_estado(const Dados *d) {
    if (d->tipo == 0) {
        printf("Dados atuais: nenhum\n");
    } else if (d->tipo == 1) {
        printf("Dados atuais: %d inteiros\n", d->n);
    } else {
        printf("Dados atuais: %d jogadores | Criterio: %s | Direcao: %s\n",
               d->n, nome_campo(d->campo), nome_direcao(d->dir));
    }
}

void mostrar_dados(const Dados *d) {
    if (d->tipo == 0) {
        printf("Nenhum dado carregado\n");
        return;
    }

    int mostrar = d->n > 20 ? 10 : d->n;

    if (d->tipo == 1) {
        imprimir_numeros("Dados", d->ints, mostrar);
    } else {
        imprimir_jogadores("Dados:", d->jogs, mostrar);
    }

    if (mostrar < d->n) {
        printf("(mostrando os %d primeiros de %d)\n", mostrar, d->n);
    }
}

double tempo_desde(clock_t ini) {
    return (double)(clock() - ini) * 1000.0 / CLOCKS_PER_SEC;
}

int rodar_inteiros(int alg, const int original[], int copia[], int n, Resultado *r) {
    memset(r, 0, sizeof(*r));
    int ok = 1;
    clock_t ini = clock();

    if (alg == 0) {
        Metricas m = {0, 0};
        selection_sort(copia, n, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    } else if (alg == 1) {
        Contadores c = {0, 0};
        insertion_sort(copia, n, &c);
        r->comparacoes = c.comparacoes;
        r->deslocamentos = c.deslocamentos;
    } else if (alg == 2) {
        MetricasBubble m = {0, 0, 0};
        bubble_sort(copia, n, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    } else if (alg == 3) {
        MetricasMerge m = {0, 0};
        ok = merge_sort_inteiros(copia, n, &m);
        r->comparacoes = m.comparacoes;
        r->deslocamentos = m.copias;
    } else {
        MetricasQuick m = {0, 0};
        quick_sort_aleatorio(copia, n, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    }

    r->tempo_ms = tempo_desde(ini);
    r->valido = ok && numeros_ordenados(copia, n) && mesmos_elementos(original, copia, n);
    return ok;
}

int rodar_jogadores(int alg, const Jogador original[], Jogador copia[], int n, Campo campo, Direcao dir, Resultado *r) {
    memset(r, 0, sizeof(*r));
    int ok = 1;
    clock_t ini = clock();

    if (alg == 0) {
        Metricas m = {0, 0};
        selection_sort_jogadores_criterio(copia, n, campo, dir, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    } else if (alg == 1) {
        Contadores c = {0, 0};
        insertion_sort_jogadores_criterio(copia, n, campo, dir, &c);
        r->comparacoes = c.comparacoes;
        r->deslocamentos = c.deslocamentos;
    } else if (alg == 2) {
        MetricasBubble m = {0, 0, 0};
        bubble_sort_jogadores(copia, n, campo, dir, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    } else if (alg == 3) {
        MetricasMerge m = {0, 0};
        ok = merge_sort_jogadores(copia, n, campo, dir, &m);
        r->comparacoes = m.comparacoes;
        r->deslocamentos = m.copias;
    } else {
        MetricasQuick m = {0, 0};
        quick_sort_jogadores(copia, n, campo, dir, &m);
        r->comparacoes = m.comparacoes;
        r->trocas = m.trocas;
    }

    r->tempo_ms = tempo_desde(ini);
    r->valido = ok && jogadores_ordenados_por(copia, n, campo, dir) && jogadores_preservados(original, copia, n);
    return ok;
}

int executar_algoritmo_nos_dados(const Dados *d, int alg, Resultado *r, int mostrar_resultado) {
    int ok = 0;

    if (d->tipo == 1) {
        int *copia = malloc(d->n * sizeof(int));
        if (copia == NULL) {
            printf("Erro de alocacao\n");
            return 0;
        }
        memcpy(copia, d->ints, d->n * sizeof(int));

        ok = rodar_inteiros(alg, d->ints, copia, d->n, r);

        if (mostrar_resultado && d->n <= 20) {
            imprimir_numeros("Antes", d->ints, d->n);
            imprimir_numeros("Depois", copia, d->n);
        }
        free(copia);
    } else {
        Jogador *copia = malloc(d->n * sizeof(Jogador));
        if (copia == NULL) {
            printf("Erro de alocacao\n");
            return 0;
        }
        memcpy(copia, d->jogs, d->n * sizeof(Jogador));

        ok = rodar_jogadores(alg, d->jogs, copia, d->n, d->campo, d->dir, r);

        if (mostrar_resultado && d->n <= 20) {
            imprimir_jogadores("Antes:", d->jogs, d->n);
            imprimir_jogadores("Depois:", copia, d->n);
        }
        free(copia);
    }

    if (!ok) {
        printf("Erro de alocacao no vetor auxiliar\n");
    }
    return ok;
}

void executar_um_algoritmo(const Dados *d) {
    if (d->tipo == 0) {
        printf("Gere ou carregue dados primeiro\n");
        return;
    }

    printf("Algoritmo:\n");
    for (int i = 0; i < NUM_ALGORITMOS; i++) {
        printf("%d - %s\n", i + 1, NOMES_ALGORITMOS[i]);
    }
    printf("Opcao: ");
    int alg = ler_opcao();

    if (alg < 1 || alg > NUM_ALGORITMOS) {
        printf("Opcao invalida\n");
        return;
    }

    Resultado r;
    printf("\n--- %s ---\n", NOMES_ALGORITMOS[alg - 1]);
    mostrar_estado(d);

    if (!executar_algoritmo_nos_dados(d, alg - 1, &r, 1)) {
        return;
    }

    printf("Comparacoes: %ld\n", r.comparacoes);
    printf("Trocas: %ld\n", r.trocas);
    printf("Deslocamentos/copias: %ld\n", r.deslocamentos);
    printf("Tempo: %.3f ms\n", r.tempo_ms);
    printf("Validacao: %s\n", r.valido ? "OK" : "FALHOU");
}

void relatorio_comparativo(const Dados *d) {
    if (d->tipo == 0) {
        printf("Gere ou carregue dados primeiro\n");
        return;
    }

    Resultado resultados[NUM_ALGORITMOS];
    int executou[NUM_ALGORITMOS];

    for (int i = 0; i < NUM_ALGORITMOS; i++) {
        executou[i] = executar_algoritmo_nos_dados(d, i, &resultados[i], 0);
    }

    printf("\n===== RELATORIO COMPARATIVO =====\n");
    mostrar_estado(d);
    printf("Todos os algoritmos ordenaram uma copia da mesma entrada original\n\n");

    printf("%-22s %12s %10s %14s %10s  %s\n",
           "Algoritmo", "Comparacoes", "Trocas", "Deslocamentos", "Tempo(ms)", "Validacao");

    for (int i = 0; i < NUM_ALGORITMOS; i++) {
        if (!executou[i]) {
            printf("%-22s %s\n", NOMES_ALGORITMOS[i], "nao executou");
            continue;
        }
        printf("%-22s %12ld %10ld %14ld %10.3f  %s\n",
               NOMES_ALGORITMOS[i],
               resultados[i].comparacoes,
               resultados[i].trocas,
               resultados[i].deslocamentos,
               resultados[i].tempo_ms,
               resultados[i].valido ? "OK" : "FALHOU");
    }

    printf("\nDeslocamentos: movimentos do Insertion Sort e copias do Merge Sort\n");
    printf("Trocas: trocas do Selection, Bubble e Quick Sort\n\n");
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

void testar_particao(const char *titulo, int entrada[], int n) {
    int *copia_aux = malloc(n * sizeof(int));
    int *copia_in = malloc(n * sizeof(int));

    if (copia_aux == NULL || copia_in == NULL) {
        printf("Erro de alocacao\n");
        free(copia_aux);
        free(copia_in);
        return;
    }

    memcpy(copia_aux, entrada, n * sizeof(int));
    memcpy(copia_in, entrada, n * sizeof(int));

    int pivo = entrada[n - 1];
    MetricasQuick ma = {0, 0};
    MetricasQuick mi = {0, 0};

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Entrada", entrada, n);
    printf("Pivo (ultimo elemento): %d\n\n", pivo);

    int pa = particao_auxiliar(copia_aux, 0, n - 1, &ma);
    if (pa < 0) {
        printf("Erro de alocacao na particao auxiliar\n");
        free(copia_aux);
        free(copia_in);
        return;
    }

    int pi = particao_inplace(copia_in, 0, n - 1, &mi);

    imprimir_numeros("Particao com auxiliar", copia_aux, n);
    printf("Posicao final do pivo: %d\n", pa);
    printf("Comparacoes: %ld\n", ma.comparacoes);
    printf("Regra (< pivo | pivo | >= pivo): %s\n", particao_valida(copia_aux, 0, n - 1, pa, pivo) ? "OK" : "FALHOU");
    printf("Todos os elementos preservados: %s\n\n", mesmos_elementos(entrada, copia_aux, n) ? "OK" : "FALHOU");

    imprimir_numeros("Particao in-place", copia_in, n);
    printf("Posicao final do pivo: %d\n", pi);
    printf("Comparacoes: %ld\n", mi.comparacoes);
    printf("Trocas: %ld\n", mi.trocas);
    printf("Regra (< pivo | pivo | >= pivo): %s\n", particao_valida(copia_in, 0, n - 1, pi, pivo) ? "OK" : "FALHOU");
    printf("Todos os elementos preservados: %s\n", mesmos_elementos(entrada, copia_in, n) ? "OK" : "FALHOU");
    printf("Mesma posicao de pivo nas duas versoes: %s\n", pa == pi ? "OK" : "FALHOU");
    printf("\n");

    free(copia_aux);
    free(copia_in);
}

void testar_quick(const char *titulo, int entrada[], int n) {
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

    MetricasQuick m = {0, 0};
    Contadores c = {0, 0};

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Antes", copia, n);

    quick_sort(copia, n, &m);
    insertion_sort(ref, n, &c);

    imprimir_numeros("Depois", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);

    printf("Ordem final: %s\n", numeros_ordenados(copia, n) ? "OK" : "FALHOU");
    printf("Mesmos valores da entrada (com repetidos): %s\n", mesmos_elementos(entrada, copia, n) ? "OK" : "FALHOU");
    printf("Igual ao Insertion Sort: %s\n", n == 0 || memcmp(ref, copia, n * sizeof(int)) == 0 ? "OK" : "FALHOU");
    printf("\n");

    free(copia);
    free(ref);
}

void executar_quick(void) {
    int aleatorio[TAM_TESTE] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int repetidos[8] = {5, 3, 5, 1, 3, 5, 1, 3};
    int iguais[5] = {4, 4, 4, 4, 4};
    int ordenado[TAM_TESTE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[TAM_TESTE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int unico[1] = {7};
    int vazio[1] = {0};

    printf("===== PARTICAO ISOLADA =====\n\n");
    testar_particao("Vetor aleatorio", aleatorio, TAM_TESTE);
    testar_particao("Vetor com valores repetidos", repetidos, 8);
    testar_particao("Todos os valores iguais", iguais, 5);

    printf("===== QUICK SORT =====\n\n");
    testar_quick("Vetor aleatorio", aleatorio, TAM_TESTE);
    testar_quick("Vetor com valores repetidos", repetidos, 8);
    testar_quick("Todos os valores iguais", iguais, 5);
    testar_quick("Vetor ja ordenado", ordenado, TAM_TESTE);
    testar_quick("Vetor em ordem inversa", inverso, TAM_TESTE);
    testar_quick("Um elemento", unico, 1);
    testar_quick("Vetor vazio", vazio, 0);
}

void testar_quick_aleatorio(const char *titulo, int entrada[], int n) {
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

    MetricasQuick m = {0, 0};
    Contadores c = {0, 0};

    printf("--- %s ---\n", titulo);
    imprimir_numeros("Antes", copia, n);

    quick_sort_aleatorio(copia, n, &m);
    insertion_sort(ref, n, &c);

    imprimir_numeros("Depois", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);

    printf("Ordem final: %s\n", numeros_ordenados(copia, n) ? "OK" : "FALHOU");
    printf("Mesmos valores da entrada (com repetidos): %s\n", mesmos_elementos(entrada, copia, n) ? "OK" : "FALHOU");
    printf("Igual ao Insertion Sort: %s\n", n == 0 || memcmp(ref, copia, n * sizeof(int)) == 0 ? "OK" : "FALHOU");
    printf("\n");

    free(copia);
    free(ref);
}

void comparar_pivo_fixo_aleatorio(void) {
    int n = 2000;
    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    if (a == NULL || b == NULL) {
        printf("Erro de alocacao\n");
        free(a);
        free(b);
        return;
    }

    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
        b[i] = i + 1;
    }

    MetricasQuick mf = {0, 0};
    MetricasQuick mr = {0, 0};

    quick_sort(a, n, &mf);
    quick_sort_aleatorio(b, n, &mr);

    printf("--- Pivo fixo x pivo aleatorio (entrada ja ordenada, %d elementos) ---\n", n);
    printf("Pivo fixo (ultimo):  comparacoes = %ld | trocas = %ld | ordem: %s\n",
           mf.comparacoes, mf.trocas, numeros_ordenados(a, n) ? "OK" : "FALHOU");
    printf("Pivo aleatorio:      comparacoes = %ld | trocas = %ld | ordem: %s\n",
           mr.comparacoes, mr.trocas, numeros_ordenados(b, n) ? "OK" : "FALHOU");
    printf("\n");

    free(a);
    free(b);
}

void testar_quick_jogadores(Campo campo, Direcao dir) {
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

    MetricasQuick m = {0, 0};
    MetricasBubble mb = {0, 0, 0};

    printf("--- Criterio: %s | Direcao: %s ---\n", nome_campo(campo), nome_direcao(dir));
    imprimir_jogadores("Antes da ordenacao:", copia, n);

    quick_sort_jogadores(copia, n, campo, dir, &m);
    bubble_sort_jogadores(ref, n, campo, dir, &mb);

    imprimir_jogadores("Depois da ordenacao:", copia, n);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);

    printf("Ordem final: %s\n", jogadores_ordenados_por(copia, n, campo, dir) ? "OK" : "FALHOU");
    printf("Id, nome e pontuacao associados: %s\n", jogadores_preservados(original, copia, n) ? "OK" : "FALHOU");
    printf("Igual ao Bubble Sort: %s\n", jogadores_iguais(ref, copia, n) ? "OK" : "FALHOU");
    printf("\n");

    free(copia);
    free(ref);
}

void executar_quick_aleatorio(void) {
    int aleatorio[TAM_TESTE] = {42, 17, 8, 93, 25, 61, 3, 77, 50, 12};
    int repetidos[8] = {5, 3, 5, 1, 3, 5, 1, 3};
    int iguais[5] = {4, 4, 4, 4, 4};
    int ordenado[TAM_TESTE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[TAM_TESTE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int unico[1] = {7};
    int vazio[1] = {0};

    printf("===== QUICK SORT ALEATORIO: NUMEROS =====\n\n");
    testar_quick_aleatorio("Vetor aleatorio", aleatorio, TAM_TESTE);
    testar_quick_aleatorio("Vetor com valores repetidos", repetidos, 8);
    testar_quick_aleatorio("Todos os valores iguais", iguais, 5);
    testar_quick_aleatorio("Vetor ja ordenado", ordenado, TAM_TESTE);
    testar_quick_aleatorio("Vetor em ordem inversa", inverso, TAM_TESTE);
    testar_quick_aleatorio("Um elemento", unico, 1);
    testar_quick_aleatorio("Vetor vazio", vazio, 0);

    comparar_pivo_fixo_aleatorio();

    printf("===== QUICK SORT ALEATORIO: JOGADORES =====\n\n");
    testar_quick_jogadores(CAMPO_ID, CRESCENTE);
    testar_quick_jogadores(CAMPO_ID, DECRESCENTE);
    testar_quick_jogadores(CAMPO_PONTUACAO, CRESCENTE);
    testar_quick_jogadores(CAMPO_PONTUACAO, DECRESCENTE);
    testar_quick_jogadores(CAMPO_NOME, CRESCENTE);
    testar_quick_jogadores(CAMPO_NOME, DECRESCENTE);
}

void executar_central(void) {
    Dados d;
    memset(&d, 0, sizeof(d));
    d.campo = CAMPO_ID;
    d.dir = CRESCENTE;

    int opcao;

    do {
        printf("\n===== CENTRAL DE ORDENACAO =====\n");
        mostrar_estado(&d);
        printf("1 - Gerar inteiros aleatorios\n");
        printf("2 - Gerar jogadores aleatorios\n");
        printf("3 - Carregar exemplo (inteiros)\n");
        printf("4 - Carregar exemplo (jogadores)\n");
        printf("5 - Carregar de arquivo (inteiros)\n");
        printf("6 - Carregar de arquivo (jogadores)\n");
        printf("7 - Escolher criterio dos jogadores\n");
        printf("8 - Mostrar dados\n");
        printf("9 - Executar um algoritmo\n");
        printf("10 - Relatorio comparativo de todos os algoritmos\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        opcao = ler_opcao();
        printf("\n");

        switch (opcao) {
            case 1:
                gerar_inteiros(&d);
                break;
            case 2:
                gerar_jogadores(&d);
                break;
            case 3:
                carregar_exemplo_inteiros(&d);
                break;
            case 4:
                carregar_exemplo_jogadores(&d);
                break;
            case 5:
                carregar_arquivo_inteiros(&d);
                break;
            case 6:
                carregar_arquivo_jogadores(&d);
                break;
            case 7:
                escolher_criterio(&d);
                break;
            case 8:
                mostrar_dados(&d);
                break;
            case 9:
                executar_um_algoritmo(&d);
                break;
            case 10:
                relatorio_comparativo(&d);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    liberar_dados(&d);
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

    srand((unsigned)time(NULL));
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
        printf("9 - Particao e Quick Sort\n");
        printf("10 - Quick Sort aleatorio (numeros e jogadores)\n");
        printf("11 - Central de ordenacao e relatorio comparativo\n");
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
            case 9:
                executar_quick();
                break;
            case 10:
                executar_quick_aleatorio();
                break;
            case 11:
                executar_central();
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