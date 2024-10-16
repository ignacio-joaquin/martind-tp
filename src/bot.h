#ifndef BOT_H
#define BOT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "struc_juego.h"
#include "struct_bot.h"

#define PALABRA_INICIAL "aireo\0"

void filtrarPalabras(Heap *heap, char letras_presentes[], LetraPosicionada letras_incorrectas[], LetraPosicionada letras_correctas[],
                     int letrasPresentes, int letrasCorrectas, int letrasIncorrectas, int desfasajePresentes);

void procesarFeeback(WordleGame game, char *player_word, LetraPosicionada *letras_correctas, int *cantLetrasCorrectas,
                     LetraPosicionada *letras_incorrectas, int *cantLetrasIncorrectas, char *letras_presentes, int *cantLetrasPresentes);

#endif  // STRUCT_BOT_H