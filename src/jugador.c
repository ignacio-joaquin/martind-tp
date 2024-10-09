#include <stdio.h>
#include <string.h>

#include "juego.h"
#include "struc_juego.h"

void print_feedback(WordleGame *game, const char *player_word) {
    // Solo imprime retroalimentación si el intento fue válido
    if (game->attempts_left < MAX_ATTEMPTS) { // Verifica que no sea el primer intento
        printf("Palabra ingresada: %s\n", player_word);
        printf("Retroalimentación: ");
        for (int i = 0; i < WORD_LENGTH - 1; i++) {
            if (game->feedback[i] == CORRECTO) {
                printf("\033[0;32mC \033[0m");  // Correcto (verde)
            } else if (game->feedback[i] == PRESENTE) {
                printf("\033[0;33mP \033[0m");  // Presente (amarillo)
            } else {
                printf("\033[0;31mI \033[0m");  // Incorrecto (rojo)
            }
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <palabra_secreta>\n", argv[0]);
        return 1;
    }

    char words[MAX_WORDS][WORD_LENGTH];
    int word_count = readWords("3.txt", words);  // Lee las palabras del archivo

    // Verifica que las palabras se cargaron correctamente
    if (word_count <= 0) {
        printf("No se pudieron cargar palabras del archivo.\n");
        return 1;
    }

    WordleGame game;
    // Inicializa el juego con la palabra secreta
    if (initialize_game(&game, words, argv[1]) != 0) {
        return 1;
    }

    char player_word[WORD_LENGTH];

    while (game.attempts_left > 0 && !game.won) {
        printf("\nIngresa tu palabra de 5 letras: ");
        scanf("%5s", player_word);  // Limita la entrada a 5 letras

        // Check if the guessed word is valid
        int valid_guess = guess_word(&game, words, player_word);

        // Only print feedback if the guess was valid
        if (valid_guess == 0) { // If the guess is invalid
            printf("Palabra no válida. Inténtalo de nuevo.\n");
        } else {
            print_feedback(&game, player_word);
        }

        if (game.won) {
            printf("¡Felicidades, has ganado!\n");
        } else if (game.attempts_left == 0) {
            printf("Has perdido. La palabra secreta era: %s\n", game.letters);
        } else {
            printf("Intentos restantes: %d\n", game.attempts_left);
        }
    }

    return 0;
}
