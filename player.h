#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro.h>

#include "controle.h"
#include "arma.h"
#include "powerup.h"

// Direções que o player pode andar
#define BAIXO 0
#define CIMA 1
#define DIREITA 2
#define ESQUERDA 3

#define DANO_COOLDOWN 90

// Velocidade do andar do jogador
#define PASSO_QUADRADO 7

//#define LADO_QUADRADO 50
#define LARG_NAVE 27
#define ALT_NAVE 17


struct jogador_t {
	struct controle_t *controle ;
	struct arma_t *arma ;
	ALLEGRO_BITMAP *player_spr ;
	// Posição atual do jogador
	int x ;
	int y ;
	int larg ;
	int alt ;
	int vida ;
	int invulnerabilidade ;
	int gelo_timer ;
	float larg_spr_player ;
	float alt_spr_player ;
} ;

// Cria e retorna um player
struct jogador_t *cria_jogador (int x, int y) ;

// Verifica e movimenta o player
void movimenta_jogador (struct jogador_t *p, int passo, int trajetoria) ;

void jogador_atira (struct jogador_t *p) ;

void jogador_perde_vida (struct jogador_t *p) ;

// Destroi um player
void destroi_jogador (struct jogador_t *p) ;

#endif
