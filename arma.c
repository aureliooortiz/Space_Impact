#include <stdlib.h>

#include "arma.h"

struct arma_t *cria_arma() {
	struct arma_t *a ;
	
	a = (struct arma_t*)malloc(sizeof(struct arma_t)) ;
	a->timer = 0 ;
	a->bala = NULL ;
	
	return a ;
}

struct bala_t *tiro_de_arma (int x , int y, struct arma_t *a) {
	struct bala_t *bala ;
	
	bala = cria_bala (x, y, a->bala) ;
	
	return bala ;
}

void destroi_arma(struct arma_t *a) {
	free (a) ;
}
