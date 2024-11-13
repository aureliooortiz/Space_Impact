#include "player.h"

struct jogador_t *cria_jogador (int x, int y) {
	struct jogador_t *p ;
	
	p = (struct jogador_t*)malloc(sizeof(struct jogador_t)) ;
	
	p->x = x ;
	p->y = y ;
	
	return p ;
}
