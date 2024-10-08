#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "struc_juego.h"

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