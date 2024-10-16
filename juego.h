#ifndef JUEGO_H
#define JUEGO_H

#include "struc_juego.h"

// Prototipos de funciones
int initialize_game(WordleGame *game,  const char *player_word);
int guess_word(WordleGame *game,  const char *player_word);

#endif // JUEGO_H
