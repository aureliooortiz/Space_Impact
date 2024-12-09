#include "powerup.h"

struct powerup_t cria_powerup (int x, int y, int tipo) {
	struct powerup_t p ;
	
	//p = malloc (sizeof(struct powerup_t)) ;
	p.x = x ;
	p.y = y ;
	p.tipo = tipo ;
	p.coletado = 0 ;
	p.larg_power = LARG_POWERUP ;
	p.alt_power = ALT_POWERUP ;
	p.powerup_timer = POWERUP_COOLDOWN ;
	//p->prox = prox ;
	
	p.coracao = malloc (sizeof(struct coracao_t)) ;
	p.coracao->coracao_spr = al_load_bitmap("assets/Heart.png") ;
	p.coracao->larg_spr = al_get_bitmap_width(p.coracao->coracao_spr) / 3 ;
	p.coracao->alt_spr = al_get_bitmap_height(p.coracao->coracao_spr) ;
	
	p.gelo = malloc (sizeof(struct gelo_t)) ;
	p.gelo->gelo_spr = al_load_bitmap("assets/power-up.png") ;
	p.gelo->larg_spr = al_get_bitmap_width(p.gelo->gelo_spr) / 2 ;
	p.gelo->alt_spr = (al_get_bitmap_height(p.gelo->gelo_spr) / 2) - 1 ;
	
	return p ;
}

void desenha_powerup (struct powerup_t *power) {
	al_draw_bitmap_region (power->gelo->gelo_spr, 0, 1, power->gelo->larg_spr, 
								power->gelo->alt_spr, power->x, power->y, 0) ;
}

void movimenta_powerup (struct powerup_t *p, int passo, int trajetoria) {
	switch (trajetoria) {
		case BAIXO:
			if ((p->y + PASSO_POWERUP) < ALT_TELA) {
				p->y += passo*PASSO_POWERUP ;
			}	
			break ;
		case CIMA:
			if ((p->y - PASSO_POWERUP) > 0) {
				p->y -= passo*PASSO_POWERUP ;
			}	
			break ;
		case DIREITA:
			if ((p->x + PASSO_POWERUP) < LARG_TELA) {	
				p->x += passo*PASSO_POWERUP ;
			}	
			break ;		
		case ESQUERDA:
			// Não atualiza a posição do powerup se o mesmo já saiu da tela
			//if ((p->x - PASSO_POWERUP) < -LARG_POWERUP) {
				p->x -= passo*PASSO_POWERUP ;
			//}	
			break ;	
	}	
}

void tiro_congelante (struct bala_t *b) {
	b->congelante = 1 ;
}

void destroi_powerup (struct powerup_t *p) {
	al_destroy_bitmap (p->coracao->coracao_spr) ;
	free (p->coracao) ;
}

// Coração para recuperar vida
