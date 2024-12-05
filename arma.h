#ifndef __ARMA_H__
#define __ARMA_H__

#include "bala.h"

// A cada 10 frames é possivel atirar novamente
#define ARMA_COOLDOWN 10

#define INIMIGO_ARMA_COOLDOWN 30

struct arma_t {
	struct bala_t *bala ;
	int timer ;
} ;

struct arma_t *cria_arma() ;

struct bala_t *tiro_de_arma (int x , int y, struct arma_t *a) ;

void destroi_arma(struct arma_t *a) ;

#endif
