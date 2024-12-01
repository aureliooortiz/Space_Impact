#include <stdlib.h>
#include "colisao.h"

// Colisão de player para inimigo
int colisao_player_inimigo(struct jogador_t *p, struct lista_t *l) {
	// Verifica cada inimigo procurando colisão com player
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {
		if ((((i->y-i->alt/2 >= p->y-p->alt/2) && (p->y+p->alt/2 >= i->y-i->alt/2)) 
			||
			((p->y-p->alt/2 >= i->y-i->alt/2) && (i->y+i->alt/2 >= p->y-p->alt/2)))
			&&
			(((i->x-i->larg/2 >= p->x-p->larg/2) && (p->x+p->larg/2 >= i->x-i->larg/2))
			|| 
			((p->x-p->larg/2 >= i->x-i->larg/2) && (i->x+i->larg/2 >= p->x-p->larg/2)))
			) {
			return 1 ;
		}
	}
	return 0 ;
}

// Colisão de inimigo para player, para quando o player estiver parado

// Colisão de inimigo para inimigo
int colisao_inimigo_inimigo (struct inimigo_t *inimigo, struct lista_t *l) {
	// Verifica cada inimigo na lista procurando colisão com um inimigo em especifico
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {
		if ((((i->y-i->alt/2 >= inimigo->y-inimigo->alt/2) && (inimigo->y+inimigo->alt/2 >= i->y-i->alt/2)) 
			||
			((inimigo->y-inimigo->alt/2 >= i->y-i->alt/2) && (i->y+i->alt/2 >= inimigo->y-inimigo->alt/2)))
			&&
			(((i->x-i->larg/2 >= inimigo->x-inimigo->larg/2) && (inimigo->x+inimigo->larg/2 >= i->x-i->larg/2))
			|| 
			((inimigo->x-inimigo->alt/2 >= i->x-i->larg/2) && (i->x+i->larg/2 >= inimigo->x-inimigo->larg/2)))
			) {
			return 1 ;
		}
	}
	return 0 ;
}

// Colisão de tiro para inimigo
int colisao_inimigo_bala (struct jogador_t *p, struct inimigo_t *i) {
	for (struct bala_t *b = p->arma->bala; b != NULL; b = b->prox) {
		// Verifica se a bala está dentro da área do inimigo
		if ((b->x >= i->x) && (b->x <= i->x + i->alt) &&
		    (b->y >= i->y) && (b->y <= i->y + i->alt)) {
			b->acertou = 1 ;
			// Verifica se foi um tiro congelante
			if (b->congelante) {	
				return 2 ;
			}
			return 1 ;
		}
	}
	return 0 ;
}

// Colisão de player para tiro
int colisao_player_bala (struct jogador_t *p, struct lista_t *l) { 
	// Verifica cada bala procurando colisão com player
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {	
		for (struct bala_t *b = i->arma->bala; b != NULL; b = b->prox) {
			// Verifica se a bala está dentro da área do jogador
			if ((b->x >= p->x) && (b->x <= p->x + p->larg) &&
			    (b->y >= p->y) && (b->y <= p->y + p->alt)) {
				return 1 ;
			}
		}
	}	
	return 0 ;	
}

// Verifica colisao com powerup
int colisao_player_powerup(struct jogador_t *p, struct powerup_t power) {
	if ((((power.y-power.alt_power/2 >= p->y-p->alt/2) && (p->y+p->alt/2 >= power.y-power.alt_power/2)) 
		||
		((p->y-p->alt/2 >= power.y-power.alt_power/2) && (power.y+power.alt_power/2 >= p->y-p->alt/2)))
		&&
		(((power.x-power.larg_power/2 >= p->x-p->larg/2) && (p->x+p->larg/2 >= power.x-power.larg_power/2))
		|| 
		((p->x-p->larg/2 >= power.x-power.larg_power/2) && (power.x+power.larg_power/2 >= p->x-p->larg/2)))
		) {
		return 1 ;
	}
	return 0 ;
}

