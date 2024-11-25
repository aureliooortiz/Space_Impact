// Compilar: 
// gcc main.c -o main -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdlib.h> 
#include <stdio.h> 

//#include "player.h"
//#include "inimigos.h"
#include "utili.h"
#include "colisao.h"

void nasce_inimigo (struct lista_t *l, ALLEGRO_COLOR cor) {
				
	// Inimigo nasce no lado esquerdo da tela em um y aleatório
	l->ini = cria_inimigo(LARG_TELA - LADO_INIMIGO, aleat(0, ALT_TELA-LADO_INIMIGO) , l->ini) ;
				
	// Desenha primeira "aparição" do inimigo em tela, ele nasce
	al_draw_filled_rectangle(l->ini->x, l->ini->y, l->ini->x + LADO_INIMIGO, l->ini->y + LADO_INIMIGO, cor) ;
}

// Verifica cada inimigo se saiu da tela, se sim o destroi
void verifica_inimigos (struct lista_t *l) {
	struct inimigo_t *ant ;
	struct inimigo_t *aux ;
	
	ant = NULL ;
	for (struct inimigo_t *atual = l->ini; atual != NULL;) {	
		// Verifica se o inimigo saiu da tela
		if (atual->x < 0) {
			// Caso seja o primeiro inimigo da lista
			if (!ant) {
				// Aponta para o proximo e destroi o inimigo atual
				l->ini = (struct inimigo_t*) atual->prox ;
			} else {
				// Aponta para o proximo e destroi a bala atual
				ant->prox = atual->prox ;
			}
			aux = atual ;
			atual = (struct inimigo_t*)atual->prox ;
			destroi_inimigo(aux) ;
					
		} else {
			ant = atual ;
			atual = (struct inimigo_t*)atual->prox ;
		}
	}	
}

// Atualiza posição dos tiros inimigos em tela e os destroi se precisar
void atualiza_balas_inimigas (struct lista_t *l) {
	struct bala_t *ant ;
	
	// Percorre cada inimigo
	for (struct inimigo_t *i = l->ini; i != NULL; i = i->prox) {	
		ant = NULL ;
		// Percorre por cada bala
		for (struct bala_t *atual = i->arma->bala; atual != NULL;) {
			// Atualiza posição da bala
			atual->x -= MOVIMENTO_BALA ;
			
			// Verifica se o tiro saiu da tela
			if (atual->x < 0) {
				// Caso seja o primeiro tiro da lista
				if (!ant) {
					// Aponta para o proximo e destroi a bala atual
					i->arma->bala = (struct bala_t*) atual->prox ;
					atual = i->arma->bala ;
				} else {
					// Aponta para o proximo e destroi a bala atual
					ant->prox = atual->prox ;
					atual = ant->prox ;
				}
				destroi_bala(atual) ;
				
			} else {
				ant = atual ;
				atual = (struct bala_t*)atual->prox ;
			}
		
		}
	}	
}

// Atualiza posição dos tiros em tela e os destroi se precisar
void atualiza_balas (struct jogador_t *p) {
	struct bala_t *ant ;
	
	ant = NULL ;
	// Percorre por cada bala
	for (struct bala_t *atual = p->arma->bala; atual != NULL;) {
		// Atualiza posição da bala
		atual->x += MOVIMENTO_BALA ;
		
		// Verifica se o tiro saiu da tela
		if (atual->x > LARG_TELA) {
			// Caso seja o primeiro tiro da lista
			if (!ant) {
				// Aponta para o proximo e destroi a bala atual
				p->arma->bala = (struct bala_t*) atual->prox ;
				destroi_bala(atual) ;
				atual = p->arma->bala ;
			} else {
				// Aponta para o proximo e destroi a bala atual
				ant->prox = atual->prox ;
				destroi_bala(atual) ;
				atual = ant->prox ;
			}
		} else {
			ant = atual ;
			atual = (struct bala_t*)atual->prox ;
		}
	
	}
}

// Atualiza a posição do jogador e dos tiros disparados
void atualiza_pos (struct jogador_t *p, struct lista_t *l1, struct lista_t *l2) {
	// Verifica cada controle se estiver ativado movimenta jogador
	if (p->controle->cima) {
		movimenta_jogador (p, 1, CIMA) ;
		if (!p->invulnerabilidade) {	
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)) {
				jogador_perde_vida(p) ; 
			}	
		}		
	}
	if (p->controle->baixo) {
		movimenta_jogador (p, 1, BAIXO) ;
		if (!p->invulnerabilidade) {		
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)) {
				jogador_perde_vida(p) ; 
			}	
		}	
	}	
	if (p->controle->dir) {
		movimenta_jogador (p, 1, DIREITA) ;
		if (!p->invulnerabilidade) {	
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)) {
				jogador_perde_vida(p) ;
			}	
		}	
	}	
	if (p->controle->esq) {
		movimenta_jogador (p, 1, ESQUERDA) ;
		if (!p->invulnerabilidade) {		
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)) {
				jogador_perde_vida(p) ;
			}	
		}		
	}		
	// Só atira se o cooldown estiver zerado
	if (p->controle->tiro && !p->arma->timer) {
		// Cria o tiro e armazena
		jogador_atira (p) ;
		p->arma->timer = ARMA_COOLDOWN ;
	}
	atualiza_balas (p) ;
}

void desenha_vida(struct jogador_t *p, ALLEGRO_COLOR vermelho) {
	if (p->vida == 3) {
		al_draw_filled_rectangle(0+10, (ALT_TELA - 20)-10, 10+20, ALT_TELA-10, vermelho) ;
		al_draw_filled_rectangle(0+40, (ALT_TELA - 20)-10, 40+20, ALT_TELA-10, vermelho) ;
		al_draw_filled_rectangle(0+70, (ALT_TELA - 20)-10, 70+20, ALT_TELA-10, vermelho) ;
	} else if (p->vida == 2 ) {
		al_draw_filled_rectangle(0+10, (ALT_TELA - 20)-10, 20+10, ALT_TELA-10, vermelho) ;
		al_draw_filled_rectangle(0+20, (ALT_TELA - 20)-10, 20+30, ALT_TELA-10, vermelho) ;
	} else if (p->vida == 1) {
		al_draw_filled_rectangle(0+10, (ALT_TELA - 20)-10, 20+10, ALT_TELA-10, vermelho) ;
	}
}

int main() {
	ALLEGRO_DISPLAY *janela ;
	ALLEGRO_COLOR azul, vermelho, verde ;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos ;
	ALLEGRO_EVENT eventos ;
	ALLEGRO_TIMER *tempo ;
	struct jogador_t *player ;
	struct lista_t *l_atira ;
	struct lista_t *l ;
	
	srand(0) ;
	
	// Inicializa váriaveis de cor
	azul = al_map_rgb(0, 0, 255) ;
	vermelho = al_map_rgb(255, 0, 0) ;
	verde = al_map_rgb(0, 255, 0) ;
	
	// Inicializa e instala tudo que será utilizado no programa
	al_init() ;
	al_init_primitives_addon() ;
	al_install_keyboard() ;
		
	// Cria e inicializa a janela
	janela = al_create_display(LARG_TELA, ALT_TELA) ;
	al_set_window_position(janela, POS_TELA+100, POS_TELA) ;
	al_set_window_title(janela, NOME_TELA) ;
	//al_set_new_display_flags(ALLEGRO_RESIZABLE) ;
	
	// Cria a taxa de atualização do jogo FPS
	tempo = al_create_timer(1.0 / 30.0) ;
	
	// Cria e inicializa os tipos de eventos na fila
	fila_de_eventos = al_create_event_queue() ;
	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source()) ;
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(tempo)) ;
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	
	player = cria_jogador (LADO_QUADRADO, LADO_QUADRADO) ;
	
	l = cria_lista_inimigos() ;	
	l_atira = cria_lista_inimigos() ;	
	
	al_start_timer(tempo) ;		
	// Loop principal do jogo
	while (1) {
		al_wait_for_event(fila_de_eventos, &eventos) ;
		
		// Evento de clique no "x" da janela
		if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break ;	
		
		// Evento que atualiza a tela
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0)) ;
			
			// Desenha coração na tela
			desenha_vida(player, vermelho) ;
			
			// Verifica os inimigos que possam ter saído da tela
			verifica_inimigos(l_atira) ;
			verifica_inimigos(l) ;
			
			if (!l->timer) {
				nasce_inimigo(l, vermelho) ;
				l->timer = INIMIGO_COOLDOWN_1 ;
			} else if (l_atira->timer <= 0) {
				nasce_inimigo(l_atira, verde) ;
				l_atira->timer = INIMIGO_COOLDOWN_2 ;
			}
			l->timer-- ;
			l_atira->timer-- ;
			
			// Desenha todos inimigos na tela que apenas se movimentam
			for (struct inimigo_t *inimigo = l->ini; inimigo != NULL; inimigo = inimigo->prox) {
				// Este inimigo se move apenas para a esquerda
				inimigo_esquerda(inimigo) ;
				//colisao_inimigo_inimigo() ;
				
				al_draw_filled_rectangle(inimigo->x, inimigo->y, inimigo->x + LADO_INIMIGO, inimigo->y + LADO_INIMIGO, vermelho) ;
			}
				
			// Desenha e movimenta todos inimigos na tela que podem disparar balas
			for (struct inimigo_t *inimigo = l_atira->ini; inimigo != NULL; inimigo = inimigo->prox) {				
				inimigo_esquerda(inimigo) ;	
				// colisao_player_inimigo(struct jogador_t *p, struct lista_t *l) ;
				
				// Chance de 50% do inimigo se movimentar para cima, baixo ou direita
				if (!inimigo->move_timer) {
					int a = 3 % aleat(2, 4) ;
					if (a == 0) {
						inimigo_cima(inimigo) ;
					} else if (a == 1) {
						inimigo_baixo(inimigo) ;
					} else {
						inimigo_direita(inimigo) ;
					}
					inimigo->move_timer = COOLDOWN_MOVE ;
				}
				inimigo->move_timer-- ;
				
				if (!inimigo->arma->timer) {
					inimigo_atira(inimigo) ;
					inimigo->arma->timer = ARMA_COOLDOWN ;
				}
				if (inimigo->arma->timer) {
					inimigo->arma->timer-- ;
				}
				al_draw_filled_rectangle(inimigo->x, inimigo->y, inimigo->x + LADO_INIMIGO, inimigo->y + LADO_INIMIGO, verde) ;				
			}
			
			// Se a lista foi inicializada desenha cada bala de cada inimigo	
			if (l_atira->ini != NULL) {
				atualiza_balas_inimigas (l_atira) ;
				// Percorre cada inimigo
				for (struct inimigo_t *i = l_atira->ini; i != NULL; i = i->prox) {	
					// Desenha as balas inimigas
					for (struct bala_t *b = i->arma->bala; b != NULL; b = b->prox) {
						al_draw_filled_circle(b->x, b->y, 4, verde) ;					
					}		
				}	
			}

			atualiza_pos(player, l_atira, l) ;
			// Soma Lado do quadrado para que o quadrado se movimente+seu proprio lado, desenha o player
			al_draw_filled_rectangle(player->x, player->y, player->x+LADO_QUADRADO, player->y+LADO_QUADRADO, azul) ;
			// Desenha os tiros do player na tela
			for (struct bala_t *b = player->arma->bala; b != NULL; b = b->prox) {
				al_draw_filled_circle(b->x, b->y, 4, azul) ;
			}
			if (player->arma->timer) {
				player->arma->timer-- ;
			}
			
			// Atualiza a tela
			al_flip_display() ;			
		// Evento de tecla pressionada ou apertado
		} else if (eventos.type == ALLEGRO_EVENT_KEY_DOWN || eventos.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (eventos.keyboard.keycode) {	
				case ALLEGRO_KEY_DOWN:
					controle_baixo(player->controle) ;
					break ;	
				case ALLEGRO_KEY_UP:	
					controle_cima(player->controle) ;
					break ;
				case ALLEGRO_KEY_RIGHT:	
					controle_direita(player->controle) ;		
					break ;
				case ALLEGRO_KEY_LEFT:	
					controle_esquerda(player->controle) ;
					break ;
				case ALLEGRO_KEY_X:	
					controle_tiro(player->controle) ;
					break ;			
			}	
		// Evento de tecla solta
		} else if (eventos.type == ALLEGRO_EVENT_KEY_UP) {
			switch (eventos.keyboard.keycode) {	
				// Desativa o controle
				case ALLEGRO_KEY_DOWN:
					player->controle->baixo = 0; 
					break;	
				case ALLEGRO_KEY_UP:	
					player->controle->cima = 0;
					break;
				case ALLEGRO_KEY_RIGHT:	
					player->controle->dir = 0;		
					break;
				case ALLEGRO_KEY_LEFT:	
					player->controle->esq = 0;
					break;	
				case ALLEGRO_KEY_X:	
					player->controle->tiro = 0;
					break;		
			}	 
		}
	}	
	
	destroi_jogador(player) ;
	destroi_lista(l) ;
	destroi_lista(l_atira) ;
	al_destroy_display(janela) ;
	al_destroy_timer(tempo) ;														
	al_destroy_event_queue(fila_de_eventos) ;
	
	return 0 ;
}
