#include "bot.h"
#include "juego.h"
#include "struc_juego.h"
#include "struct_bot.h"

void print_feedback(WordleGame *game, const char *player_word) {
    // Solo imprime retroalimentación si el intento fue válido
    if (game->attempts_left < MAX_ATTEMPTS) {
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
    if (argc < 3) {
        printf("Uso: %s <palabra_secreta>  <modo_de_game: 0 para normal, 1 para usar recomendaciones>\n", argv[0]);
        return 1;
    }
    int useBot = *argv[2] - '0';
    if (useBot != 0 && useBot != 1) {
        printf("Uso: %s <palabra_secreta>  <modo_de_game: 0 para normal, 1 para usar recomendaciones>\n", argv[0]);
        return 1;
    }

    Heap *heap = crearHeap(MAX_WORDS);
    char palabra_bot[WORD_LENGTH] = PALABRA_INICIAL;  // Para almacenar la palabra que usara el bot
    LetraPosicionada letras_correctas[WORD_LENGTH - 1] = {0};
    LetraPosicionada letras_incorrectas[MAX_ALFA - WORD_LENGTH] = {0};  // Letras correctas en las posiciones correctas
    char letras_presentes[WORD_LENGTH - 1] = {0};
    int cantLetrasPresentes = 0;
    int cantLetrasCorrectas = 0;
    int cantLetrasIncorrectas = 0;
    int desfasajeIncorrectas = 0;
    if (useBot) {
        cargarPalabrasDesdeArchivo(&heap, "3.txt");
    }

    WordleGame game;
    // Inicializa el game con la palabra secreta
    if (initialize_game(&game, argv[1]) != 0) {
        return 1;
    }

    char player_word[WORD_LENGTH];

    for (int intento = 0; intento < MAX_ATTEMPTS; intento++) {
        if (useBot) {
            if (heap->size > 0) {
                if (intento != 0) {
                    // Obtener la palabra más frecuente (raíz del heap)
                    PalabraConFrecuencia palabraActual = eliminarRaiz(heap);
                    strcpy(palabra_bot, palabraActual.palabra);
                }
                printf("\nEl bot recomienda la palabra: %s", palabra_bot);
            } else {
                printf("No hay más palabras para recomendar\n");
            }
        }

        printf("\nIngresa tu palabra de %d letras: ", WORD_LENGTH);

        scanf("%s", player_word);  // Limita la entrada a 5 letras
        if (strlen(player_word) != WORD_LENGTH) {
            printf("\nIngresa una palabra de %d letras: ", WORD_LENGTH);
        }

        // "adivinar" la palabra
        guess_word(&game, player_word);

        print_feedback(&game, player_word);

        if (game.won) {
            printf("¡Felicidades, has ganado!\n");
            liberarHeap(heap);
            return 0;
        } else if (game.attempts_left == 0) {
            printf("Has perdido. La palabra secreta era: %s\n", game.letters);
            liberarHeap(heap);
            return 0;
        } else {
            printf("Intentos restantes: %d\n", game.attempts_left);
        }

        // Procesar el feedback de la adivinanza
        procesarFeeback(game, player_word, letras_correctas, &cantLetrasCorrectas, letras_incorrectas, &cantLetrasIncorrectas, letras_presentes, &cantLetrasPresentes);

        // Filtrar palabras según la retroalimentación
        filtrarPalabras(heap, letras_presentes, letras_incorrectas, letras_correctas, cantLetrasPresentes, cantLetrasCorrectas, cantLetrasIncorrectas, desfasajeIncorrectas);

        desfasajeIncorrectas = cantLetrasIncorrectas;
    }
}