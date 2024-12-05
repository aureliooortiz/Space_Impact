#include "powerup.h"

struct powerup_t cria_powerup (int x, int y, int tipo) {
	struct powerup_t p ;
	
	p.x = x ;
	p.y = y ;
	p.tipo = tipo ;
	p.coletado = 0 ;
	p.larg_power = LARG_POWERUP ;
	p.alt_power = ALT_POWERUP ;
	p.powerup_timer = POWERUP_COOLDOWN ;
	
	return p ;
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
			if ((p->x - PASSO_POWERUP) > -LARG_POWERUP) {
				p->x -= passo*PASSO_POWERUP ;
			}	
			break ;	
	}	
}

void tiro_congelante (struct bala_t *b) {
	b->congelante = 1 ;
}



// Coração para recuperar vida
