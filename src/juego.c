#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>  // Include the time library

#define MAX_WORDS 4976 // todo el 3.txt
#define WORD_LENGTH 6
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

#define WORD_LENGTH 6 // Assuming this is meant to include the null terminator

int isValidWord(const char *word) {
    if (strlen(word) == WORD_LENGTH - 1) { // Check for WORD_LENGTH - 1 for the actual word length
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
                words[count][WORD_LENGTH - 1] = '\0'; // Null-terminate correctly
                count++;
            }
        }
    }

    fclose(file);
    return count;
}


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
    if (argc != 2 || strlen(argv[1]) != WORD_LENGTH-1) {
        printf("Por favor, proporciona una palabra secreta de 5 letras como argumento.\n");
        return 1;
    }
    char words[MAX_WORDS][WORD_LENGTH];
    readWords("3.txt",words);

    clock_t start_time = clock(); // Start timing
    int result = binary_search(words, MAX_WORDS, argv[1]);
    clock_t end_time = clock(); // End timing

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calculate elapsed time

    if (result == -1) {
        printf("palabra no valida\n");
    } else {
        printf("Palabra válida encontrada en el índice: %d\n", result);
    }
    
    printf("Tiempo de búsqueda binaria: %f segundos\n", time_taken);

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
