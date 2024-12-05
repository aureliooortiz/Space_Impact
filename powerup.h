#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "player.h"
#include "bala.h"
#include "utili.h"

#define TEMPO_CONGELADO 90
// Tempo que o player utiliza o tiro congelante
#define GELO_COOLDOWN 500

#define POWERUP_COOLDOWN 800
//#define LADO_POWERUP 30
#define LARG_POWERUP 30
#define ALT_POWERUP 30

#define PASSO_POWERUP 7

#define GELO 1

struct powerup_t {
	int x ;
	int y ;
	int tipo ;
	int larg_power ;
	int alt_power ;
	int powerup_timer ;
	int coletado ;
} ;

struct powerup_t cria_powerup () ;

void movimenta_powerup (struct powerup_t *p, int passo, int trajetoria) ;

void tiro_congelante (struct bala_t *b) ;

// Coração para recuperar vida

#endif
