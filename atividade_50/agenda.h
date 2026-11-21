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
int adicionar_missao_inicio(Agenda *agenda, Missao missao);
int adicionar_missao_fim(Agenda *agenda, Missao missao);
int adicionar_lote_missoes(Agenda *agenda, const Missao *missoes, int quantidade);
Missao *buscar_missao_por_id(Agenda *agenda, int id);
int adicionar_missao_apos(Agenda *agenda, int id_referencia, Missao missao);
int adicionar_missao_antes(Agenda *agenda, int id_referencia, Missao missao);
int alterar_missao(Agenda *agenda, int id, Missao novos_dados);
int remover_primeira_missao(Agenda *agenda, Missao *removida);
int remover_ultima_missao(Agenda *agenda, Missao *removida);
int remover_missao_por_id(Agenda *agenda, int id, Missao *removida);
int remover_missao_apos(Agenda *agenda, int id_referencia, Missao *removida);
int remover_missao_antes(Agenda *agenda, int id_referencia, Missao *removida);
int contar_missoes(const Agenda *agenda);
int contar_missoes_pendentes(const Agenda *agenda);
void ordenar_agenda_por_prioridade(Agenda *agenda);
int salvar_agenda(const Agenda *agenda, const char *caminho);
void exibir_agenda(const Agenda *agenda);
void limpar_agenda(Agenda *agenda);
void liberar_agenda(Agenda *agenda);

#endif