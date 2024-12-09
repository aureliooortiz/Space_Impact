#ifndef __COLISAO_H__
#define __COLISAO_H__

#include "player.h"
#include "inimigos.h"
#include "powerup.h"

// Guarda as funções que fazem verificação das colisões
// Retornam 1 caso tenha ocorrido colisão
int colisao_player_inimigo(struct jogador_t *p, struct lista_t *l) ;

// Colisão de inimigo para player, para quando o player estiver parado
int colisao_inimigo_player(struct jogador_t *p, struct inimigo_t *i) ;

int colisao_boss_player(struct jogador_t *p, struct inimigo_t *i) ;

// Colisão de inimigo para inimigo
int colisao_inimigo_inimigo (struct inimigo_t *i, struct lista_t *l) ;

// Colisão de tiro para inimigo
int colisao_inimigo_bala(struct jogador_t *p, struct inimigo_t *i) ;

// Colisão de player para tiro do chefe, para quando player estiver parado
int colisao_bala_player_boss (struct jogador_t *p, struct arma_t *arma) ;

// Colisão de player para tiro do inimigo, para quando player estiver parado
int colisao_bala_player (struct jogador_t *p, struct inimigo_t *i) ;

// Colisão de player para tiro
int colisao_player_bala (struct jogador_t *p, struct lista_t *l) ;

// Colisão do player com powerup
int colisao_player_powerup(struct jogador_t *p, struct powerup_t power) ;

#endif
