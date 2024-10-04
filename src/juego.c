#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 5000
#define WORD_LENGTH 5
#define MAX_ATTEMPTS 6
#define CORRECTO 1
#define INCORRECTO 0
#define PRESENTE 2

typedef struct {
    char letters[WORD_LENGTH];
    int feedback[WORD_LENGTH];
    int attempts_left;
    int won;
} WordleGame;

int isValidWord(const char *word) {
    if (strlen(word) > 0 && strlen(word) < WORD_LENGTH) {
        for (int i = 0; i < strlen(word); i++) {
            if (!isalpha(word[i])) {
                return 0; // Invalid word
            }
        }
        return 1; // Valid word
    }
    return 0; // Invalid word
}

int readWords(const char *filename, char words[MAX_WORDS][WORD_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    int count = 0;
    while (fgets(words[count], sizeof(words[count]), file) != NULL && count < MAX_WORDS) {
        char *word = strtok(words[count], ",");
        if (word != NULL) {
            word[strcspn(word, "\n")] = '\0'; // Remove newline
            if (isValidWord(word)) {
                strncpy(words[count], word, WORD_LENGTH - 1);
                words[count][WORD_LENGTH - 1] = '\0'; // Null-terminate
                count++;
            }
        }
    }

    fclose(file);
    return count;
}


void initialize_game(WordleGame *game, const char *secret_word) {
    strncpy(game->letters, secret_word, WORD_LENGTH);
    memset(game->feedback, ' ', WORD_LENGTH);
    game->attempts_left = MAX_ATTEMPTS;
    game->won = false;
}

void guess_word(WordleGame *game, const char *player_word) {
    game->attempts_left--;
    memset(game->feedback, ' ', WORD_LENGTH);

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (player_word[i] == game->letters[i]) {
            game->feedback[i] = CORRECTO;  // Correct letter in the correct position
        } else if (strchr(game->letters, player_word[i])) {
            game->feedback[i] = PRESENTE;  // Present letter but in the wrong position
        } else {
            game->feedback[i] = INCORRECTO;  // Incorrect letter
        }
    }

    if (strcmp(player_word, game->letters) == 0) {
        game->won = true;
    }
}

void print_feedback(const WordleGame *game, const char *player_word) {
    printf("Palabra: %s | Feedback: ", player_word);
    for (int i = 0; i < WORD_LENGTH; i++) {
        printf("%d ", game->feedback[i]);
    }
    printf("\nIntentos restantes: %d\n", game->attempts_left);
}

int main(int argc, char *argv[]) {
    if (argc != 2 || strlen(argv[1]) != WORD_LENGTH) {
        printf("Por favor, proporciona una palabra secreta de 5 letras como argumento.\n");
        return 1;
    }

    WordleGame game;
    initialize_game(&game, argv[1]);

    while (game.attempts_left > 0 && !game.won) {
        char player_word[WORD_LENGTH + 1];
        printf("Ingresa tu palabra de 5 letras: ");
        scanf("%5s", player_word);  // Limit input to 5 letters

        guess_word(&game, player_word);
        print_feedback(&game, player_word);

        if (game.won) {
            printf("¡Felicidades, has ganado!\n");
        } else if (game.attempts_left == 0) {
            printf("Has perdido. La palabra secreta era: %s\n", game.letters);
        }
    }

    return 0;
}
