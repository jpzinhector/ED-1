/*
 * agenda.c - IMPLEMENTACAO do modulo agenda de missoes.
 *
 * Aqui ficam os corpos das funcoes declaradas em agenda.h.
 * O main.c nao enxerga este arquivo: ele so conhece o que esta no cabecalho.
 */

#include <stdio.h>   /* snprintf e printf */
#include "agenda.h"  /* inclui a propria interface para o compilador conferir
                        se as definicoes abaixo batem com os prototipos */

Missao criar_missao(int id, const char *titulo, int prioridade) {
    Missao m;

    m.id = id;

    /* snprintf nunca escreve alem de TAM_TITULO e sempre termina com '\0' */
    snprintf(m.titulo, TAM_TITULO, "%s", titulo != NULL ? titulo : "");

    /* garante que a prioridade fique dentro dos limites do modulo */
    if (prioridade < PRIORIDADE_MIN) {
        prioridade = PRIORIDADE_MIN;
    } else if (prioridade > PRIORIDADE_MAX) {
        prioridade = PRIORIDADE_MAX;
    }
    m.prioridade = prioridade;

    m.concluida = 0; /* toda missao nasce pendente */

    return m; /* devolve uma copia: nada de malloc, nada de global */
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
