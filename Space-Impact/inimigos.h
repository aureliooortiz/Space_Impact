#ifndef __INIMIGOS_H__
#define __INIMIGOS_H__

#define LADO_INIMIGO 50

// Velocidade que anda o inimigo
#define PASSO_INIMIGO 7

#define INIMIGO_COOLDOWN_1 100
#define INIMIGO_COOLDOWN_2 70

#define COOLDOWN_MOVE 50

struct lista_t {
	int timer ;
	struct inimigo_t *ini ;
} ;

struct inimigo_t {
	int x ;
	int y ;
	int move_timer ;
	struct inimigo_t *prox ;
	struct arma_t *arma ;
} ;

// Cria um inimigo que funciona como um nó na lista
struct inimigo_t *cria_inimigo(int x, int y, struct inimigo_t *n) ;

// Cria lista que armazenaraá os inimigos
struct lista_t *cria_lista_inimigos () ;

// Funções de movimentação 
void inimigo_esquerda (struct inimigo_t *i) ;
void inimigo_direita(struct inimigo_t *i) ; 
void inimigo_cima(struct inimigo_t *i) ;
void inimigo_baixo(struct inimigo_t *i) ;

// Inimigo dispara projétil
void inimigo_atira(struct inimigo_t *i) ;

// libera memoria do inimigo
void destroi_inimigo(struct inimigo_t *i) ;

// Destroi toda memória alocada da lista de inimigos
void destroi_lista(struct lista_t *i) ;

#endif
