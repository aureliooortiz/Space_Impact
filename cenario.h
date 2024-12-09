#ifndef __CENARIO_H__
#define __CENARIO_H__

#include <allegro5/allegro.h>

#include "utili.h"

#define BG_VEL 2.0

struct cenario_t {
	ALLEGRO_BITMAP *bg_spr ;
	float bg_desloc_x ;
	float bg_scroll_vel ;
	float larg_spr ;
	float alt_spr ;
} ;

struct cenario_t cria_background (char *nomeCenario) ;

void desenha_background_animado (struct cenario_t *c) ;

void desenha_background_estatico (struct cenario_t *c) ;

#endif
