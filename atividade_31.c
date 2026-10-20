
#include <stdio.h>

#define CAPACIDADE 20
#define MAX_FIBONACCI 25

void criar_vetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (i * 7) % 31;
    }
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

int vetor_ordenado(int vetor[], int tamanho) {
    if (vetor == NULL || tamanho <= 0) {
        return 0;
    }

    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] < vetor[i - 1]) {
            return 0;
        }
    }

    return 1;
}

int acessar_direto(int vetor[], int tamanho, int indice,
                   int *resultado, int *acessos) {
    if (vetor == NULL || indice < 0 || indice >= tamanho) {
        return 0;
    }

    *resultado = vetor[indice];
    (*acessos)++;

    return 1;
}

int encontrar_maior(int vetor[], int tamanho, int *maior,
                    int *acessos, int *comparacoes) {
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

int encontrar_pares(int vetor[], int tamanho, int alvo,
                    int *comparacoes) {
    if (vetor == NULL || tamanho <= 0) {
        return 0;
    }

    int quantidade = 0;

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            (*comparacoes)++;

            if (vetor[i] + vetor[j] == alvo) {
                printf("%d + %d = %d\n",
                       vetor[i], vetor[j], alvo);
                quantidade++;
            }
        }
    }

    return quantidade;
}

int busca_linear(int vetor[], int tamanho, int alvo,
                 int *comparacoes) {
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

int busca_binaria(int vetor[], int tamanho, int alvo,
                  int *comparacoes) {
    if (vetor == NULL || tamanho <= 0 ||
        !vetor_ordenado(vetor, tamanho)) {
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

int busca_binaria_recursiva(int vetor[], int inicio, int fim,
                            int alvo, int *chamadas,
                            int *comparacoes) {
    (*chamadas)++;

    if (vetor == NULL || inicio > fim) {
        return -1;
    }

    int meio = inicio + (fim - inicio) / 2;

    printf("Chamada %d: inicio=%d, meio=%d, fim=%d\n",
           *chamadas, inicio, meio, fim);

    (*comparacoes)++;

    if (vetor[meio] == alvo) {
        return meio;
    }

    (*comparacoes)++;

    if (vetor[meio] < alvo) {
        return busca_binaria_recursiva(
            vetor, meio + 1, fim, alvo, chamadas, comparacoes
        );
    }

    return busca_binaria_recursiva(
        vetor, inicio, meio - 1, alvo, chamadas, comparacoes
    );
}

int soma_iterativa(int n, int *operacoes) {
    int soma = 0;

    for (int i = 1; i <= n; i++) {
        soma += i;
        (*operacoes)++;
    }

    return soma;
}

int soma_recursiva(int n, int *chamadas) {
    (*chamadas)++;

    if (n <= 0) {
        return 0;
    }

    return n + soma_recursiva(n - 1, chamadas);
}

int soma_vetor_iterativa(int vetor[], int tamanho,
                         int *operacoes) {
    int soma = 0;

    for (int i = 0; i < tamanho; i++) {
        soma += vetor[i];
        (*operacoes)++;
    }

    return soma;
}

int soma_vetor_recursiva(int vetor[], int tamanho, int indice,
                         int *chamadas) {
    (*chamadas)++;

    if (vetor == NULL || indice >= tamanho) {
        return 0;
    }

    return vetor[indice] +
           soma_vetor_recursiva(
               vetor, tamanho, indice + 1, chamadas
           );
}

long long fibonacci_recursivo(int n, long long *chamadas) {
    (*chamadas)++;

    if (n <= 1) {
        return n;
    }

    return fibonacci_recursivo(n - 1, chamadas) +
           fibonacci_recursivo(n - 2, chamadas);
}

void experimento_acesso(int vetor[], int tamanho) {
    int indice;
    int resultado;
    int acessos = 0;

    printf("Informe o indice desejado (0 a %d): ", tamanho - 1);

    if (scanf("%d", &indice) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    if (acessar_direto(vetor, tamanho, indice,
                       &resultado, &acessos)) {
        printf("Valor: %d\n", resultado);
        printf("Acessos: %d\n", acessos);
        printf("Complexidade: O(1)\n");
    } else {
        printf("Indice invalido.\n");
    }
}

void experimento_maior(int vetor[], int tamanho) {
    int maior;
    int acessos = 0;
    int comparacoes = 0;

    if (encontrar_maior(vetor, tamanho, &maior,
                        &acessos, &comparacoes)) {
        printf("Maior valor: %d\n", maior);
        printf("Acessos: %d\n", acessos);
        printf("Comparacoes: %d\n", comparacoes);
        printf("Complexidade: O(n)\n");
    }
}

void experimento_pares(int vetor[], int tamanho) {
    int alvo;
    int comparacoes = 0;

    printf("Informe o valor-alvo da soma: ");

    if (scanf("%d", &alvo) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    int quantidade = encontrar_pares(
        vetor, tamanho, alvo, &comparacoes
    );

    printf("Quantidade de pares: %d\n", quantidade);
    printf("Comparacoes: %d\n", comparacoes);
    printf("Complexidade: O(n^2)\n");
}

void experimento_buscas(int vetor[], int tamanho) {
    int ordenado[CAPACIDADE];
    int alvos[4];
    int comparacoes_linear;
    int comparacoes_binaria;
    int pos_linear;
    int pos_binaria;

    for (int i = 0; i < tamanho; i++) {
        ordenado[i] = vetor[i];
    }

    ordenar_vetor(ordenado, tamanho);

    alvos[0] = ordenado[0];
    alvos[1] = ordenado[tamanho / 2];
    alvos[2] = ordenado[tamanho - 1];
    alvos[3] = 100;

    printf("\nVetor ordenado: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", ordenado[i]);
    }
    printf("\n");

    for (int i = 0; i < 4; i++) {
        comparacoes_linear = 0;
        comparacoes_binaria = 0;

        pos_linear = busca_linear(
            ordenado, tamanho, alvos[i], &comparacoes_linear
        );

        pos_binaria = busca_binaria(
            ordenado, tamanho, alvos[i], &comparacoes_binaria
        );

        printf("\nAlvo: %d\n", alvos[i]);
        printf("Linear: posicao=%d, comparacoes=%d\n",
               pos_linear, comparacoes_linear);
        printf("Binaria iterativa: posicao=%d, comparacoes=%d\n",
               pos_binaria, comparacoes_binaria);
    }

    int comparacoes_desordenado = 0;
    int alvo_desordenado = vetor[tamanho - 1];

    int posicao = busca_linear(
        vetor, tamanho, alvo_desordenado, &comparacoes_desordenado
    );

    printf("\nBusca linear no vetor original desordenado:\n");
    printf("Alvo: %d, posicao: %d, comparacoes: %d\n",
           alvo_desordenado, posicao, comparacoes_desordenado);
    printf("Busca binaria nao utilizada no vetor desordenado.\n");
}

void experimento_busca_recursiva(int vetor[], int tamanho) {
    int ordenado[CAPACIDADE];
    int alvo;
    int chamadas = 0;
    int comparacoes = 0;
    int chamadas_iterativa = 0;
    int comparacoes_iterativa = 0;

    for (int i = 0; i < tamanho; i++) {
        ordenado[i] = vetor[i];
    }

    ordenar_vetor(ordenado, tamanho);

    printf("Informe o valor que deseja procurar: ");

    if (scanf("%d", &alvo) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    int posicao_recursiva = busca_binaria_recursiva(
        ordenado, 0, tamanho - 1, alvo, &chamadas, &comparacoes
    );

    int posicao_iterativa = busca_binaria(
        ordenado, tamanho, alvo, &comparacoes_iterativa
    );

    printf("\nBusca binaria recursiva:\n");
    printf("Posicao: %d\n", posicao_recursiva);
    printf("Chamadas: %d\n", chamadas);
    printf("Comparacoes: %d\n", comparacoes);

    printf("\nBusca binaria iterativa:\n");
    printf("Posicao: %d\n", posicao_iterativa);
    printf("Comparacoes: %d\n", comparacoes_iterativa);

    printf("Complexidade das buscas: O(log n)\n");
}

void experimento_somas(int vetor[], int tamanho) {
    int chamadas_num = 0;
    int operacoes_num = 0;
    int chamadas_vetor = 0;
    int operacoes_vetor = 0;

    int rec_num = soma_recursiva(tamanho, &chamadas_num);
    int ite_num = soma_iterativa(tamanho, &operacoes_num);

    int rec_vetor = soma_vetor_recursiva(
        vetor, tamanho, 0, &chamadas_vetor
    );

    int ite_vetor = soma_vetor_iterativa(
        vetor, tamanho, &operacoes_vetor
    );

    printf("\nSoma de 1 ate %d:\n", tamanho);
    printf("Recursiva: resultado=%d, chamadas=%d\n",
           rec_num, chamadas_num);
    printf("Iterativa: resultado=%d, operacoes=%d\n",
           ite_num, operacoes_num);

    printf("\nSoma dos elementos do vetor:\n");
    printf("Recursiva: resultado=%d, chamadas=%d\n",
           rec_vetor, chamadas_vetor);
    printf("Iterativa: resultado=%d, operacoes=%d\n",
           ite_vetor, operacoes_vetor);

    printf("Resultados do vetor iguais: %s\n",
           rec_vetor == ite_vetor ? "Sim" : "Nao");
}

void experimento_fibonacci(void) {
    int n;
    long long chamadas = 0;

    printf("Informe n para Fibonacci (0 a %d): ",
           MAX_FIBONACCI);

    if (scanf("%d", &n) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    if (n < 0 || n > MAX_FIBONACCI) {
        printf("Valor fora do limite permitido.\n");
        return;
    }

    long long resultado = fibonacci_recursivo(n, &chamadas);

    printf("Fibonacci(%d) = %lld\n", n, resultado);
    printf("Chamadas recursivas: %lld\n", chamadas);
    printf("Complexidade temporal: O(2^n), aproximadamente\n");
}

int main(void) {
    int vetor[CAPACIDADE];
    int tamanho = 10;
    int opcao;

    criar_vetor(vetor, CAPACIDADE);

    do {
        printf("\n========== LABORATORIO DE ALGORITMOS ==========\n");
        printf("Tamanho atual do vetor: %d\n", tamanho);
        printf("1. Alterar tamanho do vetor\n");
        printf("2. Acesso direto - O(1)\n");
        printf("3. Encontrar maior - O(n)\n");
        printf("4. Encontrar pares - O(n^2)\n");
        printf("5. Comparar busca linear e binaria iterativa\n");
        printf("6. Comparar busca binaria recursiva e iterativa\n");
        printf("7. Comparar somas recursivas e iterativas\n");
        printf("8. Fibonacci recursivo ramificado\n");
        printf("0. Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }

        switch (opcao) {
            case 1: {
                int novo_tamanho;

                printf("Informe o tamanho (1 a %d): ",
                       CAPACIDADE);

                if (scanf("%d", &novo_tamanho) != 1) {
                    printf("Entrada invalida.\n");
                    return 1;
                }

                if (novo_tamanho < 1 ||
                    novo_tamanho > CAPACIDADE) {
                    printf("Tamanho invalido.\n");
                } else {
                    tamanho = novo_tamanho;
                    printf("Tamanho atualizado para %d.\n", tamanho);
                }

                break;
            }

            case 2:
                experimento_acesso(vetor, tamanho);
                break;

            case 3:
                experimento_maior(vetor, tamanho);
                break;

            case 4:
                experimento_pares(vetor, tamanho);
                break;

            case 5:
                experimento_buscas(vetor, tamanho);
                break;

            case 6:
                experimento_busca_recursiva(vetor, tamanho);
                break;

            case 7:
                experimento_somas(vetor, tamanho);
                break;

            case 8:
                experimento_fibonacci();
                break;

            case 0:
                printf("Encerrando o laboratorio.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}