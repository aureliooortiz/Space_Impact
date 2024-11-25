#include <stdlib.h>
#include "colisao.h"

// Colisão de player para inimigo
int colisao_player_inimigo(struct jogador_t *p, struct lista_t *l) {
	// Verifica cada inimigo procurando colisão com player
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {
		if ((((i->y-LADO_INIMIGO/2 >= p->y-LADO_QUADRADO/2) && (p->y+LADO_QUADRADO/2 >= i->y-LADO_INIMIGO/2)) 
			||
			((p->y-LADO_QUADRADO/2 >= i->y-LADO_INIMIGO/2) && (i->y+LADO_INIMIGO/2 >= p->y-LADO_QUADRADO/2)))
			&&
			(((i->x-LADO_INIMIGO/2 >= p->x-LADO_QUADRADO/2) && (p->x+LADO_QUADRADO/2 >= i->x-LADO_INIMIGO/2))
			|| 
			((p->x-LADO_QUADRADO/2 >= i->x-LADO_INIMIGO/2) && (i->x+LADO_INIMIGO/2 >= p->x-LADO_QUADRADO/2)))
			) {
			return 1 ;
		}
	}
	return 0 ;
}

// Colisão de inimigo para player, para quando o player estiver parado

// Colisão de inimigo para inimigo

// Colisão de tiro para inimigo

// Colisão de player para tiro
