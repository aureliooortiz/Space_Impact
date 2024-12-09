#ifndef __INIMIGOS_H__
#define __INIMIGOS_H__

#include <allegro5/allegro.h>

//#define LADO_INIMIGO 50
#define LARG_INIMIGO 50 
#define ALT_INIMIGO 50

// Velocidade que anda o inimigo
#define PASSO_INIMIGO 7

// Cooldown de nascimento 
#define INIMIGO_COOLDOWN_1 100
#define INIMIGO_COOLDOWN_2 70

#define COOLDOWN_MOVE 50
#define COOLDOWN_BOSS_MOVE 30

#define COOLDOWN_BOSS_INVOCA 30
#define COOLDOWN_BOSS1_ATAQUE 450
#define COOLDOWN_BOSS2_ATAQUE 300
#define ATAQUE 600

struct inimigo_t {
	int x ;
	int y ;
	int alt ;
	int larg ;
	int congelado ;
	// Cooldown para o inimigo se mover a uma direção aleatoria
	int move_timer ;
	int vida ;
	// Indica se o inimigo levou dano
	int pisca ;
	struct inimigo_t *prox ;
	struct arma_t *arma ;
} ;

struct boss_t {
	// Atributos do boss
	struct inimigo_t *atbt ;
	struct arma_t *arma1 ;
	struct arma_t *arma2 ;
	struct arma_t *arma3 ;
	struct arma_t *arma4 ;
	struct arma_t *arma5 ;
	ALLEGRO_BITMAP *chefe_spr ;
	ALLEGRO_BITMAP *balas_spr ;
	float larg_spr ;
	float alt_spr ;
	int ataque ;
	int ataque_timer ;
} ;

struct lista_t {
	struct inimigo_t *ini ;
	struct boss_t *chefe ;
	// Sprites daquela lista de inimigos
	ALLEGRO_BITMAP *inimigos_spr ;
	ALLEGRO_BITMAP *balas_spr ;
	int timer ;
	float larg_spr_inimigos ;
	float alt_spr_inimigos ;
	float larg_spr_balas ;
	float alt_spr_balas ;
} ;

struct boss_t *cria_chefe (char *nomeChefe, int lin, int col,
							char*nomeBalaChefe, int boss) ;

// Cria um inimigo que funciona como um nó na lista
struct inimigo_t *cria_inimigo(int x, int y, struct inimigo_t *n) ;

// Cria lista que armazenaraá os inimigos
struct lista_t *cria_lista_inimigos (char *nomeImagem, int ind_lin, int ind_col,
									char *nomeBala, int lin2, int col2,
									char *nomeChefe, int lin3, int col3,
									char *nomeBalaChefe, int boss) ;

void desenha_inimigo (struct lista_t *l, struct inimigo_t *inimigo) ;

void desenha_boss (struct lista_t *l) ;

void desenha_balas_boss (struct boss_t *b, struct arma_t *arma) ;

void atualiza_balas_boss (struct boss_t *b, struct arma_t *arma) ;

void boss_atira (struct arma_t *arma, int x, int y) ;

void verifica_congelado (struct inimigo_t *inimigo) ;

// Função de movimentação 
void movimenta_inimigo (struct inimigo_t *i, int passo, int trajetoria) ;

void inimigo_perde_vida(struct inimigo_t *i) ;

void verifica_congelado (struct inimigo_t *inimigo) ;

// Verifica cada inimigo se saiu da tela ou morreu, se sim o destroi
void verifica_inimigos (struct lista_t *l) ;

// Atualiza posição dos tiros inimigos em tela e os destroi se precisar
void atualiza_balas_inimigas (struct lista_t *l) ;

void desenha_balas_inimigas (struct lista_t *l_atira) ;

// Inimigo dispara projétil
void inimigo_atira(struct inimigo_t *i) ;

void destroi_todos_inimigos (struct lista_t *l) ;

// libera memoria do inimigo
void destroi_inimigo(struct inimigo_t *i) ;

// Destroi toda memória alocada da lista de inimigos
void destroi_lista(struct lista_t *i) ;

#endif
