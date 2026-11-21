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

/* Tipo opaco: os detalhes dos nos e do encadeamento ficam ocultos em agenda.c. */
typedef struct Agenda Agenda;

/* Cria uma missao pendente, ajustando a prioridade para [PRIORIDADE_MIN, PRIORIDADE_MAX]. */
Missao criar_missao(int id, const char *titulo, int prioridade);

/* Exibe os dados de uma missao na saida padrao. Ignora ponteiro NULL. */
void exibir_missao(const Missao *m);

/* Marca uma missao como concluida. Ignora ponteiro NULL. */
void concluir_missao(Missao *m);

/* Aloca e inicializa uma agenda vazia. Retorna NULL em caso de falha. */
Agenda *inicializar_agenda(void);

/* Insere uma missao no inicio da agenda. Retorna 1 em sucesso e 0 em falha. */
int adicionar_missao_inicio(Agenda *agenda, Missao missao);

/* Insere uma missao no fim da agenda. Retorna 1 em sucesso e 0 em falha. */
int adicionar_missao_fim(Agenda *agenda, Missao missao);

/* Insere um lote de missoes no fim da agenda preservando a ordem do vetor. */
int adicionar_lote_missoes(Agenda *agenda, const Missao *missoes, int quantidade);

/* Busca a primeira missao com o identificador informado. Retorna NULL se ausente. */
Missao *buscar_missao_por_id(Agenda *agenda, int id);

/* Insere uma missao imediatamente apos a referencia. Retorna 1 em sucesso e 0 em falha. */
int adicionar_missao_apos(Agenda *agenda, int id_referencia, Missao missao);

/* Insere uma missao imediatamente antes da referencia. Retorna 1 em sucesso e 0 em falha. */
int adicionar_missao_antes(Agenda *agenda, int id_referencia, Missao missao);

/* Altera os dados de uma missao valida preservando o identificador original. */
int alterar_missao(Agenda *agenda, int id, Missao novos_dados);

/* Remove a primeira missao da agenda e copia seus dados para removida (se nao NULL). */
int remover_primeira_missao(Agenda *agenda, Missao *removida);

/* Remove a ultima missao da agenda e copia seus dados para removida (se nao NULL). */
int remover_ultima_missao(Agenda *agenda, Missao *removida);

/* Remove a primeira missao com o identificador informado. */
int remover_missao_por_id(Agenda *agenda, int id, Missao *removida);

/* Remove a missao imediatamente posterior a referencia informada. */
int remover_missao_apos(Agenda *agenda, int id_referencia, Missao *removida);

/* Remove a missao imediatamente anterior a referencia informada. */
int remover_missao_antes(Agenda *agenda, int id_referencia, Missao *removida);

/* Retorna o total de missoes cadastradas (0 para lista vazia). */
int contar_missoes(const Agenda *agenda);

/* Retorna a quantidade de missoes ainda pendentes (0 para lista vazia). */
int contar_missoes_pendentes(const Agenda *agenda);

/* Ordena a agenda por prioridade decrescente, usando menor ID como desempate. */
void ordenar_agenda_por_prioridade(Agenda *agenda);

/* Salva somente os dados das missoes no arquivo binario, na ordem atual. */
int salvar_agenda(const Agenda *agenda, const char *caminho);

/*
 * Carrega missoes de um arquivo binario preservando a ordem gravada.
 * Substitui o conteudo atual da agenda para evitar duplicacoes em carregamentos sucessivos.
 * Retorna 1 em sucesso e 0 se o arquivo nao existir ou ocorrer erro de leitura/validacao.
 */
int carregar_agenda(Agenda *agenda, const char *caminho);

/* Exibe todas as missoes da agenda do inicio ao fim, ou informa se estiver vazia. */
void exibir_agenda(const Agenda *agenda);

/* Remove todos os nos da agenda, deixando-a vazia. Segura para lista ja vazia. */
void limpar_agenda(Agenda *agenda);

/* Limpa todos os nos e libera a memoria da propria estrutura Agenda. */
void liberar_agenda(Agenda *agenda);

#endif