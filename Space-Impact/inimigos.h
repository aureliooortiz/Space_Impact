#ifndef __INIMIGOS_H__
#define __INIMIGOS_H__

#define LADO_INIMIGO 50

// Velocidade que anda o inimigo
#define PASSO_INIMIGO 7

// Cooldown de nascimento 
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
	int congelado ;
	// Cooldown para o inimigo se mover a uma direção aleatoria
	int move_timer ;
	int vida ;
	// Indica se o inimigo levou dano
	int pisca ;
	struct inimigo_t *prox ;
	struct arma_t *arma ;
} ;

// Cria um inimigo que funciona como um nó na lista
struct inimigo_t *cria_inimigo(int x, int y, struct inimigo_t *n) ;

// Cria lista que armazenaraá os inimigos
struct lista_t *cria_lista_inimigos () ;

// Função de movimentação 
void movimenta_inimigo (struct inimigo_t *i, int passo, int trajetoria) ;

void inimigo_perde_vida(struct inimigo_t *i) ;

// Inimigo dispara projétil
void inimigo_atira(struct inimigo_t *i) ;

// libera memoria do inimigo
void destroi_inimigo(struct inimigo_t *i) ;

// Destroi toda memória alocada da lista de inimigos
void destroi_lista(struct lista_t *i) ;

#endif
