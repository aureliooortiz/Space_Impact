#ifndef __CONTROLE_H__
#define __CONTROLE_H__

struct controle_t {
	int baixo ;
	int cima ;
	int dir ;
	int esq ;
	int tiro ;
} ;

// Cria e retorna o controle
struct controle_t *cria_controle () ;

// Funções que ativam e desativam o controle
void controle_cima (struct controle_t *c) ;
void controle_baixo (struct controle_t *c) ;
void controle_direita (struct controle_t *c) ;
void controle_esquerda (struct controle_t *c) ;
void controle_tiro (struct controle_t *c) ;

// Destroi o controle
void destroi_controle (struct controle_t *c) ;

#endif
