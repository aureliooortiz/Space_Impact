#ifndef __POWERUP_H__
#define __POWERUP_H__

#include <allegro5/allegro.h>
#include <stdlib.h>

#include "player.h"
#include "bala.h"
#include "utili.h"

#define TEMPO_CONGELADO 90
// Tempo que o player utiliza o tiro congelante
#define GELO_COOLDOWN 500

#define POWERUP_COOLDOWN 600
#define LARG_POWERUP 30
#define ALT_POWERUP 30

#define PASSO_POWERUP 7

#define GELO 1

struct coracao_t {
	ALLEGRO_BITMAP *coracao_spr ;
	float larg_spr ;
	float alt_spr ;
};

struct gelo_t {
	ALLEGRO_BITMAP *gelo_spr ;
	float larg_spr ;
	float alt_spr ;
};


struct powerup_t {
	//struct powerup_t *prox ;
	struct coracao_t *coracao ;
	struct gelo_t *gelo ;
	int x ;
	int y ;
	int tipo ;
	int larg_power ;
	int alt_power ;
	int powerup_timer ;
	int coletado ;
} ;

struct powerup_t cria_powerup (int x, int y, int tipo) ;

void desenha_powerup (struct powerup_t *power) ;

void movimenta_powerup (struct powerup_t *p, int passo, int trajetoria) ;

void tiro_congelante (struct bala_t *b) ;

// Coração para recuperar vida

void destroi_powerup (struct powerup_t *p) ;

#endif
