#ifndef AGENDA_H
#define AGENDA_H

#define TAM_TITULO 50

#define PRIORIDADE_MIN 1
#define PRIORIDADE_MAX 5

typedef struct {
    int id;
    char titulo[TAM_TITULO];
    int prioridade;
    int concluida;
} Missao;

typedef struct Agenda Agenda;

Missao criar_missao(int id, const char *titulo, int prioridade);
void exibir_missao(const Missao *m);
void concluir_missao(Missao *m);

Agenda *inicializar_agenda(void);
int adicionar_missao(Agenda *agenda, Missao missao);
void exibir_agenda(const Agenda *agenda);
void liberar_agenda(Agenda *agenda);

#endif