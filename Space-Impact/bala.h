#ifndef __BALA_H__
#define __BALA_H__

#define MOVIMENTO_BALA 10

struct bala_t {
	struct bala_t *prox ;
	// Posição atual da bala
	int x ;
	int y ;
} ;

// Cria e retorna uma bala, funciona como um nodo de lista
struct bala_t *cria_bala (int x, int y, struct bala_t *n) ;

void destroi_bala (struct bala_t *b) ;

#endif
