#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "utili.h"

// Cria e retorna um player
struct jogador_t *cria_jogador (int x, int y) {
	struct jogador_t *p ;
	
	p = (struct jogador_t*)malloc(sizeof(struct jogador_t)) ;
	p->controle = cria_controle() ;
	p->arma = cria_arma() ;
	p->x = x ;
	p->y = y ;
	p->gelo_timer = 0 ;  
	// Vida máxima do jogador
	p->vida = 3 ;
	p->invulnerabilidade = 0 ;
	
	return p ;
}

// Verifica e movimenta o player
void movimenta_jogador (struct jogador_t *p, int passo, int trajetoria) {
	switch (trajetoria) {
		case BAIXO:
			if ((p->y + PASSO_QUADRADO) < ALT_TELA) {
				p->y += passo*PASSO_QUADRADO ;
			}	
			break ;
		case CIMA:
			if ((p->y - PASSO_QUADRADO) > 0) {
				p->y -= passo*PASSO_QUADRADO ;
			}	
			break ;
		case DIREITA:
			if ((p->x + PASSO_QUADRADO) < LARG_TELA) {	
				p->x += passo*PASSO_QUADRADO ;
			}	
			break ;		
		case ESQUERDA:
			if ((p->x - PASSO_QUADRADO) > 0) {	
				p->x -= passo*PASSO_QUADRADO ;
			}	
			break ;	
	}
}

void jogador_atira (struct jogador_t *p) {
	struct bala_t *tiro ;
	
	// Tiro sempre parte de um deslocamento fixo à direita do centro, saindo do meio
	tiro = tiro_de_arma(p->x + LADO_QUADRADO/2, p->y + LADO_QUADRADO/2, p->arma) ;
	
	// Se o timer estiver ativo signifca que o player pegou o tiro congelante
	if (p->gelo_timer) {
		tiro_congelante (tiro) ;
	}
	
	p->arma->bala = tiro ;
}

void jogador_perde_vida (struct jogador_t *p) {
	p->vida-- ;
	p->invulnerabilidade = DANO_COOLDOWN ;
}

// Destroi um player
void destroi_jogador (struct jogador_t *p) {
	destroi_arma(p->arma) ;
	destroi_controle (p->controle) ;
	free(p) ;
}

