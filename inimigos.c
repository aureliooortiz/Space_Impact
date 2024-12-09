#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "arma.h"
#include "inimigos.h"
#include "utili.h"

// Cria um inimigo que funciona como um nó na lista
struct inimigo_t *cria_inimigo(int x, int y, struct inimigo_t *n) {
	struct inimigo_t *i ;
	
	i = (struct inimigo_t*)malloc(sizeof(struct inimigo_t)) ;
	i->x = x ;
	i->y = y ;
	i->move_timer = 0 ;
	i->vida = 10 ;
	i->pisca = 0 ;
	i->congelado = 0 ;
	i->arma = cria_arma() ;
	i->prox = n ;
	
	return i ;
}

struct boss_t *cria_chefe (char *nomeChefe, int lin, int col,
							char*nomeBalaChefe, int boss) {
	struct boss_t *b ;
	
	b = malloc (sizeof(struct boss_t)) ;	

	b->chefe_spr = al_load_bitmap(nomeChefe) ;
	b->larg_spr = al_get_bitmap_width(b->chefe_spr) / col ;
	b->alt_spr = al_get_bitmap_height(b->chefe_spr) / lin ;
	b->balas_spr = al_load_bitmap(nomeBalaChefe) ;
	b->ataque = 0 ;
	if (!boss) {
		b->ataque_timer = COOLDOWN_BOSS1_ATAQUE ;
	} else {
		b->ataque_timer = COOLDOWN_BOSS2_ATAQUE ;
	}	 
	b->atbt = cria_inimigo (LARG_TELA-b->larg_spr-20, 20, NULL) ;
	if (!boss) {	
		b->atbt->vida = 90 ;
	} else {
		b->atbt->vida = 80 ;
	}
	b->atbt->alt = b->alt_spr ; 
	b->atbt->larg = b->larg_spr ;
	b->ataque = 1 ; 
	
	b->arma1 = cria_arma() ;
	b->arma2 = cria_arma() ;
	b->arma3 = cria_arma() ;
	b->arma4 = cria_arma() ;
	b->arma5 = cria_arma() ;
	
	return b ;
}

// Cria lista que armazenará os inimigos
struct lista_t *cria_lista_inimigos (char *nomeImagem, int lin1, int col1,
									char *nomeBala, int lin2, int col2,
									char *nomeChefe, int lin3, int col3,
									char *nomeBalaChefe, int boss) {
	struct lista_t *l ;
	
	l = (struct lista_t*)malloc(sizeof(struct lista_t)) ;
	l->timer = 0 ;
	l->ini = NULL ;	
	
	l->inimigos_spr = al_load_bitmap(nomeImagem) ;
	l->larg_spr_inimigos = al_get_bitmap_width(l->inimigos_spr) / col1 ;
	l->alt_spr_inimigos = al_get_bitmap_height(l->inimigos_spr) / lin1 ;
	
	if (nomeBala != NULL) {
		l->balas_spr = al_load_bitmap(nomeBala) ;
		l->larg_spr_balas = al_get_bitmap_width(l->balas_spr) / col2 ;
		l->alt_spr_balas = al_get_bitmap_height(l->balas_spr) / lin2 ;
	}
	
	if (nomeChefe != NULL) {
		// Cria o chefão que fica no "final" da lista
		l->chefe = cria_chefe (nomeChefe, lin3, col3, nomeBalaChefe, boss) ;
	}
	
	return l ;
}

void desenha_inimigo (struct lista_t *l, struct inimigo_t *inimigo) {
	ALLEGRO_COLOR azul_escuro ;
	
	azul_escuro = al_map_rgb(0, 0, 139) ;
	if (inimigo->vida > 0) {
		if (!inimigo->pisca) {	
			if (!inimigo->congelado) {		
				al_draw_bitmap_region (l->inimigos_spr, 0, 0, 
				l->larg_spr_inimigos, l->alt_spr_inimigos, inimigo->x, inimigo->y, 0) ;		
			} else {
				al_draw_tinted_bitmap_region (l->inimigos_spr, azul_escuro, 0, 0, 
				l->larg_spr_inimigos, l->alt_spr_inimigos, inimigo->x, inimigo->y, 0) ;		
			}	
		} else {
			inimigo->pisca = 0 ;
		}									
	}
}

void desenha_boss (struct lista_t *l) {
	ALLEGRO_COLOR azul_escuro ;
	
	azul_escuro = al_map_rgb(0, 0, 139) ;	
	if (l->chefe->atbt->vida > 0) {	
		if (!l->chefe->atbt->pisca) {	
			if (!l->chefe->atbt->congelado) {
				al_draw_bitmap (l->chefe->chefe_spr, l->chefe->atbt->x, l->chefe->atbt->y, 0) ;
			} else {
				al_draw_tinted_bitmap (l->chefe->chefe_spr, azul_escuro, l->chefe->atbt->x, l->chefe->atbt->y, 0) ;
			}	
		} else {
			l->chefe->atbt->pisca = 0 ;
		}
	}		
}

void desenha_balas_boss (struct boss_t *c, struct arma_t *arma) {
	for (struct bala_t *b = arma->bala; b != NULL; b = b->prox) {				
		al_draw_bitmap (c->balas_spr, b->x, b->y, 0) ;
	}		
}

void atualiza_balas_boss (struct boss_t *b, struct arma_t *arma) {
	struct bala_t *ant ;
	
	ant = NULL ;
	if (b->atbt->vida > 0) {	
		// Percorre por cada bala da arma
		for (struct bala_t *atual = arma->bala; atual != NULL;) {
			// Atualiza posição da bala
			atual->x -= MOVIMENTO_BALA ;
			
			// Verifica se o tiro saiu da tela 
			if (atual->x < 0) {
				// Caso seja o primeiro tiro da lista
				if (!ant) {
					// Aponta para o proximo e destroi a bala atual
					arma->bala = (struct bala_t*) atual->prox ;
					atual = arma->bala ;
				} else {
					// Aponta para o proximo e destroi a bala atual
					ant->prox = atual->prox ;
					atual = ant->prox ;
				}
				destroi_bala(atual) ;
				
			} else {
				ant = atual ;
				atual = (struct bala_t*)atual->prox ;
			}
		
		}
	}		
}

void boss_atira (struct arma_t *arma, int x, int y) {
	struct bala_t *tiro ;
	
	tiro = tiro_de_arma(x, y, arma) ;
	
	arma->bala = tiro ;
}

// Função de movimentação 
void movimenta_inimigo (struct inimigo_t *i, int passo, int trajetoria) {
	switch (trajetoria) {
		case BAIXO:
			if ((i->y+i->larg + PASSO_INIMIGO) < ALT_TELA) {
				i->y += passo*PASSO_INIMIGO ;
			}	
			break ;
		case CIMA:
			if ((i->y - PASSO_INIMIGO) > 0) {
				i->y -= passo*PASSO_INIMIGO ;
			}	
			break ;
		case DIREITA:
			if ((i->x-i->larg + PASSO_INIMIGO) < LARG_TELA) {	
				i->x += passo*PASSO_INIMIGO ;
			}	
			break ;		
		case ESQUERDA:
			i->x -= passo*PASSO_INIMIGO ;
			break ;	
	}
}

void inimigo_perde_vida(struct inimigo_t *i) {
	i->vida-- ;
	i->pisca = 1 ;
}

void verifica_congelado (struct inimigo_t *inimigo) {
	if (inimigo->congelado == TEMPO_CONGELADO) {
		inimigo->congelado = 0 ;
	} else if (inimigo->congelado > 0) {
		inimigo->congelado++ ;
	}
}

// Verifica cada inimigo se saiu da tela ou morreu, se sim o destroi
void verifica_inimigos (struct lista_t *l) {
	struct inimigo_t *ant ;
	struct inimigo_t *aux ;
	
	ant = NULL ;
	for (struct inimigo_t *atual = l->ini; atual != NULL;) {	
		// Verifica se o inimigo saiu da tela ou morreu
		if (atual->x < 0 || !atual->vida) {
			// Caso seja o primeiro inimigo da lista
			if (!ant) {
				// Aponta para o proximo e destroi o inimigo atual
				l->ini = (struct inimigo_t*) atual->prox ;
			} else {
				// Aponta para o proximo e destroi a bala atual
				ant->prox = atual->prox ;
			}
			aux = atual ;
			atual = (struct inimigo_t*)atual->prox ;
			destroi_inimigo(aux) ;
					
		} else {
			ant = atual ;
			atual = (struct inimigo_t*)atual->prox ;
		}
	}	
}

// Atualiza posição dos tiros inimigos em tela e os destroi se precisar
void atualiza_balas_inimigas (struct lista_t *l) {
	struct bala_t *ant ;
	
	// Percorre cada inimigo
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {	
		ant = NULL ;
		if (i->vida > 0) {	
			// Percorre por cada bala
			for (struct bala_t *atual = i->arma->bala; atual != NULL;) {
				// Atualiza posição da bala
				atual->x -= MOVIMENTO_BALA ;
				
				// Verifica se o tiro saiu da tela 
				if (atual->x < 0) {
					// Caso seja o primeiro tiro da lista
					if (!ant) {
						// Aponta para o proximo e destroi a bala atual
						i->arma->bala = (struct bala_t*) atual->prox ;
						atual = i->arma->bala ;
					} else {
						// Aponta para o proximo e destroi a bala atual
						ant->prox = atual->prox ;
						atual = ant->prox ;
					}
					destroi_bala(atual) ;
					
				} else {
					ant = atual ;
					atual = (struct bala_t*)atual->prox ;
				}
			
			}
		}	
	}	
}

void desenha_balas_inimigas (struct lista_t *l_atira) {
	// Se a lista foi inicializada desenha cada bala de cada inimigo	
	if (l_atira->ini != NULL) {
		atualiza_balas_inimigas (l_atira) ;
		// Percorre cada inimigo
		for (struct inimigo_t *i = l_atira->ini; i != NULL; i = i->prox) {	
			// Desenha as balas inimigas
			for (struct bala_t *b = i->arma->bala; b != NULL; b = b->prox) {				
				al_draw_bitmap (l_atira->balas_spr, b->x, b->y, 0) ;
			}		
		}	
	}
}	

// Inimigo dispara projétil
void inimigo_atira(struct inimigo_t *i) {
	struct bala_t *tiro ;
	
	// Tiro sempre parte de um deslocamento fixo à direita do centro, saindo do meio
	tiro = tiro_de_arma(i->x + i->larg/2, i->y + i->alt/2, i->arma) ;
	
	i->arma->bala = tiro ;
}	

void destroi_todos_inimigos (struct lista_t *l) {
	struct inimigo_t *aux ;
	
	aux = l->ini ;
	while (aux != NULL) {
		l->ini = l->ini->prox ;
		free (aux) ;
		aux = l->ini ;
	}
}

// libera memoria do inimigo
void destroi_inimigo(struct inimigo_t *i) {
	/* Destroi a lista de balas e arma
	aux = l->ini ;
	while (aux != NULL) {
		l->ini = l->ini->prox ;
		destroi_inimigo(aux) ;
		aux = l->ini ;
	}*/
	free(i) ;
	
}

// Destroi toda memória alocada da lista de inimigos
void destroi_lista(struct lista_t *l) {
	struct inimigo_t *aux ;
	
	aux = l->ini ;
	while (aux != NULL) {
		l->ini = l->ini->prox ;
		destroi_inimigo(aux) ;
		aux = l->ini ;
	}
	free (l) ;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
