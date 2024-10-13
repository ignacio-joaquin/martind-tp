#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "struc_juego.h"

typedef struct PalabraConFrecuencia {
    char palabra[WORD_LENGTH];          // Palabra
    int frecuencia;                     // Frecuencia de uso
    struct PalabraConFrecuencia *next;  // Puntero al siguiente nodo
} PalabraConFrecuencia;

typedef struct {
    char letra;
    int posicion[WORD_LENGTH - 1];
} letraPresente;

typedef struct {
    char letra;
    int posicion[WORD_LENGTH - 1];
} letraIncorrecta;

// Función para crear un nuevo nodo
PalabraConFrecuencia *crearNodo(const char *palabra, int frecuencia) {
    PalabraConFrecuencia *nuevoNodo = malloc(sizeof(PalabraConFrecuencia));
    if (!nuevoNodo) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    strncpy(nuevoNodo->palabra, palabra, WORD_LENGTH);
    nuevoNodo->frecuencia = frecuencia;
    nuevoNodo->next = NULL;
    return nuevoNodo;
}

// Función para agregar una palabra a la lista
// Función para agregar una palabra a la lista de manera ordenada por frecuencia
void agregarPalabra(PalabraConFrecuencia **cabeza, const char *palabra, int frecuencia) {
    PalabraConFrecuencia *nuevoNodo = crearNodo(palabra, frecuencia);

    // Si la lista está vacía o la frecuencia del nuevo nodo es mayor que la cabeza
    if (*cabeza == NULL || frecuencia > (*cabeza)->frecuencia) {
        nuevoNodo->next = *cabeza;  // Insertar al inicio
        *cabeza = nuevoNodo;
        return;
    }

    // Buscar la posición correcta para insertar el nuevo nodo
    PalabraConFrecuencia *actual = *cabeza;
    while (actual->next != NULL && actual->next->frecuencia >= frecuencia) {
        actual = actual->next;
    }

    // Insertar el nuevo nodo en la posición correcta
    nuevoNodo->next = actual->next;
    actual->next = nuevoNodo;
}

// Función para cargar palabras desde un archivo
void cargarPalabrasDesdeArchivo(PalabraConFrecuencia **cabeza, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char palabra[WORD_LENGTH];
    int frecuencia;

    while (fscanf(archivo, "%[^,],%d\n", palabra, &frecuencia) == 2) {
        agregarPalabra(cabeza, palabra, frecuencia);
        printf("Cargada palabra: %s con frecuencia: %d\n", palabra, frecuencia);  // Para depuración
    }

    fclose(archivo);
}

// Función para filtrar palabras inválidas
// Función para filtrar palabras inválidas
void filtrarPalabras(PalabraConFrecuencia **cabeza, char letras_presentes[], letraIncorrecta letras_incorrectas[], letraPresente letras_correctas[], int letrasPresentes, int letrasCorrectas, int letrasIncorrectas) {
    PalabraConFrecuencia *actual = *cabeza;
    PalabraConFrecuencia *anterior = NULL;

    while (actual != NULL) {
        bool esValida = true;

        // Verificar letras correctas
        for (int i = 0; i < WORD_LENGTH - 1 && esValida; i++) {
            for (int j = 0; j < letrasCorrectas; j++) {
                if (letras_correctas[j].posicion[i]) {
                    if (letras_correctas[j].letra != actual->palabra[i]) {
                        printf("le falta letra correcta\n");
                        esValida = false;
                        break;
                    }
                }
            }

            // Verificar letras incorrectas
            for (int j = 0; j < letrasIncorrectas && esValida; j++) {
                if (letras_incorrectas[j].posicion[i]) {
                    if (letras_incorrectas[j].letra == actual->palabra[i]) {
                        printf("tiene letra incorrecta\n");
                        esValida = false;
                        break;
                    }
                }
            }
        }

        // Verificar que todas las letras presentes estén en la palabra
        for (int j = 0; letras_presentes[j] != '\0' && esValida; j++) {
            bool presente = false;
            for (int i = 0; i < WORD_LENGTH - 1; i++) {
                if (letras_presentes[j] == actual->palabra[i]) {
                    presente = true;
                    break;
                }
            }
            if (!presente) {
                printf("le falta letra posible\n");
                esValida = false;  // Si una letra presente no está en la palabra, la palabra es inválida
            }
        }

        // Si la palabra no es válida, eliminarla
        if (!esValida) {
            printf("Palabra eliminada: %s\n", actual->palabra);  // Mensaje de depuración
            if (anterior == NULL) {
                // Se elimina la cabeza
                *cabeza = actual->next;
                free(actual);
                actual = *cabeza;
            } else {
                anterior->next = actual->next;
                free(actual);
                actual = anterior->next;
            }
        } else {
            anterior = actual;
            actual = actual->next;
        }
    }
}

// Función para imprimir la lista de palabras
void imprimirPalabras(PalabraConFrecuencia *cabeza) {
    PalabraConFrecuencia *actual = cabeza;
    while (actual != NULL) {
        printf("Palabra: %s, Frecuencia: %d\n", actual->palabra, actual->frecuencia);
        actual = actual->next;
    }
}

// Liberar memoria de la lista
void liberarLista(PalabraConFrecuencia *cabeza) {
    PalabraConFrecuencia *actual = cabeza;
    while (actual != NULL) {
        PalabraConFrecuencia *temp = actual;
        actual = actual->next;
        free(temp);
    }
}

// Función para eliminar una palabra de la lista
void eliminarPalabra(PalabraConFrecuencia **cabeza, const char *palabra) {
    PalabraConFrecuencia *actual = *cabeza;
    PalabraConFrecuencia *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->palabra, palabra) == 0) {
            // Encontró la palabra a eliminar
            if (anterior == NULL) {
                // Se elimina la cabeza
                *cabeza = actual->next;
            } else {
                anterior->next = actual->next;
            }
            free(actual);
            printf("Palabra eliminada: %s\n", palabra);  // Mensaje de depuración
            return;
        }
        anterior = actual;
        actual = actual->next;
    }
}

int main(int argc, char *argv[]) {
    FILE *file = freopen("salida.txt", "w", stdout);
    // Verificar si se pasó una palabra inicial
    const char *palabra_jugador = NULL;
    if (argc > 1) {
        palabra_jugador = argv[1];  // Tomar la palabra inicial desde el argumento
    }

    // Crear lista de palabras
    PalabraConFrecuencia *lista = NULL;

    // Cargar palabras desde el archivo 3.txt
    // cargarPalabrasDesdeArchivo(&lista, "src/4.txt");
    cargarPalabrasDesdeArchivo(&lista, "3.txt");

    // Definir el juego
    WordleGame juego;                       // Suponiendo que WordleGame está definido en struc_juego.h
    char palabras[MAX_WORDS][WORD_LENGTH];  // Array para almacenar las palabras
    readWords("3.txt", palabras);

    // Inicializar el juego con la lista de palabras y la palabra del jugador
    if (initialize_game(&juego, palabras, palabra_jugador) != 0) {
        fprintf(stderr, "Error al inicializar el juego\n");
        liberarLista(lista);  // Liberar memoria antes de salir
        return EXIT_FAILURE;
    }

    // Ciclo principal para adivinar la palabra
    // Ciclo principal para adivinar la palabra
    char palabra_adivinada[WORD_LENGTH] = {'a', 'i', 'r', 'e', 'o', '\0'};  // Para almacenar la palabra adivinada

    letraPresente letras_correctas[WORD_LENGTH - 1] = {0};
    letraIncorrecta letras_incorrectas[MAX_ALFA - WORD_LENGTH] = {0};  // Letras correctas en las posiciones correctas
    char letras_presentes[WORD_LENGTH - 1] = {0};
    int letrasPresentes = 0;
    int letrasCorrectas = 0;
    int letrasIncorrectas = 0;

    bool posiciones_invalidas[WORD_LENGTH] = {false};

    for (int intento = 0; intento < MAX_ATTEMPTS; intento++) {
        // Elegir la primera palabra de la lista (la más frecuente)
        if (lista != NULL) {
            // Realizar la adivinanza usando la función guess_word
            guess_word(&juego, palabras, palabra_adivinada);

            // Mostrar el estado del juego después de cada intento
            printf("Estado actual: Intentos restantes: %d, ", juego.attempts_left);
            printf("Retroalimentación: ");
            for (int i = 0; i < WORD_LENGTH - 1; i++) {
                printf("%d ", juego.feedback[i]);
            }
            printf("\n");

            // Comprobar si el jugador ha ganado
            if (juego.won) {
                printf("¡Palabra encontrada: %s!\n", palabra_adivinada);
                break;  // Salir si se encuentra la palabra
            }

            // Eliminar la palabra adivinada de la lista
            eliminarPalabra(&lista, palabra_adivinada);

            // Procesar el feedback de la adivinanza
            // Posiciones inválidas

            // Procesar feedback
            for (int i = 0; i < WORD_LENGTH - 1; i++) {
                int estado = juego.feedback[i];

                // Chequear letras correctas
                if (estado == CORRECTO) {
                    bool yaCorrecta = false;

                    // Verificar si la letra ya está en letras_correctas
                    for (int k = 0; k < letrasCorrectas; k++) {
                        if (letras_correctas[k].letra == palabra_adivinada[i]) {
                            yaCorrecta = true;
                            break;  // Si la letra ya está marcada como correcta, salir del bucle
                        }
                    }

                    // Si la letra no está en letras_correctas, se agrega
                    if (!yaCorrecta) {
                        letras_correctas[letrasCorrectas].letra = palabra_adivinada[i];
                        letras_correctas[letrasCorrectas].posicion[i] = true;
                        letrasCorrectas++;
                    }
                }
                // Chequear letras incorrectas
                else if (estado == INCORRECTO) {
                    bool yaIncorrecta = false;

                    // Verificar si la letra ya está en letras_incorrectas
                    for (int k = 0; k < letrasIncorrectas; k++) {
                        if (letras_incorrectas[k].letra == palabra_adivinada[i]) {
                            yaIncorrecta = true;
                            break;  // Si la letra ya está en incorrectas, salir del bucle
                        }
                    }

                    // Si la letra no está en letras_incorrectas, se agrega
                    if (!yaIncorrecta) {
                        letras_incorrectas[letrasIncorrectas].letra = palabra_adivinada[i];
                        letras_incorrectas[letrasIncorrectas].posicion[i] = true;
                        letrasIncorrectas++;  // Marcar posición inválida para la letra
                    }
                }
                // Chequear letras presentes
                else if (estado == PRESENTE) {
                    bool yaPresente = false;

                    // Verificar si la letra ya está en letras_presentes
                    for (int k = 0; k < letrasPresentes; k++) {
                        if (letras_presentes[k] == palabra_adivinada[i]) {
                            yaPresente = true;
                            break;  // Si la letra ya está en presentes, salir del bucle
                        }
                    }

                    // Si la letra no está en letras_presentes, se agrega
                    if (!yaPresente) {
                        letras_presentes[letrasPresentes] = palabra_adivinada[i];
                        letrasPresentes++;
                    }

                    // Actualizar letras_incorrectas y verificar si la letra está en la lista de incorrectas
                    bool letraIncorrecta = false;
                    for (int j = 0; j < letrasIncorrectas; j++) {
                        if (letras_incorrectas[j].letra == palabra_adivinada[i]) {
                            letras_incorrectas[j].posicion[i] = true;  // Marcar la posición como inválida
                            letraIncorrecta = true;                    // Marcar que la letra es incorrecta
                            break;                                     // Salir del bucle una vez encontrada
                        }
                    }

                    // Si la letra no está en letras_incorrectas, la marcamos como incorrecta en esa posición
                    if (!letraIncorrecta) {
                        // Aquí puedes crear una nueva entrada en letras_incorrectas
                        letras_incorrectas[letrasIncorrectas].letra = palabra_adivinada[i];
                        letras_incorrectas[letrasIncorrectas].posicion[i] = true;  // Marcar la posición como inválida
                        letrasIncorrectas++;
                    }
                }
            }

            // Filtrar palabras según la retroalimentación (manteniendo la lista ordenada)

            filtrarPalabras(&lista, letras_presentes, letras_incorrectas, letras_correctas, letrasPresentes, letrasCorrectas, letrasIncorrectas);
            strncpy(palabra_adivinada, lista->palabra, WORD_LENGTH);
            printf("Intento %d: Adivinando la palabra: %s\n", intento + 1, palabra_adivinada);

        } else {
            printf("No hay más palabras para adivinar.\n");
            break;  // Salir si no hay más palabras
        }
    }

    // Liberar memoria
    liberarLista(lista);

    return EXIT_SUCCESS;
}
