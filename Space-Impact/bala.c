#include <stdlib.h>

#include "bala.h"

// Cria e retorna uma bala, funciona como um nodo de lista
struct bala_t *cria_bala (int x, int y, struct bala_t *n) {
	struct bala_t *b ;
	
	b = (struct bala_t*)malloc (sizeof(struct bala_t)) ;
	b->x = x ;
	b->y = y ;
	b->prox = (struct bala_t*)n ;
	
	return b ;
}

void destroi_bala (struct bala_t *b) {
	free (b) ;
}

