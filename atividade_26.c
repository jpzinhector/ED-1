
#include <stdio.h>

int acessar_direto(int vetor[], int tamanho, int indice, int *resultado, int *acessos) {
    if (indice < 0 || indice >= tamanho) {
        return 0;
    }

    *resultado = vetor[indice];
    (*acessos)++;

    return 1;
}

int encontrar_maior(int vetor[], int tamanho, int *maior, int *acessos, int *comparacoes) {
    if (tamanho <= 0) {
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

int main(void) {
    int vetor[20];
    int tamanhos[] = {5, 10, 20};
    int quantidade = 3;

    for (int i = 0; i < 20; i++) {
        vetor[i] = (i * 7) % 31;
    }

    for (int t = 0; t < quantidade; t++) {
        int n = tamanhos[t];
        int resultado;
        int maior;
        int acessos_diretos = 0;
        int acessos_busca = 0;
    
        int comparacoes = 0;

        printf("\nTamanho do vetor: %d\n", n);

        if (acessar_direto(vetor, n, n / 2, &resultado, &acessos_diretos)) {
            printf("Acesso direto: valor = %d\n", resultado);
            printf("Acessos diretos: %d\n", acessos_diretos);
        }

        if (encontrar_maior(vetor, n, &maior, &acessos_busca, &comparacoes)) {
            printf("Maior valor: %d\n", maior);
            printf("Acessos na busca: %d\n", acessos_busca);
            printf("Comparacoes na busca: %d\n", comparacoes);
        }

        int acessos_invalidos = 0;

        if (!acessar_direto(vetor, n, n, &resultado, &acessos_invalidos)) {
            printf("Indice invalido rejeitado corretamente.\n");
        }
    }

    return 0;
}
