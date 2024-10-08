#include <stdio.h>
#include <string.h>
#include "juego.h"
#include "struc_juego.h"

void print_feedback(WordleGame *game, const char *player_word) {
    printf("Palabra ingresada: %s\n", player_word);
    printf("Retroalimentación: ");
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        if (game->feedback[i] == CORRECTO) {
            printf("C "); // Correcto
        } else if (game->feedback[i] == PRESENTE) {
            printf("P "); // Presente
        } else {
            printf("I "); // Incorrecto
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <palabra_secreta>\n", argv[0]);
        return 1;
    }

    char words[MAX_WORDS][WORD_LENGTH];
    int word_count = readWords("3.txt", words); // Lee las palabras del archivo

    // Verifica que las palabras se cargaron correctamente
    if (word_count <= 0) {
        printf("No se pudieron cargar palabras del archivo.\n");
        return 1;
    }

    WordleGame game;
    // Inicializa el juego con la palabra secreta
    if (initialize_game(&game, argv[1], argv[1]) != 0) {
        return 1;
    }

    while (game.attempts_left > 0 && !game.won) {
        char player_word[WORD_LENGTH + 1];
        printf("Ingresa tu palabra de 5 letras: ");
        scanf("%5s", player_word);  // Limita la entrada a 5 letras

        guess_word(&game, words, player_word);
        print_feedback(&game, player_word);

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
