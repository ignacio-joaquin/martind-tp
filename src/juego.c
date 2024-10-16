#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>  

#include "struc_juego.h"

// Inicializa el juego
int initialize_game(WordleGame *game, const char *palabraSecreta) {
    if (strlen(palabraSecreta) != WORD_LENGTH - 1) {
        printf("Por favor, proporciona una palabra secreta de 5 letras como argumento que este presente en la base de datos.\n");
        return 1;
    }
    strncpy(game->letters, palabraSecreta, WORD_LENGTH);
    memset(game->feedback, ' ', WORD_LENGTH);
    game->attempts_left = MAX_ATTEMPTS;
    game->won = false;
    return 0;
}

int guess_word(WordleGame *game, const char *player_word) {

    // Arrays temporales para controlar el estado de las letras de la palabra secreta y la palabra del jugador
    int contadorLetrasSecretas[WORD_LENGTH] = {0};  // Para contar la cantidad de cada letra en la palabra secreta
    int letrasMarcadasCorrecto[WORD_LENGTH] = {0};  // Para marcar las letras del jugador que ya fueron evaluadas como CORRECTO

    //Identificar letras correctas (CORRECTO) y contar las letras restantes de la palabra secreta
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        if (player_word[i] == game->letters[i]) {
            game->feedback[i] = CORRECTO;  // Letra en la posición correcta
            letrasMarcadasCorrecto[i] = 1;   // Marca esta posición como evaluada
        } else {
            game->feedback[i] = INCORRECTO;  // Por defecto, incorrecto hasta que lo revisemos más adelante
            contadorLetrasSecretas[game->letters[i] - 'a']++;  // Contar las letras restantes en la palabra secreta
        }
    }

    //Evaluar letras presentes pero en posición incorrecta (PRESENTE)
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        if (game->feedback[i] == INCORRECTO) {  // Solo revisar las que aún no están correctas
            char current_letter = player_word[i];

            // Si la letra está presente en la palabra secreta y no marcamos todas sus ocurrencias
            if (contadorLetrasSecretas[current_letter - 'a'] > 0) {
                game->feedback[i] = PRESENTE;  // Marcar como presente
                contadorLetrasSecretas[current_letter - 'a']--;  // Reducir el contador de letras restantes en la palabra secreta
            }
        }
    }

    game->attempts_left--;

    if (strcmp(player_word, game->letters) == 0) {
        game->won = true;
    }

    return 1;  // Adivinanza válida
}

