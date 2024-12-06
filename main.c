// Compilar: 
// gcc main.c -o main -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <stdlib.h> 
#include <stdio.h> 

#include "utili.h"
#include "colisao.h"

// Atualiza posição dos tiros do jogador em tela e os destroi se precisar
void atualiza_balas (struct jogador_t *p) {
	struct bala_t *ant ;
	
	ant = NULL ;
	// Percorre por cada bala
	for (struct bala_t *atual = p->arma->bala; atual != NULL;) {
		// Atualiza posição da bala
		atual->x += MOVIMENTO_BALA ;
		
		// Verifica se o tiro saiu da tela ou acertou um inimigo
		if (atual->x > LARG_TELA || atual->acertou) {
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

// Atualiza a posição do jogador e dos tiros disparados, verifica e colisões do player
void atualiza_player (struct jogador_t *p, struct lista_t *l1, struct lista_t *l2, struct powerup_t *power,
					ALLEGRO_COLOR *cor) {
	// Verifica cada controle se estiver ativado movimenta jogador
	if (p->controle->cima) {
		movimenta_jogador (p, 1, CIMA) ;
		if (!p->invulnerabilidade) {	
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)
				|| colisao_player_bala(p, l1)) {
				jogador_perde_vida(p) ; 
			}	
		}		
		if (colisao_player_powerup(p, *power)) { 
			switch (power->tipo) {
				case GELO:
					power->coletado = 1 ;
					p->gelo_timer = GELO_COOLDOWN ;
					// Tiro fica roxo
					*cor = al_map_rgb(160, 32, 240) ;
					break ;
			}
		}	
	}
	if (p->controle->baixo) {
		movimenta_jogador (p, 1, BAIXO) ;
		if (!p->invulnerabilidade) {		
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)
				|| colisao_player_bala(p, l1)) {
				jogador_perde_vida(p) ; 
			}	
		}
		if (colisao_player_powerup(p, *power)) {
			switch (power->tipo) {
				case GELO:
					power->coletado = 1 ;
					p->gelo_timer = GELO_COOLDOWN ;
					*cor = al_map_rgb(160, 32, 240) ;
					break ;
			}
		}	
	}	
	if (p->controle->dir) {
		movimenta_jogador (p, 1, DIREITA) ;
		if (!p->invulnerabilidade) {	
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)
				|| colisao_player_bala(p, l1)) {
				jogador_perde_vida(p) ;
			}	
		}
		if (colisao_player_powerup(p, *power)) {
			switch (power->tipo) {
				case GELO:
					power->coletado = 1 ;
					p->gelo_timer = GELO_COOLDOWN ;
					*cor = al_map_rgb(160, 32, 240) ;
					break ;
			}
		}	
	}	
	if (p->controle->esq) {
		movimenta_jogador (p, 1, ESQUERDA) ;
		if (!p->invulnerabilidade) {		
			if (colisao_player_inimigo(p, l1) || colisao_player_inimigo(p, l2)
				|| colisao_player_bala(p, l1)) { 
				jogador_perde_vida(p) ;
			}	
		}	
		
		if (colisao_player_powerup(p, *power)) {
			switch (power->tipo) {
				case GELO:
					power->coletado = 1 ;
					p->gelo_timer = GELO_COOLDOWN ;
					*cor = al_map_rgb(160, 32, 240) ;
					break ;
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
	
	// Atualiza timers do jogador
	if (p->invulnerabilidade != 0) {	
		p->invulnerabilidade-- ;
	}
	if (p->arma->timer) {
		p->arma->timer-- ;
	}
	if (p->gelo_timer) {
		p->gelo_timer-- ;
	}
	if (!p->gelo_timer) {
		// Tiro volta a cor normal
		*cor = al_map_rgb(0, 0, 255) ;
	}
}

void desenha_jogador (struct jogador_t *player) {
	// Player pisca caso esteja invulneravel
	if (!player->invulnerabilidade || ((player->invulnerabilidade % 2) == 0)) {
		// Desenha o jogador
		al_draw_bitmap_region(player->player_spr, 0, 2 * player->alt_spr_player, 
								player>larg_spr_player, player->alt_spr_player, player->x, player->y, 0) ;
	}
}

void desenha_balas_jogador (struct jogador_t *player, ALLEGRO_COLOR azul) {
	// Desenha os tiros do player na tela
	for (struct bala_t *b = player->arma->bala; b != NULL; b = b->prox) {
		al_draw_filled_circle(b->x, b->y, 4, azul) ;
	}
}

void desenha_vida(struct jogador_t *p, struct powerup_t power) {
	if (p->vida == 3) {
		al_draw_bitmap_region (power.coracao->coracao_spr, 1, 0, power.coracao->larg_spr*3, 
								power.coracao.alt_spr, 0, ALT_TELA-power.coracao.alt_spr+10, 0) ;
	} else if (p->vida == 2 ) {
		al_draw_bitmap_region (power.coracao->coracao_spr, 1, 0, power.coracao->larg_spr*2-10, 
								power.coracao->alt_spr, 0, ALT_TELA-power.coracao->alt_spr+10, 0) ;
	} else if (p->vida == 1) {
		al_draw_bitmap_region (power.coracao.coracao_spr, 1, 0, power.coracao.larg_spr*2-10, 
								power.coracao->alt_spr, 0, ALT_TELA-power.coracao->alt_spr+10, 0) ;
	}
}


// Atualiza informações relacionadas ao powerup
void atualiza_powerup (struct powerup *power) {
	if (!power.coletado) {
		// Powerup sempre se locomove para a esquerda
		movimenta_powerup (power, 1, ESQUERDA) ;
		// Desenha powerup
		al_draw_filled_rectangle(power.x, power.y, power.x + power.larg_power, power.y + power.alt_power, laranja) ;
	}
}

// Verifica se o powerup pode nascer e o cria 
void nasce_powerup (struct powerup_t *power) {
	if (!power.powerup_timer) {
		// Cria powerup
		power = cria_powerup (LARG_TELA-LARG_POWERUP, aleat(0, ALT_TELA-ALT_POWERUP), GELO) ;
	} else {
		power.powerup_timer-- ;
	}
}

// Verifica cada inimigo se saiu da tela ou morreu, se sim o destroi
void verifica_inimigos (struct lista_t *l) {
	struct inimigo_t *ant ;
	struct inimigo_t *aux ;
	
	ant = NULL ;
	for (struct inimigo_t *atual = l->ini; atual != NULL;) {	
		// Verifica se o inimigo saiu da tela ou morreu
		if (atual->x < 0 || !atual->vida) {
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
		if (i->vida > 0) {	
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
}

void desenha_balas_inimigas (struct lista_t *l_atira) {
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
}	

void verifica_congelado (struct inimigo_t *inimigo) {
	if (inimigo->congelado == TEMPO_CONGELADO) {
		inimigo->congelado = 0 ;
	} else if (inimigo->congelado > 0) {
		inimigo->congelado++ ;
	}
}

void desenha_inimigo (struct inimigo_t *inimigo) {
	ALLEGRO_COLOR vermelho, azul_escuro ;
	
	azul_escuro = al_map_rgb(0, 0, 139) ;
	vermelho = al_map_rgb(255, 0, 0) ;
	if (inimigo->vida > 0) {
		if (!inimigo->pisca) {	
			if (!inimigo->congelado) {		
				al_draw_filled_rectangle(inimigo->x, inimigo->y, 
										inimigo->x + inimigo->larg, inimigo->y + inimigo->alt, vermelho) ;
			} else {
				al_draw_filled_rectangle(inimigo->x, inimigo->y, 
										inimigo->x + inimigo->larg, inimigo->y + inimigo->alt, azul_escuro) ;
			}								
		} else {
			inimigo->pisca = 0 ;
		}	
	}
}

// Desenha e movimenta todos inimigos na tela que apenas se movimentam
void atualiza_inimigo_atira (struct lista_t *l, struct jogador_t *p) {
	// Desenha todos inimigos na tela que apenas se movimentam
	for (struct inimigo_t *inimigo = l->ini; inimigo != NULL; inimigo = inimigo->prox) {
		if (!inimigo->congelado) {	
			movimenta_inimigo (inimigo, 1, ESQUERDA) ;
		}
					
		if ((colisao_inimigo_bala(player, inimigo) == 2)) {
			inimigo_perde_vida(inimigo) ;
			// Verifica se o inimigo deve ser congelado
			if (!inimigo->congelado && player->gelo_timer > 0) {	
				inimigo->congelado++ ;
			}	
		}
		// Verifica se o inimigo ainda está congelado		
		verifica_congelado (inimigo) ;
		
		// Verifica colisao com player
		
		// Desenha o inimigo caso esteja vivo, e o faz piscar caso leve dano
		desenha_inimigo (inimigo) ;	
	}
}

// Desenha e movimenta todos inimigos na tela que podem disparar balas
void atualiza_inimigo_atira (struct lista_t *l_atira, struct jogador_t *p) {
	ALLEGRO_COLOR verde ;
	
	verde = al_map_rgb(0, 255, 0) ;
	for (struct inimigo_t *inimigo = l_atira->ini; inimigo != NULL; inimigo = inimigo->prox) {				
		movimenta_inimigo (inimigo, 1, ESQUERDA) ;
		if (colisao_inimigo_bala(p, inimigo)) {
			inimigo_perde_vida(inimigo) ;
		} 
				
		// Chance de 50% do inimigo se movimentar para cima, baixo ou direita 
		if (!inimigo->move_timer) { 
			int a = 3 % aleat(2, 4) ;
			if (a == 0) {
				movimenta_inimigo (inimigo, 1, CIMA) ;
				if (colisao_inimigo_bala(p, inimigo)) {
					inimigo_perde_vida(inimigo) ;
				}
				// colisao_player_inimigo(player, l_atira) ;
			} else if (a == 1) {
				movimenta_inimigo (inimigo, 1, BAIXO) ;
				if (colisao_inimigo_bala(p, inimigo)) {
					inimigo_perde_vida(inimigo) ;
				}
				// colisao_player_inimigo(player, l_atira) ;
			} else {
				movimenta_inimigo (inimigo, 1, DIREITA) ;
				if (colisao_inimigo_bala(p, inimigo)) {
					inimigo_perde_vida(inimigo) ;
				}
				//colisao_player_inimigo(player, l_atira) ;
			}
			
			inimigo->move_timer = COOLDOWN_MOVE ;
		} 
		inimigo->move_timer-- ;		
				
		if (!inimigo->arma->timer) {
			inimigo_atira(inimigo) ;
			inimigo->arma->timer = INIMIGO_ARMA_COOLDOWN ;
		}
		if (inimigo->arma->timer) {
			inimigo->arma->timer-- ;
		}
		// Desenha inimigo
		if (inimigo->vida > 0) {
			if (!inimigo->pisca) {
				al_draw_filled_rectangle(inimigo->x, inimigo->y, inimigo->x + inimigo->larg, inimigo->y + inimigo->alt, verde) ;
			} else {
				inimigo->pisca = 0 ;
			}					
		}	
	} 
}

void spawna_inimigo (struct lista_t *l, ALLEGRO_COLOR cor) {
	// Inimigo nasce no lado esquerdo da tela em um y aleatório
	l->ini = cria_inimigo(LARG_TELA - LARG_INIMIGO, aleat(0, ALT_TELA-ALT_INIMIGO) , l->ini) ;
	// Desenha primeira "aparição" do inimigo em tela, ele nasce
	al_draw_filled_rectangle(l->ini->x, l->ini->y, l->ini->x + l->ini->larg, l->ini->y + l->ini->alt, cor) ;
}

// Verifica se os inimigos podem nascer e o cria
void nasce_inimigo (struct lista_t *l, struct lista_t *l_atira) {
	ALLEGRO_COLOR vermelho, verde ;
	
	vermelho = al_map_rgb(255, 0, 0) ;
	verde = al_map_rgb(0, 255, 0) ;
	if (!l->timer) {
		spawna_inimigo(l_atira, vermelho) ;
		
		// Verifica se inimigos não nasceram no mesmo lugar
		
		
		l->timer = INIMIGO_COOLDOWN_1 ;
	} else if (l_atira->timer <= 0) {
		spawna_inimigo(l_atira, verde) ;
		
		// Verifica se inimigos não nasceram no mesmo lugar
		
		l_atira->timer = INIMIGO_COOLDOWN_2 ;
	}
	l->timer-- ;
	l_atira->timer-- ;
}

int main() {
	ALLEGRO_DISPLAY *janela ;
	ALLEGRO_COLOR azul, vermelho, verde, azul_escuro, laranja ;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos ;
	ALLEGRO_EVENT eventos ;
	ALLEGRO_TIMER *tempo ;
	struct jogador_t *player ;
	struct lista_t *l_atira ;
	struct lista_t *l ;
	struct powerup_t power ;
	
	srand(0) ;
	
	// Inicializa váriaveis de cor
	azul = al_map_rgb(0, 0, 255) ;
	azul_escuro = al_map_rgb(0, 0, 139) ;
	vermelho = al_map_rgb(255, 0, 0) ;
	verde = al_map_rgb(0, 255, 0) ;
	laranja = al_map_rgb(255, 165, 0) ;
	
	// Inicializa e instala tudo que será utilizado no programa
	al_init() ;
	al_init_primitives_addon() ;
	al_install_keyboard() ;
	al_init_image_addon() ;
		
	// Cria e inicializa a janela
	janela = al_create_display(LARG_TELA, ALT_TELA) ;
	al_set_window_position(janela, POS_TELA, POS_TELA) ;
	al_set_window_title(janela, NOME_TELA) ;
	
	// Cria a taxa de atualização do jogo FPS
	tempo = al_create_timer(1.0 / 30.0) ;
	
	// Cria e inicializa os tipos de eventos na fila
	fila_de_eventos = al_create_event_queue() ;
	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source()) ;
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(tempo)) ;
	al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
	
	// Inicia jogador e seus sprites
	player = cria_jogador(0, 0) ;
	p->player_spr = al_load_bitmap("assets/ship.png") ;
	p->larg_spr_player = al_get_bitmap_width(player_spr) / 2 ;
	p->alt_spr_player = al_get_bitmap_height(player_spr) / 5 ;
	
	// Inicia powerup e seus sprites
	power = cria_powerup(LARG_TELA-LARG_POWERUP, ALT_TELA-ALT_POWERUP, GELO) ;
	power.coracao->coracao_spr = al_load_bitmap("assets/Heart.png") ;
	power.coracao.larg_spr = al_get_bitmap_width(power.coracao.coracao_spr) / 3 ;
	power.coracao.alt_spr = al_get_bitmap_height(power.coracao.coracao_spr) ;
	
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
						
			// Verifica morte 
			if (!player->vida) break ;
			
			// Spawna powerup caso possível
			nasce_powerup (power) ;
			// Movimenta powerup e o desenha na tela
			atualiza_powerup (&power) ;
			
			// Verifica os inimigos que possam ter saído da tela ou morrido
			verifica_inimigos(l_atira) ;
			verifica_inimigos(l) ;
			
			// Inimigo spawna caso não esteja em cooldown
			nasce_inimigo(l, l_atira) ;		
	
			// Desenha e movimenta todos inimigos na tela que apenas se movimentam
			atualiza_inimigo_movimenta (l, player) ; 
			
			// Desenha e movimenta todos inimigos na tela que podem disparar balas
			atualiza_inimigo_atira (l_atira, player) ;
			desenha_balas_inimigas (l_atira) ;

			atualiza_player(player, l_atira, l, &power, &azul) ;
			desenha_vida(player, power) ;
			desenha_jogador (player) ;
			desenha_balas_jogador (player, azul) ;
			
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
