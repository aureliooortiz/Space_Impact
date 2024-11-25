# Nome do executável
TARGET = space

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Bibliotecas Allegro
ALLEGRO_LIBS = -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

# Arquivos-fonte e headers
SRC = main.c player.c controle.c arma.c bala.c utili.c inimigos.c colisao.c
HEADERS = player.h controle.h arma.h bala.h utili.h inimigos.h colisao.h

# Regra padrão para compilar o executável
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(ALLEGRO_LIBS)

# Regra para limpar arquivos gerados pela compilação
clean:
	rm -f $(TARGET)

