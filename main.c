// Compilar: 
// gcc main.c -o main -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <stdlib.h> 
#include <stdio.h> 
#include <time.h>

#include "utili.h"
#include "colisao.h"
#include "cenario.h"

// Atualiza a posição do jogador e dos tiros disparados, verifica e colisões do player
void atualiza_player (struct jogador_t *p, struct lista_t *l1, struct lista_t *l2, struct powerup_t *power) {
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
}

// Atualiza informações relacionadas ao powerup
void atualiza_powerup (struct powerup_t *power, struct jogador_t *p) {
	if (!power->coletado) {
		// Powerup sempre se locomove para a esquerda
		movimenta_powerup (power, 1, ESQUERDA) ;
		if (colisao_player_powerup(p, *power)) {
			switch (power->tipo) {
				case GELO:
					power->coletado = 1 ;
					p->gelo_timer = GELO_COOLDOWN ;
					break ;
			}		
		}
	
		desenha_powerup (power) ;						
	}
}

void desenha_vida(struct jogador_t *p, struct powerup_t *power) {
	if (p->vida == 3) {
		al_draw_bitmap_region (power->coracao->coracao_spr, 0, 0, power->coracao->larg_spr*3, 
								power->coracao->alt_spr, 0, ALT_TELA-65, 0) ;
	} else if (p->vida == 2 ) {
		al_draw_bitmap_region (power->coracao->coracao_spr, 0, 0, power->coracao->larg_spr*2-10, 
								power->coracao->alt_spr, 0, ALT_TELA-65, 0) ;
	} else if (p->vida == 1) {
		al_draw_bitmap_region (power->coracao->coracao_spr, 0, 0, power->coracao->larg_spr*1, 
								power->coracao->alt_spr, 0, ALT_TELA-65, 0) ;
	}
}

// Verifica se o powerup pode nascer e o cria 
void nasce_powerup (struct powerup_t *power) {
	if (!power->powerup_timer) {
		destroi_powerup (power) ;
		// Cria powerup
		*power = cria_powerup (LARG_TELA-LARG_POWERUP, aleat(0, ALT_TELA-ALT_POWERUP), GELO) ;
	} else {
		power->powerup_timer-- ;
	}
}

// Desenha e movimenta todos inimigos na tela que apenas se movimentam
void atualiza_inimigo_movimenta (struct lista_t *l, struct jogador_t *player, int passo) {
	int aux ;

	// Desenha todos inimigos na tela que apenas se movimentam
	for (struct inimigo_t *inimigo = l->ini; inimigo != NULL; inimigo = inimigo->prox) {
		if (!inimigo->congelado) {		
			movimenta_inimigo (inimigo, passo, ESQUERDA) ;
		}
		
		aux = colisao_inimigo_bala(player, inimigo) ;			
		if (aux) {
			inimigo_perde_vida(inimigo) ;
			// Verifica se o inimigo deve ser congelado
			if (aux == 2) {	
				inimigo->congelado++ ;
			}	
		} 
		// Verifica se o inimigo ainda está congelado		
		verifica_congelado (inimigo) ;
		
		if (colisao_inimigo_player (player, inimigo)
			&& !player->invulnerabilidade) {	
			jogador_perde_vida(player) ;
		}
		
		// Desenha o inimigo caso esteja vivo, e o faz piscar caso leve dano	
		desenha_inimigo (l, inimigo) ;	
	}
}

// Desenha e movimenta todos inimigos na tela que podem disparar balas
void atualiza_inimigo_atira (struct lista_t *l_atira, struct jogador_t *p, int fase) {
	for (struct inimigo_t *inimigo = l_atira->ini; inimigo != NULL; inimigo = inimigo->prox) {				
		movimenta_inimigo (inimigo, 1, ESQUERDA) ;
		if (colisao_inimigo_bala(p, inimigo)) {
			inimigo_perde_vida(inimigo) ;
		} 
		if (colisao_inimigo_player (p, inimigo)
			&& !p->invulnerabilidade) {
			jogador_perde_vida(p) ;
		}
		
		// Movimentação dos inimigos da fase 1
		if (!fase) {		
			// Chance de 50% do inimigo se movimentar para cima, baixo ou direita 
			if (!inimigo->move_timer) { 
				int a = 3 % aleat(2, 4) ;
				if (a == 0) {
					movimenta_inimigo (inimigo, 1, CIMA) ;
					if (colisao_inimigo_bala(p, inimigo)) {
						inimigo_perde_vida(inimigo) ;
					}
					if (colisao_inimigo_player (p, inimigo)
						&& !p->invulnerabilidade) {
						jogador_perde_vida(p) ;
					}
				} else if (a == 1) {
					movimenta_inimigo (inimigo, 1, BAIXO) ;
					if (colisao_inimigo_bala(p, inimigo)) {
						inimigo_perde_vida(inimigo) ;
					}
					if (colisao_inimigo_player (p, inimigo)
						&& !p->invulnerabilidade) {
						jogador_perde_vida(p) ;
					}
				} else {
					movimenta_inimigo (inimigo, 1, DIREITA) ;
					if (colisao_inimigo_bala(p, inimigo)) {
						inimigo_perde_vida(inimigo) ;
					}
					if (colisao_inimigo_player (p, inimigo)
						&& !p->invulnerabilidade) {
						jogador_perde_vida(p) ;
					}
				}
				
				inimigo->move_timer = COOLDOWN_MOVE ;
			} 
		} 	
		inimigo->move_timer-- ;		
				
		if (!inimigo->arma->timer) {
			inimigo_atira(inimigo) ;
			inimigo->arma->timer = INIMIGO_ARMA_COOLDOWN ;
		}
		if (colisao_bala_player (p, inimigo) 
			&& !p->invulnerabilidade) {
			jogador_perde_vida(p) ;
		} 
		if (inimigo->arma->timer) {
			inimigo->arma->timer-- ;
		}
		desenha_inimigo (l_atira, inimigo) ;					
	} 
}

void spawna_inimigo (struct lista_t *l) {
	// Inimigo nasce no lado esquerdo da tela em um y aleatório
	l->ini = cria_inimigo(LARG_TELA-100, aleat(0, ALT_TELA-100), l->ini) ;
	l->ini->alt = l->alt_spr_inimigos ;
	l->ini->larg = l->larg_spr_balas ;
	// Desenha primeira "aparição" do inimigo em tela, ele nasce
	al_draw_bitmap_region (l->inimigos_spr, 0, 0, l->larg_spr_inimigos, l->alt_spr_inimigos, l->ini->x, l->ini->y, 0) ;	
}

// Verifica se os inimigos podem nascer e o cria
void nasce_inimigo (struct lista_t *l, struct lista_t *l_atira) {	
	if (!l->timer) {
		spawna_inimigo(l) ;

		// Verifica se inimigos não nasceram no mesmo lugar
		
		
		l->timer = INIMIGO_COOLDOWN_1 ;
	} else if (l_atira->timer <= 0) {
		spawna_inimigo(l_atira) ;
		
		// Verifica se inimigos não nasceram no mesmo lugar
		
		
		l_atira->timer = INIMIGO_COOLDOWN_2 ;
	}
	l->timer-- ;
	l_atira->timer-- ;
}

int main() {
	ALLEGRO_DISPLAY *janela ;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos ;
	ALLEGRO_EVENT eventos ;
	ALLEGRO_TIMER *tempo ;
	struct jogador_t *player ;
	struct lista_t *l_atira , *l ;
	struct lista_t *l_fase2 , *l1_fase2 ;
	struct powerup_t power ;
	struct cenario_t cenario, cenario_fase2 ;
	int contador, fase, movimenta_cima, cooldown_atq ;
	
	srand(10) ;
	
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
	player->player_spr = al_load_bitmap("assets/ship.png") ;
	player->larg_spr_player = al_get_bitmap_width(player->player_spr) / 2 ;
	player->alt_spr_player = al_get_bitmap_height(player->player_spr) / 5 ;	
	player->balas_spr = al_load_bitmap("assets/PlayerBullet.png") ;
	player->larg_spr_bala = al_get_bitmap_width(player->balas_spr) ;
	player->alt_spr_bala = al_get_bitmap_height(player->balas_spr) ;
	
	// Inicia powerup e seus sprites
	power = cria_powerup (LARG_TELA-LARG_POWERUP, ALT_TELA-ALT_POWERUP, GELO) ;
	
	// Lista de inimigos da fase 1
	l = cria_lista_inimigos ("assets/Asteroids1.png", 1, 1, 
							NULL, 1, 1,
							NULL, 1, 1,
							NULL, -1) ;	
	l_atira = cria_lista_inimigos ("assets/enemy1.png", 2, 1, 
								"assets/EnemyBullet.png", 1, 1,
								"assets/KanyeWest.png", 1, 1, 
								"assets/EnemyBullet.png", 0) ;	
	
	// Lista de inimigos da fase 2
	l_fase2 = cria_lista_inimigos ("assets/enemy2.png", 1, 1, 
							"assets/EnemyBullet.png", 1, 1,
							NULL, 1, 1,
							NULL, -1) ;	
	l1_fase2 = cria_lista_inimigos ("assets/enemy3.png", 1, 1, 
								"assets/EnemyBullet.png", 1, 1,
								"assets/Boss.png", 1, 1, 
								"assets/EnemyBullet.png", 1) ;	
	
	// Inicia váriaveis do cenário
	cenario = cria_background ("assets/Space01.png") ;
	cenario_fase2 = cria_background ("assets/background.png") ;
	
	cooldown_atq = -300 ;
	movimenta_cima = 1 ;
	// Verifica passagem de fase
	contador = 0 ;
	fase = 0 ;
	al_start_timer (tempo) ;		
	// Loop principal do jogo
	while (1) {
		al_wait_for_event (fila_de_eventos, &eventos) ;
		
		// Evento de clique no "x" da janela
		if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break ;	
		
		// Evento que atualiza a tela
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0)) ;
			
			if (!fase) {	
				desenha_background_animado (&cenario) ;
			} else {
				desenha_background_animado (&cenario_fase2) ;
			}
			
			// Verifica morte 
			if (!player->vida || fase == 3) break ;
			
			// Spawna powerup caso possível
			nasce_powerup (&power) ;
			// Movimenta powerup e o desenha na tela
			atualiza_powerup (&power, player) ;
			//verifica_powerup (power) ;
		
			contador++ ;
			if (contador < 300) { 
				// Fase 1
				if (!fase) {
					// Verifica os inimigos que possam ter saído da tela ou morrido
					verifica_inimigos(l_atira) ;
					verifica_inimigos(l) ;
					// Inimigos spawnam caso não estejam em cooldown
					nasce_inimigo(l, l_atira) ;		
					// Desenha e movimenta todos inimigos na tela que apenas se movimentam
					atualiza_inimigo_movimenta (l, player, 1) ; 
					// Desenha e movimenta todos inimigos na tela que podem disparar balas
					atualiza_inimigo_atira (l_atira, player, fase) ;
					desenha_balas_inimigas (l_atira) ;
					// Destroi todos os inimigos em tela
					if (contador == 299) {
						destroi_todos_inimigos (l_atira) ;
						destroi_todos_inimigos (l) ;
					} 
				// Fase 2	
				} else {
					// Verifica os inimigos que possam ter saído da tela ou morrido
					verifica_inimigos(l_fase2) ;
					verifica_inimigos(l1_fase2) ;
					nasce_inimigo(l_fase2, l1_fase2) ;		
					atualiza_inimigo_atira (l_fase2, player, fase) ;
					atualiza_inimigo_atira (l1_fase2, player, fase) ;
					desenha_balas_inimigas (l_fase2) ;				
					desenha_balas_inimigas (l1_fase2) ;
					// Destroi todos os inimigos em tela
					if (contador == 299) {
						destroi_todos_inimigos (l_fase2) ;
						destroi_todos_inimigos (l1_fase2) ;
					} 
				}	
			} else { 
				// Boss da fase 1
				if (!fase) {
					if (l_atira->chefe->atbt->vida > 0) {
						int aux ;
						
						// Desenha boss
						desenha_boss (l_atira) ;
						
						// Chefe "balança" para cima e para baixo
						
						aux = colisao_inimigo_bala(player, l_atira->chefe->atbt) ;			
						if (aux) {
							inimigo_perde_vida(l_atira->chefe->atbt) ;
							// Verifica se o inimigo deve ser congelado
							if (aux == 2) {	
								l_atira->chefe->atbt->congelado++ ;
							}	
						} 
						// Verifica se o inimigo ainda está congelado		
						verifica_congelado (l_atira->chefe->atbt) ;
						
						// Checa colisão com player
						if (colisao_boss_player (player, l_atira->chefe->atbt)
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						}
						atualiza_balas_boss (l_atira->chefe, l_atira->chefe->arma1) ;
						desenha_balas_boss (l_atira->chefe, l_atira->chefe->arma1) ;
						
						if (colisao_bala_player_boss (player, l_atira->chefe->arma1) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l_atira->chefe, l_atira->chefe->arma2) ;
						desenha_balas_boss (l_atira->chefe, l_atira->chefe->arma2) ;
						
						if (colisao_bala_player_boss (player, l_atira->chefe->arma2) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l_atira->chefe, l_atira->chefe->arma3) ;
						desenha_balas_boss (l_atira->chefe, l_atira->chefe->arma3) ;
						
						if (colisao_bala_player_boss (player, l_atira->chefe->arma3) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l_atira->chefe, l_atira->chefe->arma4) ;
						desenha_balas_boss (l_atira->chefe, l_atira->chefe->arma4) ;
						
						if (colisao_bala_player_boss (player, l_atira->chefe->arma4) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l_atira->chefe, l_atira->chefe->arma5) ;
						desenha_balas_boss (l_atira->chefe, l_atira->chefe->arma5) ;
						
						if (colisao_bala_player_boss (player, l_atira->chefe->arma5) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						if (!l_atira->chefe->atbt->congelado) {				
							if (l_atira->chefe->ataque) {
								// Ataque 1
								if (l_atira->chefe->ataque_timer > ATAQUE) {
									movimenta_inimigo (l_atira->chefe->atbt, 3, ESQUERDA) ;
									// Se o boss saiu da tela, volta para pos inicial, e reinicia timer
									if (l_atira->chefe->atbt->x < 0) {
										l_atira->chefe->atbt->x = LARG_TELA-l_atira->chefe->larg_spr-20 ;
										l_atira->chefe->atbt->y = 20 ;
										l_atira->chefe->ataque_timer = COOLDOWN_BOSS1_ATAQUE ;
										// Muda a chance do proximo ataque ocorrer
										l_atira->chefe->ataque = 0 ;
									}
								} 
								l_atira->chefe->ataque_timer++ ;
							} else if (!l_atira->chefe->ataque) {	
								// Ataque 2	
								if (l_atira->chefe->ataque_timer < ATAQUE) {
									// Tiro que sai de cima 
									boss_atira (l_atira->chefe->arma1, l_atira->chefe->atbt->x+35, 20) ;
									// Tiro que sai de cima parte baixa
									boss_atira (l_atira->chefe->arma2, l_atira->chefe->atbt->x, 100) ;
									// Tiro que sai do meio
									boss_atira (l_atira->chefe->arma3, l_atira->chefe->atbt->x, 
												l_atira->chefe->atbt->y+l_atira->chefe->alt_spr/2) ;
									// Tiro que sai de baixo parte cima
									boss_atira (l_atira->chefe->arma4, l_atira->chefe->atbt->x, l_atira->chefe->alt_spr-50) ;
									// Tiro que sai de baixo 
									boss_atira (l_atira->chefe->arma5, l_atira->chefe->atbt->x+30, l_atira->chefe->alt_spr) ;
								} else {
									l_atira->chefe->ataque_timer = COOLDOWN_BOSS1_ATAQUE ;
									l_atira->chefe->ataque = 1 ;	
								}
								l_atira->chefe->ataque_timer++ ; 
							}	
						}
					} else {
						fase = 1 ;
						contador = 0 ;
					}		
				} else { 
					if (l1_fase2->chefe->atbt->vida > 0) {
						// Boss da fase 2
						int aux ;
						
						if (!l1_fase2->chefe->atbt->congelado && cooldown_atq > 0) {
							if (movimenta_cima) {	
								movimenta_inimigo (l1_fase2->chefe->atbt, 1, CIMA) ;
								if (l1_fase2->chefe->atbt->y-PASSO_INIMIGO <= 0) {
									movimenta_cima = 0 ;
								}
							} else {
								movimenta_inimigo (l1_fase2->chefe->atbt, 1, BAIXO) ;
								if ((l1_fase2->chefe->atbt->y+l1_fase2->chefe->atbt->alt)+PASSO_INIMIGO >= ALT_TELA) {
									movimenta_cima = 1 ;
								}
							}	
						}	
						// Desenha boss
						desenha_boss (l1_fase2) ;
						
						atualiza_inimigo_movimenta (l, player, 4) ; 
						verifica_inimigos (l) ;
						
						// Chefe "balança" para cima e para baixo
						
						aux = colisao_inimigo_bala(player, l1_fase2->chefe->atbt) ;			
						if (aux) {
							inimigo_perde_vida(l1_fase2->chefe->atbt) ;
							// Verifica se o inimigo deve ser congelado
							if (aux == 2) {	
								l1_fase2->chefe->atbt->congelado++ ;
							}	
						} 
						// Verifica se o inimigo ainda está congelado		
						verifica_congelado (l1_fase2->chefe->atbt) ;
						
						// Checa colisão com player
						if (colisao_boss_player (player, l1_fase2->chefe->atbt)
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						}
						
						atualiza_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma1) ;
						desenha_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma1) ;
						
						if (colisao_bala_player_boss (player, l1_fase2->chefe->arma1) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma2) ;
						desenha_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma2) ;
						
						if (colisao_bala_player_boss (player, l1_fase2->chefe->arma2) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 
						
						atualiza_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma3) ;
						desenha_balas_boss (l1_fase2->chefe, l1_fase2->chefe->arma3) ;
	
						if (colisao_bala_player_boss (player, l1_fase2->chefe->arma3) 
							&& !player->invulnerabilidade) {
							jogador_perde_vida(player) ;
						} 

						if (!l1_fase2->chefe->atbt->congelado && cooldown_atq > 0) {	
							if (l1_fase2->chefe->ataque) {
								// Ataque 1
								if (l1_fase2->chefe->ataque_timer < ATAQUE) {
									// Invoca asteroids que vem muito rápido
									if (!l->timer) {	
										spawna_inimigo (l) ;
										l->timer = 15 ;											
									}
									l->timer-- ;	
								} else {
									l1_fase2->chefe->ataque_timer = COOLDOWN_BOSS2_ATAQUE ;
									l1_fase2->chefe->ataque = 0 ;
									cooldown_atq = -300 ;
								} 
								l1_fase2->chefe->ataque_timer++ ;
							} else if (!l1_fase2->chefe->ataque) {	
								// Ataque 2	
								if (l1_fase2->chefe->ataque_timer < ATAQUE) {
									// Tiro que sai de cima 
									boss_atira (l1_fase2->chefe->arma1, l1_fase2->chefe->atbt->x, l1_fase2->chefe->atbt->y) ;
									// Tiro que sai do meio
									boss_atira (l1_fase2->chefe->arma2, l1_fase2->chefe->atbt->x, 
												l1_fase2->chefe->atbt->y+l1_fase2->chefe->alt_spr/2) ;
									// Tiro que sai de baixo parte 
									boss_atira (l1_fase2->chefe->arma3, l1_fase2->chefe->atbt->x, 
									l1_fase2->chefe->atbt->y+l1_fase2->chefe->alt_spr) ;
								} else {
									l1_fase2->chefe->ataque_timer = COOLDOWN_BOSS2_ATAQUE ;
									l1_fase2->chefe->ataque = 1 ;	
									cooldown_atq = -300 ;
								}
								l1_fase2->chefe->ataque_timer++ ; 
							}
						} else {
							cooldown_atq++ ;
						}	
					} else {
						fase = 3 ;
					}
				}	
			}
			if (!fase) {
				atualiza_player(player, l_atira, l, &power) ;
			} else {
				atualiza_player(player, l_fase2, l1_fase2, &power) ;
			}	
			desenha_vida(player, &power) ;
			desenha_jogador (player) ;
			desenha_balas_jogador (player) ;
			
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
