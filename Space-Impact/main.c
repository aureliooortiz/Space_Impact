// Compilar: 
// gcc main.c -o main -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// Definições da janela
#define ALT_TELA 1280
#define LARG_TELA 720
#define POS_TELA 100
#define NOME_TELA "Space Impact"


int main() {
	ALLEGRO_DISPLAY *janela ;
	ALLEGRO_COLOR azul ;
	ALLEGRO_COLOR vermelho ;
	ALLEGRO_EVENT_QUEUE *fila_de_eventos ;
	ALLEGRO_EVENT eventos ;
	ALLEGRO_TIMER *tempo ;
	
	// Inicializa váriaveis de cor
	azul = al_map_rgb(0, 0, 255) ;
	vermelho = al_map_rgb(255, 0, 0) ;
	
	// Inicializa e instala tudo que será utilizado no programa
	al_init() ;
	al_init_primitives_addon() ;
	al_install_keyboard() ;
		
	// Cria e inicializa a janela
	al_set_new_display_flags(ALLEGRO_RESIZABLE) ;
	janela = al_create_display(ALT_TELA, LARG_TELA) ;
	al_set_window_position(janela, POS_TELA, POS_TELA) ;
	al_set_window_title(janela, NOME_TELA) ;
	
	// Cria a taxa de atualização do jogo FPS
	tempo = al_create_timer(1.0 / 30.0) ;
	
	// Cria e inicializa os tipos de eventos na fila
	fila_de_eventos = al_create_event_queue() ;
	al_register_event_source(fila_de_eventos, al_get_keyboard_event_source()) ;
	al_register_event_source(fila_de_eventos, al_get_timer_event_source(tempo)) ;
	
	al_start_timer(tempo) ;		
	// Loop principal do jogo
	while (1) {
		al_wait_for_event(fila_de_eventos, &eventos) ;
		
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			// Pinta a tela de preto e desenha dois triangulos nela
			al_draw_filled_triangle(1*20, 1*20, 1*20, 5*20, 3*20, 3*20, azul) ;
			al_draw_filled_triangle(1*20, 1*20+500, 1*20, 5*20+500, 3*20, 3*20+500, vermelho) ;
			
			// Atualiza a tela
			al_flip_display() ;
			al_clear_to_color(al_map_rgb(0, 0, 0)) ;			
			
		} else if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break ;	
	}
	
	al_destroy_display(janela) ;
	al_destroy_timer(tempo);														
	al_destroy_event_queue(fila_de_eventos);
	
	return 0 ;
}
