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
	p->larg = LARG_NAVE ;
	p->alt = ALT_NAVE ;
	p->gelo_timer = 0 ;  
	// Vida máxima do jogador
	p->vida = 3 ;
	p->invulnerabilidade = 0 ;
	
	return p ;
}

void desenha_jogador (struct jogador_t *player) {
	// Player pisca caso esteja invulneravel
	if (!player->invulnerabilidade || ((player->invulnerabilidade % 2) == 0)) {
		// Desenha o jogador
		al_draw_bitmap_region(player->player_spr, 0, 2 * player->alt_spr_player, 
								player->larg_spr_player, player->alt_spr_player, player->x, player->y, 0) ;
	}
}

void desenha_balas_jogador (struct jogador_t *player) {
	ALLEGRO_COLOR azul_claro ;
	
	azul_claro = al_map_rgb(173, 216, 230) ;
	// Desenha os tiros do player na tela
	for (struct bala_t *b = player->arma->bala; b != NULL; b = b->prox) {
		if (b->congelante) {	
			al_draw_tinted_bitmap (player->balas_spr, azul_claro, b->x, b->y, 0) ;
		} else {
			al_draw_bitmap (player->balas_spr, b->x, b->y, 0) ;
		}	
	}
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
	tiro = tiro_de_arma(p->x + p->larg/2, p->y + p->alt/2, p->arma) ;
	
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


// Atualiza posição dos tiros do jogador em tela e os destroi se precisar
void atualiza_balas (struct jogador_t *p) {
	struct bala_t *ant ;
	
	ant = NULL ;
	// Percorre por cada bala
	for (struct bala_t *atual = p->arma->bala; atual != NULL;) {
		// Atualiza posição da bala
		atual->x += MOVIMENTO_BALA ;
		
		// Verifica se o tiro saiu da tela ou acertou um inimigo
		if (atual->x > LARG_TELA || atual->acertou) {
			// Caso seja o primeiro tiro da lista
			if (!ant) {
				// Aponta para o proximo e destroi a bala atual
				p->arma->bala = (struct bala_t*) atual->prox ;
				destroi_bala(atual) ;
				atual = p->arma->bala ;
			} else {
				// Aponta para o proximo e destroi a bala atual
				ant->prox = atual->prox ;
				destroi_bala(atual) ;
				atual = ant->prox ;
			}
		} else {
			ant = atual ;
			atual = (struct bala_t*)atual->prox ;
		}
	
	}
}

// Destroi um player
void destroi_jogador (struct jogador_t *p) {
	destroi_arma(p->arma) ;
	destroi_controle (p->controle) ;
	free(p) ;
}

