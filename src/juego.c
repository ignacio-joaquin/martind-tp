#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>  // Include the time library

#include "struc_juego.h"

int binary_search(char arr[MAX_WORDS][WORD_LENGTH], int size, const char *target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid], target);

        if (cmp == 0) {
            return mid;  // Target found
        }
        if (cmp < 0) {
            left = mid + 1;  // Search in the right half
        } else {
            right = mid - 1;  // Search in the left half
        }
    }

    return -1;  // Target not found
}

int initialize_game(WordleGame *game, char words[MAX_WORDS][WORD_LENGTH], const char *palabraSecreta) {
    if (strlen(palabraSecreta) != WORD_LENGTH - 1 || binary_search(words, MAX_WORDS, palabraSecreta) == -1) {
        printf("Por favor, proporciona una palabra secreta de 5 letras como argumento que este presente en la base de datos.\n");
        return 1;
    }
    strncpy(game->letters, palabraSecreta, WORD_LENGTH);
    memset(game->feedback, ' ', WORD_LENGTH);
    game->attempts_left = MAX_ATTEMPTS;
    game->won = false;
    return 0;
}

int guess_word(WordleGame *game, char words[MAX_WORDS][WORD_LENGTH], const char *player_word) {
    if (binary_search(words, MAX_WORDS, player_word) == -1) {
        return 0;  // Palabra inválida
    }

    // Arrays temporales para controlar el estado de las letras de la palabra secreta y la palabra del jugador
    int secret_letter_count[26] = {0};  // Para contar la cantidad de cada letra en la palabra secreta
    int player_letter_marked[WORD_LENGTH] = {0};  // Para marcar las letras del jugador que ya fueron evaluadas como CORRECTO

    // Paso 1: Identificar letras correctas (CORRECTO) y contar las letras restantes de la palabra secreta
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        if (player_word[i] == game->letters[i]) {
            game->feedback[i] = CORRECTO;  // Letra en la posición correcta
            player_letter_marked[i] = 1;   // Marca esta posición como evaluada
        } else {
            game->feedback[i] = INCORRECTO;  // Por defecto, incorrecto hasta que lo revisemos más adelante
            secret_letter_count[game->letters[i] - 'a']++;  // Contar las letras restantes en la palabra secreta
        }
    }

    // Paso 2: Evaluar letras presentes pero en posición incorrecta (PRESENTE)
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        if (game->feedback[i] == INCORRECTO) {  // Solo revisar las que aún no están correctas
            char current_letter = player_word[i];

            // Si la letra está presente en la palabra secreta y no hemos marcado todas sus ocurrencias
            if (secret_letter_count[current_letter - 'a'] > 0) {
                game->feedback[i] = PRESENTE;  // Marcar como presente
                secret_letter_count[current_letter - 'a']--;  // Reducir el contador de letras restantes en la palabra secreta
            }
        }
    }

    game->attempts_left--;

    if (strcmp(player_word, game->letters) == 0) {
        game->won = true;
    }

    return 1;  // Adivinanza válida
}


