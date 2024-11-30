#include <stdlib.h>

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
	i->vida = 30 ;
	i->pisca = 0 ;
	i->congelado = 0 ;
	i->arma = cria_arma() ;
	i->prox = n ;
	
	return i ;
}

// Cria lista que armazenará os inimigos
struct lista_t *cria_lista_inimigos () {
	struct lista_t *l ;
	
	l = (struct lista_t*)malloc(sizeof(struct lista_t)) ;
	l->timer = 0 ;
	l->ini = NULL ;	
	
	return l ;
}

// Função de movimentação 
void movimenta_inimigo (struct inimigo_t *i, int passo, int trajetoria) {
	switch (trajetoria) {
		case BAIXO:
			if ((i->y + PASSO_INIMIGO) < ALT_TELA) {
				i->y += passo*PASSO_INIMIGO ;
			}	
			break ;
		case CIMA:
			if ((i->y - PASSO_INIMIGO) > 0) {
				i->y -= passo*PASSO_INIMIGO ;
			}	
			break ;
		case DIREITA:
			if ((i->x + PASSO_INIMIGO) < LARG_TELA) {	
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

// Inimigo dispara projétil
void inimigo_atira(struct inimigo_t *i) {
	struct bala_t *tiro ;
	
	// Tiro sempre parte de um deslocamento fixo à direita do centro, saindo do meio
	tiro = tiro_de_arma(i->x + LADO_INIMIGO/2, i->y + LADO_INIMIGO/2, i->arma) ;
	
	i->arma->bala = tiro ;
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
