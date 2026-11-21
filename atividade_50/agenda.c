#include <stdio.h>
#include <stdlib.h>
#include "agenda.h"

typedef struct No {
    Missao dado;
    struct No *proximo;
} No;

struct Agenda {
    No *inicio;
};

static No *buscar_no_por_id(Agenda *agenda, int id) {
    if (agenda == NULL) {
        return NULL;
    }

    No *atual = agenda->inicio;
    while (atual != NULL) {
        if (atual->dado.id == id) {
            return atual;
        }
        atual = atual->proximo;
    }

    return NULL;
}

static int validar_missao(const Missao *m) {
    if (m == NULL) {
        return 0;
    }
    if (m->titulo[0] == '\0') {
        return 0;
    }
    if (m->prioridade < PRIORIDADE_MIN || m->prioridade > PRIORIDADE_MAX) {
        return 0;
    }
    if (m->concluida != 0 && m->concluida != 1) {
        return 0;
    }

    return 1;
}

static void liberar_nos(No *inicio) {
    No *atual = inicio;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

static int deve_vir_antes(const Missao *a, const Missao *b) {
    if (a->prioridade != b->prioridade) {
        return a->prioridade > b->prioridade;
    }

    return a->id < b->id;
}

Missao criar_missao(int id, const char *titulo, int prioridade) {
    Missao m;

    m.id = id;
    snprintf(m.titulo, TAM_TITULO, "%s", titulo != NULL ? titulo : "");

    if (prioridade < PRIORIDADE_MIN) {
        prioridade = PRIORIDADE_MIN;
    } else if (prioridade > PRIORIDADE_MAX) {
        prioridade = PRIORIDADE_MAX;
    }
    m.prioridade = prioridade;

    m.concluida = 0;

    return m;
}

void exibir_missao(const Missao *m) {
    if (m == NULL) {
        return;
    }

    printf("[%d] %-25s prioridade %d | %s\n",
           m->id,
           m->titulo,
           m->prioridade,
           m->concluida ? "concluida" : "pendente");
}

void concluir_missao(Missao *m) {
    if (m == NULL) {
        return;
    }

    m->concluida = 1;
}

Agenda *inicializar_agenda(void) {
    Agenda *agenda = malloc(sizeof(Agenda));
    if (agenda == NULL) {
        return NULL;
    }

    agenda->inicio = NULL;
    return agenda;
}

int adicionar_missao_inicio(Agenda *agenda, Missao missao) {
    if (agenda == NULL) {
        return 0;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = missao;
    novo->proximo = agenda->inicio;
    agenda->inicio = novo;

    return 1;
}

int adicionar_missao_fim(Agenda *agenda, Missao missao) {
    if (agenda == NULL) {
        return 0;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = missao;
    novo->proximo = NULL;

    if (agenda->inicio == NULL) {
        agenda->inicio = novo;
        return 1;
    }

    No *atual = agenda->inicio;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo;

    return 1;
}

int adicionar_lote_missoes(Agenda *agenda, const Missao *missoes, int quantidade) {
    if (agenda == NULL || missoes == NULL || quantidade <= 0) {
        return 0;
    }

    No *inicio_lote = NULL;
    No *fim_lote = NULL;

    for (int i = 0; i < quantidade; i++) {
        No *novo = malloc(sizeof(No));
        if (novo == NULL) {
            liberar_nos(inicio_lote);
            return 0;
        }

        novo->dado = missoes[i];
        novo->proximo = NULL;

        if (inicio_lote == NULL) {
            inicio_lote = novo;
            fim_lote = novo;
        } else {
            fim_lote->proximo = novo;
            fim_lote = novo;
        }
    }

    if (agenda->inicio == NULL) {
        agenda->inicio = inicio_lote;
    } else {
        No *atual = agenda->inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = inicio_lote;
    }

    return 1;
}

Missao *buscar_missao_por_id(Agenda *agenda, int id) {
    No *encontrado = buscar_no_por_id(agenda, id);
    if (encontrado == NULL) {
        return NULL;
    }

    return &encontrado->dado;
}

int adicionar_missao_apos(Agenda *agenda, int id_referencia, Missao missao) {
    No *referencia = buscar_no_por_id(agenda, id_referencia);
    if (referencia == NULL) {
        return 0;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = missao;
    novo->proximo = referencia->proximo;
    referencia->proximo = novo;

    return 1;
}

int adicionar_missao_antes(Agenda *agenda, int id_referencia, Missao missao) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return 0;
    }

    No *anterior = NULL;
    No *atual = agenda->inicio;

    while (atual != NULL && atual->dado.id != id_referencia) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return 0;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = missao;
    novo->proximo = atual;

    if (anterior == NULL) {
        agenda->inicio = novo;
    } else {
        anterior->proximo = novo;
    }

    return 1;
}

int alterar_missao(Agenda *agenda, int id, Missao novos_dados) {
    if (agenda == NULL || !validar_missao(&novos_dados)) {
        return 0;
    }

    No *alvo = buscar_no_por_id(agenda, id);
    if (alvo == NULL) {
        return 0;
    }

    int id_original = alvo->dado.id;
    alvo->dado = novos_dados;
    alvo->dado.id = id_original;

    return 1;
}

int remover_primeira_missao(Agenda *agenda, Missao *removida) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return 0;
    }

    No *primeiro = agenda->inicio;

    if (removida != NULL) {
        *removida = primeiro->dado;
    }

    agenda->inicio = primeiro->proximo;
    free(primeiro);

    return 1;
}

int remover_ultima_missao(Agenda *agenda, Missao *removida) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return 0;
    }

    No *anterior = NULL;
    No *atual = agenda->inicio;

    while (atual->proximo != NULL) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (removida != NULL) {
        *removida = atual->dado;
    }

    if (anterior == NULL) {
        agenda->inicio = NULL;
    } else {
        anterior->proximo = NULL;
    }

    free(atual);

    return 1;
}

int remover_missao_por_id(Agenda *agenda, int id, Missao *removida) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return 0;
    }

    No *anterior = NULL;
    No *atual = agenda->inicio;

    while (atual != NULL && atual->dado.id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return 0;
    }

    if (removida != NULL) {
        *removida = atual->dado;
    }

    if (anterior == NULL) {
        agenda->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);

    return 1;
}

int remover_missao_apos(Agenda *agenda, int id_referencia, Missao *removida) {
    No *referencia = buscar_no_por_id(agenda, id_referencia);
    if (referencia == NULL || referencia->proximo == NULL) {
        return 0;
    }

    No *alvo = referencia->proximo;

    if (removida != NULL) {
        *removida = alvo->dado;
    }

    referencia->proximo = alvo->proximo;
    free(alvo);

    return 1;
}

int remover_missao_antes(Agenda *agenda, int id_referencia, Missao *removida) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return 0;
    }

    if (agenda->inicio->dado.id == id_referencia) {
        return 0;
    }

    No *ant_anterior = NULL;
    No *anterior = agenda->inicio;
    No *atual = agenda->inicio->proximo;

    while (atual != NULL && atual->dado.id != id_referencia) {
        ant_anterior = anterior;
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return 0;
    }

    if (removida != NULL) {
        *removida = anterior->dado;
    }

    if (ant_anterior == NULL) {
        agenda->inicio = atual;
    } else {
        ant_anterior->proximo = atual;
    }

    free(anterior);

    return 1;
}

int contar_missoes(const Agenda *agenda) {
    if (agenda == NULL) {
        return 0;
    }

    int total = 0;
    const No *atual = agenda->inicio;
    while (atual != NULL) {
        total++;
        atual = atual->proximo;
    }

    return total;
}

int contar_missoes_pendentes(const Agenda *agenda) {
    if (agenda == NULL) {
        return 0;
    }

    int pendentes = 0;
    const No *atual = agenda->inicio;
    while (atual != NULL) {
        if (!atual->dado.concluida) {
            pendentes++;
        }
        atual = atual->proximo;
    }

    return pendentes;
}

void ordenar_agenda_por_prioridade(Agenda *agenda) {
    if (agenda == NULL || agenda->inicio == NULL) {
        return;
    }

    for (No *i = agenda->inicio; i != NULL; i = i->proximo) {
        No *melhor = i;
        for (No *j = i->proximo; j != NULL; j = j->proximo) {
            if (deve_vir_antes(&j->dado, &melhor->dado)) {
                melhor = j;
            }
        }

        if (melhor != i) {
            Missao temp = i->dado;
            i->dado = melhor->dado;
            melhor->dado = temp;
        }
    }
}

int salvar_agenda(const Agenda *agenda, const char *caminho) {
    if (agenda == NULL || caminho == NULL || caminho[0] == '\0') {
        return 0;
    }

    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) {
        return 0;
    }

    const No *atual = agenda->inicio;
    while (atual != NULL) {
        if (fwrite(&atual->dado, sizeof(Missao), 1, arquivo) != 1) {
            fclose(arquivo);
            return 0;
        }
        atual = atual->proximo;
    }

    if (fclose(arquivo) != 0) {
        return 0;
    }

    return 1;
}

void exibir_agenda(const Agenda *agenda) {
    if (agenda == NULL || agenda->inicio == NULL) {
        printf("Agenda vazia.\n");
        return;
    }

    const No *atual = agenda->inicio;
    while (atual != NULL) {
        exibir_missao(&atual->dado);
        atual = atual->proximo;
    }
}

void limpar_agenda(Agenda *agenda) {
    if (agenda == NULL) {
        return;
    }

    liberar_nos(agenda->inicio);
    agenda->inicio = NULL;
}

void liberar_agenda(Agenda *agenda) {
    if (agenda == NULL) {
        return;
    }

    limpar_agenda(agenda);
    free(agenda);
}