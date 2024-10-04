#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORDS 5000
#define MAX_LENGTH 6 // 5 letters + 1 for null terminator

// Function prototypes
int readWords(const char *filename, char words[MAX_WORDS][MAX_LENGTH]);
int isValidWord(const char *word);
void writeOutput(const char *filename, char words[MAX_WORDS][MAX_LENGTH], int count);
char* selectRandomWord(char words[MAX_WORDS][MAX_LENGTH], int count);

int main() {
    char words[MAX_WORDS][MAX_LENGTH];
    int count = readWords("3.txt", words);

    if (count < 0) {
        return EXIT_FAILURE; // Error reading words
    }

    writeOutput("output.txt", words, count);
    
    return 0;
}

int readWords(const char *filename, char words[MAX_WORDS][MAX_LENGTH]) {
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
                strncpy(words[count], word, MAX_LENGTH - 1);
                words[count][MAX_LENGTH - 1] = '\0'; // Null-terminate
                count++;
            }
        }
    }

    fclose(file);
    return count;
}

int isValidWord(const char *word) {
    if (strlen(word) > 0 && strlen(word) < MAX_LENGTH) {
        for (int i = 0; i < strlen(word); i++) {
            if (!isalpha(word[i])) {
                return 0; // Invalid word
            }
        }
        return 1; // Valid word
    }
    return 0; // Invalid word
}

void writeOutput(const char *filename, char words[MAX_WORDS][MAX_LENGTH], int count) {
    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Error al abrir el archivo de salida");
        return;
    }

    fprintf(outputFile, "Total words read: %d\n", count);
    fprintf(outputFile, "Words read:\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(outputFile, "%s\n", words[i]);
    }

    char *randomWord = selectRandomWord(words, count);
    if (randomWord) {
        fprintf(outputFile, "Palabra seleccionada: %s\n", randomWord);
    } else {
        fprintf(outputFile, "No hay palabras para seleccionar.\n");
    }

    fclose(outputFile);
}

char* selectRandomWord(char words[MAX_WORDS][MAX_LENGTH], int count) {
    if (count > 0) {
        srand(time(NULL));
        int randomIndex = rand() % count; // Get a random index
        return words[randomIndex];
    }
    return NULL; // No words to select
}
