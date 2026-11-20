/*
 * agenda.h - INTERFACE PUBLICA do modulo agenda de missoes.
 *
 * Este arquivo diz O QUE o modulo oferece (tipos e prototipos),
 * mas nao diz COMO as funcoes sao feitas. A implementacao fica em agenda.c.
 * Quem quiser usar o modulo (como o main.c) so precisa dar #include "agenda.h".
 */

/*
 * INCLUDE GUARDS: evitam que o conteudo deste cabecalho seja processado
 * duas vezes na mesma compilacao (o que causaria erro de "redefinicao de Missao").
 * Na primeira inclusao AGENDA_H ainda nao existe, entao o conteudo e lido.
 * Nas inclusoes seguintes AGENDA_H ja existe, entao tudo ate o #endif e ignorado.
 */
#ifndef AGENDA_H
#define AGENDA_H

/* Tamanho maximo do titulo de uma missao (inclui o '\0' final) */
#define TAM_TITULO 50

/* Limites validos para a prioridade de uma missao */
#define PRIORIDADE_MIN 1
#define PRIORIDADE_MAX 5

/*
 * Missao: um item da agenda.
 * O tipo fica no .h porque quem usa o modulo precisa saber o tamanho da struct
 * para declarar variaveis do tipo Missao.
 */
typedef struct {
    int id;                  /* identificador da missao */
    char titulo[TAM_TITULO]; /* nome da missao */
    int prioridade;          /* de PRIORIDADE_MIN (baixa) ate PRIORIDADE_MAX (alta) */
    int concluida;           /* 0 = pendente, 1 = concluida */
} Missao;

/*
 * Cria uma missao pendente.
 * - titulo maior que o limite e cortado; titulo NULL vira texto vazio.
 * - prioridade fora de [PRIORIDADE_MIN, PRIORIDADE_MAX] e ajustada para o limite mais proximo.
 * Retorna a missao por valor, entao nao ha alocacao dinamica nem variavel global.
 */
Missao criar_missao(int id, const char *titulo, int prioridade);

/* Mostra uma missao em uma linha na saida padrao. Ignora ponteiro NULL. */
void exibir_missao(const Missao *m);

/* Marca a missao como concluida. Ignora ponteiro NULL. */
void concluir_missao(Missao *m);

#endif /* AGENDA_H */
