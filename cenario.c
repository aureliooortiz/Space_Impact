#include <stdio.h>

#include "cenario.h"

struct cenario_t cria_background (char *nomeCenario) {
	struct cenario_t c ;
	
	c.bg_spr = al_load_bitmap(nomeCenario) ;
	c.larg_spr = al_get_bitmap_width(c.bg_spr) ;
	c.alt_spr = al_get_bitmap_height(c.bg_spr) ; 
	c.bg_desloc_x = 0.0 ;
	c.bg_scroll_vel = BG_VEL ;
	
	return c ;
}

// Desloca a posição que a imagem será desenhada, fazendo scrolling
void desenha_background_animado (struct cenario_t *c) {
	c->bg_desloc_x -= c->bg_scroll_vel ;
	
	if (c->bg_desloc_x < -c->larg_spr) {
		c->bg_desloc_x += c->larg_spr ;
	} else if (c->bg_desloc_x > 0) {
		c->bg_desloc_x -= c->larg_spr ;
	}
	
	al_draw_bitmap (c->bg_spr, c->bg_desloc_x, 0, 0) ;
	
	// Desenha imagem mais uma vez, para criar impressão de fundo infinito
	if (c->bg_desloc_x + c->larg_spr < LARG_TELA) {
		al_draw_bitmap (c->bg_spr, c->bg_desloc_x + c->larg_spr, 0, 0) ;
	}
	
	// Volta ao inicio da imagem
	if (c->bg_desloc_x <= -c->larg_spr) {
		printf ("Looping\n") ;
		c->bg_desloc_x = 0.0 ;
	}
}

void desenha_background_estatico (struct cenario_t *c) {
	al_draw_bitmap (c->bg_spr, c->bg_desloc_x, 0, 0) ;
}
