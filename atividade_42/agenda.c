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

int adicionar_missao(Agenda *agenda, Missao missao) {
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

void exibir_agenda(const Agenda *agenda) {
    if (agenda == NULL) {
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