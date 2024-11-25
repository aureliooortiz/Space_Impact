#include <stdlib.h>

#include "utili.h"

// Retorna um numero aleatorio entre min e max
int aleat (int min, int max) {
	return min + rand() % (max-min + 1) ;
}
