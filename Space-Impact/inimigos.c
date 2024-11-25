#include <stdlib.h>

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

// Funções de movimentação 
void inimigo_esquerda (struct inimigo_t *i) {
	i->x -= PASSO_INIMIGO ;
}
void inimigo_direita(struct inimigo_t *i) {
	if ((i->x + PASSO_INIMIGO) < LARG_TELA) {
		i->x += PASSO_INIMIGO ;
	}	
}
void inimigo_cima(struct inimigo_t *i) {
	if ((i->y - PASSO_INIMIGO) > 0) {
		i->y -= PASSO_INIMIGO ;
	}	
}
void inimigo_baixo(struct inimigo_t *i) {
	if ((i->y + PASSO_INIMIGO) < ALT_TELA) {
		i->y += PASSO_INIMIGO ;
	}	
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
	free(i) ;
}

// Destroi toda memória alocada da lista de inimigos
void destroi_lista(struct lista_t *l) {
	struct inimigo_t *aux ;
	
	destroi_arma(l->ini->arma) ;
	
	aux = l->ini ;
	while (aux != NULL) {
		l->ini = l->ini->prox ;
		destroi_inimigo(aux) ;
		aux = l->ini ;
	}
	free (l) ;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
