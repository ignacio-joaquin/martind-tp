#ifndef JUEGO_H
#define JUEGO_H

#include "struc_juego.h"

// Prototipos de funciones
int initialize_game(WordleGame *game, const char *palabraSecreta, const char *secret_word);
void guess_word(WordleGame *game, char words[MAX_WORDS][WORD_LENGTH], const char *player_word);

#endif // JUEGO_H
