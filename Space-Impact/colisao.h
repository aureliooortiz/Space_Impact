#ifndef __COLISAO_H__
#define __COLISAO_H__

#include "player.h"
#include "inimigos.h"

// Guarda as funções que fazem verificação das colisões
// Retornam 1 caso tenha ocorrido colisão

int colisao_player_inimigo(struct jogador_t *p, struct lista_t *l) ;

// Colisão de inimigo para inimigo

// Colisão de tiro para inimigo

// Colisão de player para tiro

#endif
