#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>  // Include the time library
#include "struc_juego.h"



int binary_search(char arr[MAX_WORDS][WORD_LENGTH], int size, const char *target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid], target);

        if (cmp == 0) {
            return mid; // Target found
        }
        if (cmp < 0) {
            left = mid + 1; // Search in the right half
        } else {
            right = mid - 1; // Search in the left half
        }
    }

    return -1; // Target not found
}


int initialize_game(WordleGame *game, char words[MAX_WORDS][WORD_LENGTH],const char *palabraSecreta ,const char *secret_word) {
    if (strlen(palabraSecreta) != WORD_LENGTH-1 && binary_search(words, MAX_WORDS, palabraSecreta)) {
        printf("Por favor, proporciona una palabra secreta de 5 letras como argumento que este presente en la base de datos.\n");
        return 1;
    }
    strncpy(game->letters, secret_word, WORD_LENGTH);
    memset(game->feedback, ' ', WORD_LENGTH);
    game->attempts_left = MAX_ATTEMPTS;
    game->won = false;
}

void guess_word(WordleGame *game, char words[MAX_WORDS][WORD_LENGTH],const char *player_word) {
    memset(game->feedback, ' ', WORD_LENGTH);
    if(binary_search(words, MAX_WORDS, player_word)){
        for (int i = 0; i < WORD_LENGTH; i++) {
            if (player_word[i] == game->letters[i]) {
                game->feedback[i] = CORRECTO;  // Correct letter in the correct position
            } else if (strchr(game->letters, player_word[i])) {
                game->feedback[i] = PRESENTE;  // Present letter but in the wrong position
            } else {
                game->feedback[i] = INCORRECTO;  // Incorrect letter
            }
        }
        game->attempts_left--;
    }


    if (strcmp(player_word, game->letters) == 0) {
        game->won = true;
    }
}