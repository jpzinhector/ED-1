
#include <stdio.h>

int acessar_direto(int vetor[], int tamanho, int indice, int *resultado, int *acessos) {
    if (vetor == NULL || indice < 0 || indice >= tamanho) {
        return 0;
    }

    *resultado = vetor[indice];
    (*acessos)++;
    return 1;
}

int encontrar_maior(int vetor[], int tamanho, int *maior, int *acessos, int *comparacoes) {
    if (vetor == NULL || tamanho <= 0) {
        return 0;
    }

    *maior = vetor[0];
    (*acessos)++;

    for (int i = 1; i < tamanho; i++) {
        (*acessos)++;
        (*comparacoes)++;

        if (vetor[i] > *maior) {
            *maior = vetor[i];
        }
    }

    return 1;
}

int encontrar_pares(int vetor[], int tamanho, int alvo, int *comparacoes) {
    if (vetor == NULL || tamanho <= 0) {
        return 0;
    }

    int quantidade_pares = 0;

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            (*comparacoes)++;

            if (vetor[i] + vetor[j] == alvo) {
                printf("Par encontrado: %d + %d = %d\n",
                       vetor[i], vetor[j], alvo);
                quantidade_pares++;
            }
        }
    }

    return quantidade_pares;
}

void ordenar_vetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

int busca_binaria(int vetor[], int tamanho, int alvo, int *comparacoes) {
    if (vetor == NULL || tamanho <= 0) {
        return -1;
    }

    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        printf("Inicio: %d, Meio: %d, Fim: %d\n",
               inicio, meio, fim);

        (*comparacoes)++;

        if (vetor[meio] == alvo) {
            return meio;
        }

        (*comparacoes)++;

        if (vetor[meio] < alvo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return -1;
}

int busca_linear(int vetor[], int tamanho, int alvo, int *comparacoes) {
    if (vetor == NULL || tamanho <= 0) {
        return -1;
    }

    for (int i = 0; i < tamanho; i++) {
        (*comparacoes)++;

        if (vetor[i] == alvo) {
            return i;
        }
    }

    return -1;
}

void comparar_buscas(int vetor[], int tamanho, int alvo) {
    int comparacoes_linear = 0;
    int comparacoes_binaria = 0;

    int posicao_linear = busca_linear(
        vetor, tamanho, alvo, &comparacoes_linear
    );

    int posicao_binaria = busca_binaria(
        vetor, tamanho, alvo, &comparacoes_binaria
    );

    printf("Alvo: %d\n", alvo);
    printf("%-20s %-12s %-15s\n",
           "Algoritmo", "Posicao", "Comparacoes");

    printf("%-20s %-12d %-15d\n",
           "Busca linear", posicao_linear, comparacoes_linear);

    printf("%-20s %-12d %-15d\n",
           "Busca binaria", posicao_binaria, comparacoes_binaria);
}

int main(void) {
    int vetor[20];
    int tamanhos[] = {5, 10, 20};
    int quantidade = 3;
    int alvo_pares = 28;

    for (int i = 0; i < 20; i++) {
        vetor[i] = (i * 7) % 31;
    }

    for (int t = 0; t < quantidade; t++) {
        int n = tamanhos[t];
        int resultado;
        int maior;
        int acessos_diretos = 0;
        int acessos_busca = 0;
        int comparacoes_maior = 0;
        int comparacoes_pares = 0;
        int acessos_invalidos = 0;

        printf("\n===== TAMANHO: %d =====\n", n);

        if (acessar_direto(vetor, n, n / 2,
                           &resultado, &acessos_diretos)) {
            printf("Acesso direto: valor = %d\n", resultado);
            printf("Acessos diretos: %d\n", acessos_diretos);
        }

        if (encontrar_maior(vetor, n, &maior,
                            &acessos_busca, &comparacoes_maior)) {
            printf("Maior valor: %d\n", maior);
            printf("Acessos na busca: %d\n", acessos_busca);
            printf("Comparacoes para encontrar o maior: %d\n",
                   comparacoes_maior);
        }

        if (!acessar_direto(vetor, n, n,
                            &resultado, &acessos_invalidos)) {
            printf("Indice invalido rejeitado corretamente.\n");
        }

        printf("\nPares cuja soma e %d:\n", alvo_pares);

        int total_pares = encontrar_pares(
            vetor, n, alvo_pares, &comparacoes_pares
        );

        printf("Quantidade de pares encontrados: %d\n", total_pares);
        printf("Comparacoes entre pares: %d\n", comparacoes_pares);

        int ordenado[20];

        for (int i = 0; i < n; i++) {
            ordenado[i] = vetor[i];
        }

        ordenar_vetor(ordenado, n);

        printf("\n--- Comparacao entre buscas no vetor ordenado ---\n");

        int alvos[] = {
            ordenado[0],
            ordenado[n / 2],
            ordenado[n - 1],
            100
        };

        for (int i = 0; i < 4; i++) {
            printf("\n");
            comparar_buscas(ordenado, n, alvos[i]);
        }

        printf("\n--- Busca linear no vetor original desordenado ---\n");

        int comparacoes_desordenado = 0;
        int alvo_desordenado = vetor[n - 1];

        int posicao_desordenada = busca_linear(
            vetor, n, alvo_desordenado, &comparacoes_desordenado
        );

        printf("Alvo: %d\n", alvo_desordenado);
        printf("Posicao encontrada: %d\n", posicao_desordenada);
        printf("Comparacoes: %d\n", comparacoes_desordenado);
        printf("Busca binaria nao aplicada: vetor desordenado.\n");
    }

    return 0;
}