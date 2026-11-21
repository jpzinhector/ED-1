#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdio.h>

#define LARGURA_JANELA 960
#define ALTURA_JANELA  768
#define TITULO_JANELA  "Arena Survival - ED1"

/* ============================================================
 *  CONSTANTES DA ANIMAÇÃO
 *  O sprite sheet "player_walk.png" é uma grade de 5 colunas x 2 linhas.
 *  Cada célula tem 144x180 px, então a imagem inteira tem 720x360 px.
 *  Os frames são numerados da esquerda p/ direita, de cima p/ baixo:
 *      0 1 2 3 4
 *      5 6 7 8 9
 * ============================================================ */
#define FRAMES_COLUNAS  5      // quantos frames existem na horizontal
#define FRAMES_LINHAS   2      // quantos frames existem na vertical
#define TOTAL_FRAMES    10     // FRAMES_COLUNAS * FRAMES_LINHAS
#define FPS_ANIMACAO    12.0f  // frames trocados por segundo (maior = pernas mais rápidas)
#define FRAME_PARADO    0      // frame mostrado quando o jogador não está andando

typedef struct {
    Vector2 posicao;
    Vector2 tamanho;     // tamanho com que o player é DESENHADO na tela (e usado na colisão)
    Texture2D sprite;    // agora é o sprite sheet inteiro, não só um desenho
    float velocidade;

    /* ---- Campos novos, usados só pela animação ---- */
    float larguraFrame;  // largura de UM frame dentro do sprite sheet (em pixels da imagem)
    float alturaFrame;   // altura de UM frame dentro do sprite sheet
    int   frameAtual;    // índice do frame que está sendo mostrado (0 a TOTAL_FRAMES-1)
    float tempoFrame;    // acumula o tempo (em segundos) desde a última troca de frame
    bool  andando;       // true se alguma tecla de movimento está pressionada
    bool  olhandoDireita;// true = sprite original (olha p/ direita); false = espelhado
} Player;

typedef struct {
    Texture2D fundo;
    Rectangle limites;
} Arena;

void IniciarPlayer(Player *player)
{
    // Carrega o sprite sheet de caminhada (coloque o arquivo em assets/)
    player->sprite = LoadTexture("assets/player_walk.png");

    // Tamanho do personagem na tela. Mantém a proporção de um frame (144x180),
    // por isso 85x104 e não mais 75x104 (senão o anão ficaria "esticado").
    player->tamanho = (Vector2){ 85, 104 };

    player->posicao = (Vector2){
        LARGURA_JANELA / 2.0f - player->tamanho.x / 2.0f,
        ALTURA_JANELA  / 2.0f - player->tamanho.y / 2.0f
    };

    player->velocidade = 250.0f; // pixels por segundo

    // Descobre o tamanho de um frame dividindo a imagem pela quantidade de frames.
    // 720 / 5 = 144 de largura  |  360 / 2 = 180 de altura
    player->larguraFrame = (float)player->sprite.width  / FRAMES_COLUNAS;
    player->alturaFrame  = (float)player->sprite.height / FRAMES_LINHAS;

    // Estado inicial da animação: parado, olhando para a direita, no frame 0
    player->frameAtual     = FRAME_PARADO;
    player->tempoFrame     = 0.0f;
    player->andando        = false;
    player->olhandoDireita = true;
}

void IniciarArena(Arena *arena)
{
    arena->fundo = LoadTexture("assets/arena.png");

    arena->limites = (Rectangle){ 55, 100, 840, 590};
}

void DesenharArena(const Arena *arena)
{
    DrawTexturePro(
        arena->fundo,
        (Rectangle){ 0, 0, (float)arena->fundo.width, (float)arena->fundo.height },
        (Rectangle){ 0, 0, LARGURA_JANELA, ALTURA_JANELA },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

void AtualizarPlayer(Player *player, Arena *arena)
{
    // deltaTime = segundos que passaram desde o frame anterior do JOGO.
    // Usamos tanto para mover quanto para animar, assim nada depende do FPS.
    float deltaTime = GetFrameTime();

    Vector2 direcao = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    direcao.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  direcao.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  direcao.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direcao.x += 1.0f;

    // Se a direção é diferente de (0,0), o jogador está tentando andar.
    // Guardamos isso ANTES de normalizar, só para a animação saber.
    player->andando = (direcao.x != 0.0f || direcao.y != 0.0f);

    // Só troca o lado para o qual o anão olha quando há movimento HORIZONTAL.
    // Assim, ao andar só para cima/baixo ele mantém o último lado.
    if (direcao.x > 0.0f) player->olhandoDireita = true;
    if (direcao.x < 0.0f) player->olhandoDireita = false;

    if (player->andando) {
        direcao = Vector2Normalize(direcao); // diagonal não pode ser mais rápida
    }

    player->posicao.x += direcao.x * player->velocidade * deltaTime;
    player->posicao.y += direcao.y * player->velocidade * deltaTime;

    /* ---- Limites da arena (código original, sem alterações) ---- */
    if (player->posicao.x < arena->limites.x)
        player->posicao.x = arena->limites.x;

    if (player->posicao.y < arena->limites.y)
        player->posicao.y = arena->limites.y;

    if (player->posicao.x + player->tamanho.x >
        arena->limites.x + arena->limites.width)
    {
        player->posicao.x =
            arena->limites.x + arena->limites.width - player->tamanho.x;
    }

    if (player->posicao.y + player->tamanho.y >
        arena->limites.y + arena->limites.height)
    {
        player->posicao.y =
            arena->limites.y + arena->limites.height - player->tamanho.y;
    }


    /* ---- Animação ---- */
    if (player->andando) {
        // Quanto tempo cada frame fica na tela: 1 / 12 ≈ 0,083 s
        float duracaoFrame = 1.0f / FPS_ANIMACAO;

        player->tempoFrame += deltaTime;

        // "while" (e não "if") para não perder frames caso o jogo dê uma travada
        while (player->tempoFrame >= duracaoFrame) {
            player->tempoFrame -= duracaoFrame;

            // % TOTAL_FRAMES faz o contador voltar a 0 depois do frame 9 (ciclo infinito)
            player->frameAtual = (player->frameAtual + 1) % TOTAL_FRAMES;
        }
    } else {
        // Parou de andar: volta para a pose parada e zera o cronômetro,
        // assim a próxima caminhada sempre começa do começo do ciclo.
        player->frameAtual = FRAME_PARADO;
        player->tempoFrame = 0.0f;
    }
}

void DesenharPlayer(const Player *player)
{
    // Converte o número do frame (0 a 9) em coluna e linha na grade:
    //   coluna = resto da divisão por 5  |  linha = divisão inteira por 5
    // Ex.: frame 7 -> coluna 2, linha 1
    int coluna = player->frameAtual % FRAMES_COLUNAS;
    int linha  = player->frameAtual / FRAMES_COLUNAS;

    // "origem" = o retângulo DENTRO do sprite sheet que será recortado
    Rectangle origem = {
        coluna * player->larguraFrame,
        linha  * player->alturaFrame,
        player->larguraFrame,
        player->alturaFrame
    };

    // Truque do raylib: largura NEGATIVA na origem espelha a imagem na horizontal.
    // O sprite original olha para a direita, então espelhamos ao ir para a esquerda.
    if (!player->olhandoDireita) {
        origem.width = -origem.width;
    }

    // "destino" = onde e com que tamanho o recorte aparece na tela
    Rectangle destino = { player->posicao.x, player->posicao.y, player->tamanho.x, player->tamanho.y };

    DrawTexturePro(player->sprite, origem, destino, (Vector2){ 0, 0 }, 0.0f, WHITE);
}

void DesenharUI (int pontos, float tempoDecorrido)
{
    int minutos = (int)tempoDecorrido / 60;
    int segundos = (int)tempoDecorrido % 60;

    char bufferPontos[32];
    char bufferTempo[32];

    // Formata os textos
    snprintf(bufferPontos, sizeof(bufferPontos), "PONTOS: %04d", pontos);
    snprintf(bufferTempo, sizeof(bufferTempo), "TEMPO: %02d:%02d", minutos, segundos);

    //Posição da UI
    float uiX = 720.0f;
    float uiY = 0.0f;
    float uiLargura = 230.0f;
    float uiAltura = 100.0f;

    Rectangle outerRect = {uiX, uiY, uiLargura, uiAltura};
    Rectangle innerRect = {uiX + 4, uiY + 4, uiLargura - 8, uiAltura - 8};

    // Cores do retangulo

    Color backgroundDark = (Color) {3, 19, 38, 255};
    Color borderHighlight = (Color) {77, 134, 184, 255};

    // Desenha as formas
    DrawRectangleRounded(outerRect, 0.2f, 8, (Color){55, 100, 141, 255});
    DrawRectangleRoundedLines(outerRect, 0.2f, 9, borderHighlight);
    DrawRectangleRounded(innerRect, 0.18, 8, backgroundDark);

    // Renderiza os textos
    DrawText (bufferPontos, uiX + 30, uiY +25, 20, WHITE);
    DrawText (bufferTempo, uiX + 30, uiY + 60, 20, WHITE);
}
    
void FinalizarRecursos(Player *player, Arena *arena)
{
    UnloadTexture(player->sprite);
    UnloadTexture(arena->fundo);
}

int main(void)
{
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, TITULO_JANELA);
    SetTargetFPS(60);

    SetWindowPosition(50, 50);

    Player *player = malloc(sizeof(Player));
    Arena *arena = malloc(sizeof(Arena));

    IniciarPlayer(player);
    IniciarArena(arena);

    int pontosAtuais = 0;
    float tempoAtivo = 0.0f;

    while (!WindowShouldClose())
    {
        AtualizarPlayer(player, arena);
        tempoAtivo += GetFrameTime();

        BeginDrawing();

            ClearBackground(BLACK);
            DesenharArena(arena);
            DesenharPlayer(player);

            DesenharUI(pontosAtuais, tempoAtivo);

        EndDrawing();
    }

    FinalizarRecursos(player, arena);

    free(player);
    free(arena);

    CloseWindow();

    return 0;
}