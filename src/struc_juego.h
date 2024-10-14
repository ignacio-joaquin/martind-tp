#ifndef STRUC_GAME_H
#define STRUC_GAME_H

#include <stdbool.h>

#define MAX_WORDS 4976    // Total de palabras en 3.txt
#define WORD_LENGTH 6      // Longitud de cada palabra + 1 por el terminador
#define MAX_ATTEMPTS 6     // Número máximo de intentos
#define MAX_ALFA 26

// Estados de retroalimentación
typedef enum {
    INCORRECTO = 0, // Letra no está en la palabra
    PRESENTE = 1,   // Letra está en la palabra pero en posición incorrecta
    CORRECTO = 2    // Letra está en la posición correcta
} Feedback;

// Estructura que representa el estado del juego de Wordle
typedef struct {
    char letters[WORD_LENGTH];    // Letras de la palabra actual
    int feedback[WORD_LENGTH];    // Retroalimentación para cada letra
    int attempts_left;             // Intentos restantes
    int won;                       // Indicador de victoria
} WordleGame;

#endif // STRUC_GAME_H
