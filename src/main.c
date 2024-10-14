#include "juego.h"
#include "struct_bot.h"
#include "struc_juego.h"
#include "bot.h"

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
    if (argc < 3) {
        printf("Uso: %s <palabra_secreta>  <modo_de_game: 0 para normal, 1 para usar recomendaciones>\n", argv[0]);
        return 1;
    }
    int useBot = *argv[2] - '0'; 
    if(useBot != 0 && useBot != 1){
        printf("Uso: %s <palabra_secreta>  <modo_de_game: 0 para normal, 1 para usar recomendaciones>\n", argv[0]);
        return 1;
    }

    Heap *heap = crearHeap(MAX_WORDS);
    char palabra_bot[WORD_LENGTH] = {'a', 'i', 'r', 'e', 'o', '\0'};  // Para almacenar la palabra adivinada
    LetraPosicionada letras_correctas[WORD_LENGTH - 1] = {0};
    LetraPosicionada letras_incorrectas[MAX_ALFA - WORD_LENGTH] = {0};  // Letras correctas en las posiciones correctas
    char letras_presentes[WORD_LENGTH - 1] = {0};
    int letrasPresentes = 0;
    int letrasCorrectas = 0;
    int letrasIncorrectas = 0;
    int desfasajeIncorrectas = 0;
    int desfasajeCorrectas = 0;
    int desfasajeIncorrectas = 0;
    /*if(!useBot){
        liberarHeap(heap)
        free(palabra_bot)
        free(letras_correctas);
        free(letras_incorrectas);  // Letras correctas en las posiciones correctas
        free(letras_presentes);
        free(letrasPresentes);
        free(letrasCorrectas);
        free(letrasIncorrectas);
        free(desfasajeIncorrectas);
        free(desfasajeCorrectas);
        free(desfasajeIncorrectas);
    }else{*/
        cargarPalabrasDesdeArchivo(&heap, "3.txt");
//    }

    WordleGame game;
    // Inicializa el game con la palabra secreta
    if (initialize_game(&game, argv[1]) != 0) {
        return 1;
    }

    char player_word[WORD_LENGTH];

    for (int intento = 0; intento < MAX_ATTEMPTS; intento++) {

        if (heap->size > 0 && intento != 0 && useBot) {
            // Obtener la palabra más frecuente (raíz del data)
            PalabraConFrecuencia palabraActual = eliminarRaiz(heap);
            strcpy(palabra_bot, palabraActual.palabra);
            printf("\nEl bot recomienda la palabra: %s", palabra_bot);
            

        }else if(heap->size > 0 && intento == 0 && useBot){
            printf("\nEl bot recomienda la palabra: %s", palabra_bot);
        } else if (useBot)
        {
            printf("No hay más palabras para recomendar\n");
        }else
        {
            
        }
        printf("\nIngresa tu palabra de 5 letras: ");
        
        scanf("%5s", player_word);  // Limita la entrada a 5 letras

        // Check if the guessed word is valid
        guess_word(&game, player_word);

        print_feedback(&game, player_word);

        if (game.won) {
            printf("¡Felicidades, has ganado!\n");
            return 0;
        } else if (game.attempts_left == 0) {
            printf("Has perdido. La palabra secreta era: %s\n", game.letters);
            return 0;
        } else {
            printf("Intentos restantes: %d\n", game.attempts_left);
        }

        // Procesar el feedback de la adivinanza
        for (int i = 0; i < WORD_LENGTH - 1; i++) {
            int estado = game.feedback[i];

            // Chequear letras correctas
            if (estado == CORRECTO) {
                bool yaCorrecta = false;

                // Verificar si la letra ya está en letras_correctas
                for (int k = 0; k < letrasCorrectas; k++) {
                    if (letras_correctas[k].letra == player_word[i]) {
                        yaCorrecta = true;

                        // Marcar también la posición actual si ya está en otra posición
                        if (!isPositionSet(&letras_correctas[k], i)) {
                            setPosition(&letras_correctas[k], i);  // Marcar posición actual como correcta
                        }
                        break;
                    }
                }

                // Si la letra no está en letras_correctas, se agrega
                if (!yaCorrecta) {
                    letras_correctas[letrasCorrectas].letra = player_word[i];
                    setPosition(&letras_correctas[letrasCorrectas], i);  // Marcar posición como correcta
                    letrasCorrectas++;
                }
            }
            // Chequear letras incorrectas
            else if (estado == INCORRECTO) {
                bool yaIncorrecta = false;

                // Verificar si la letra ya está en letras_incorrectas
                for (int k = 0; k < letrasIncorrectas; k++) {
                    if (letras_incorrectas[k].letra == player_word[i]) {
                        yaIncorrecta = true;
                        break;
                    }
                }

                // Verificar si la letra está en letras_presentes
                bool enPresentes = false;
                for (int j = 0; j < letrasPresentes; j++) {
                    if (letras_presentes[j] == player_word[i]) {
                        enPresentes = true;
                        break;
                    }
                }

                // Solo marcar como incorrecta si no está en letras_correctas
                if (!yaIncorrecta && !enPresentes) {
                    bool letraEsCorrectaEnOtraPosicion = false;

                    // Verificar si la letra es correcta en alguna otra posición
                    for (int k = 0; k < letrasCorrectas; k++) {
                        if (letras_correctas[k].letra == player_word[i]) {
                            letraEsCorrectaEnOtraPosicion = true;  // La letra es correcta en esta posición

                            // Si es correcta en esta posición, entonces no marcar incorrecta
                            if (!isPositionSet(&letras_correctas[k], i)) {
                                // Aquí marcar como incorrecta en todas las posiciones menos i
                                for (int z = 0; z < WORD_LENGTH - 1; z++) {
                                    if (z != i) {  // No marcar la posición i
                                        setPosition(&letras_incorrectas[letrasIncorrectas], z);
                                    }
                                }
                            }
                            break;  // Salir del bucle, ya que la letra fue encontrada
                        }
                    }

                    // Si la letra no es correcta en otras posiciones, agregar a letras_incorrectas
                    if (!letraEsCorrectaEnOtraPosicion) {
                        letras_incorrectas[letrasIncorrectas].letra = player_word[i];
                        // Aquí puedes marcar todas las posiciones como incorrectas según tu lógica
                        for (int pos = 0; pos < WORD_LENGTH - 1; pos++) {
                            if (pos != i) {
                                setPosition(&letras_incorrectas[letrasIncorrectas], pos);
                            }
                        }
                        letrasIncorrectas++;
                    }
                }
            }

            // Chequear letras presentes
            else if (estado == PRESENTE) {
                bool yaPresente = false;

                // Verificar si la letra ya está en letras_presentes
                for (int k = 0; k < letrasPresentes; k++) {
                    if (letras_presentes[k] == player_word[i]) {
                        yaPresente = true;
                        break;
                    }
                }

                // Si la letra no está en letras_presentes, se agrega
                if (!yaPresente) {
                    letras_presentes[letrasPresentes] = player_word[i];
                    letrasPresentes++;
                }

                // Verificar si la letra no está en letras_incorrectas, marcarla como incorrecta en la posición actual
                bool letraIncorrecta = false;
                for (int j = 0; j < letrasIncorrectas; j++) {
                    if (letras_incorrectas[j].letra == player_word[i]) {
                        letraIncorrecta = true;
                        setPosition(&letras_incorrectas[j], i);  // Marcar la posición como inválida
                        break;
                    }
                }

                // Si la letra no está en letras_incorrectas, la agregamos y marcamos la posición actual como incorrecta
                if (!letraIncorrecta) {
                    letras_incorrectas[letrasIncorrectas].letra = player_word[i];
                    setPosition(&letras_incorrectas[letrasIncorrectas], i);  // Marcar la posición actual como incorrecta
                    letrasIncorrectas++;
                }
            }
        }
        filtrarPalabras(heap, letras_presentes, letras_incorrectas, letras_correctas, letrasPresentes, letrasCorrectas, letrasIncorrectas, desfasajeIncorrectas);
        // Filtrar palabras según la retroalimentación
        desfasajeCorrectas = letrasCorrectas;
        desfasajeIncorrectas = letrasIncorrectas;
        desfasajeIncorrectas = letrasPresentes;
    }

    liberarHeap(heap);
    return 0;
}