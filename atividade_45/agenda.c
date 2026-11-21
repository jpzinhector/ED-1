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

void liberar_agenda(Agenda *agenda) {
    if (agenda == NULL) {
        return;
    }

    No *atual = agenda->inicio;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(agenda);
}