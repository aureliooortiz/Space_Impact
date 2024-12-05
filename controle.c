#include <stdlib.h>

#include "controle.h"

// Cria e retorna o controle
struct controle_t *cria_controle () {
	struct controle_t *c ;
	
	c = (struct controle_t*)malloc(sizeof(struct controle_t)) ;
	
	c->cima = 0 ;
	c->baixo = 0 ;
	c->dir = 0 ;
	c->esq = 0 ;
	c->tiro = 0 ;
	
	return c ;
}

// Funções que ativam e desativam o controle através de XOR
void controle_cima (struct controle_t *c) {
	c->cima = 1 ;
}

void controle_baixo (struct controle_t *c) {
	c->baixo = 1 ;
}

void controle_direita (struct controle_t *c) {
	c->dir = 1 ;
}

void controle_esquerda (struct controle_t *c) {
	c->esq = 1 ;
}

void controle_tiro (struct controle_t *c) {
	c->tiro = 1 ;
}

// Destroi o controle
void destroi_controle (struct controle_t *c) {
	free(c) ;
}
