#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "struc_juego.h"

typedef struct PalabraConFrecuencia {
    char palabra[WORD_LENGTH];  // Palabra
    int frecuencia;             // Frecuencia de uso
} PalabraConFrecuencia;

typedef struct PriorityQueue {
    PalabraConFrecuencia *heap;  // Arreglo del heap
    int size;                    // Tamaño actual del heap
    int capacity;                // Capacidad del heap
} PriorityQueue;

typedef struct {
    char letra;
    unsigned char posicion;
} LetraPosicionada;

void setPosition(LetraPosicionada *letraPos, int pos) {
    letraPos->posicion |= (1 << pos);  // Establece el bit en la posición 'pos'
}

bool isPositionSet(LetraPosicionada *letraPos, int pos) {
    return (letraPos->posicion & (1 << pos)) != 0;  // Verifica si el bit en 'pos' está activo
}

// Función para crear una cola de prioridad
PriorityQueue *crearColaPrioridad(int capacidad) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    if (!pq) {
        perror("Error al asignar memoria para la cola de prioridad");
        exit(EXIT_FAILURE);
    }
    pq->heap = malloc(sizeof(PalabraConFrecuencia) * capacidad);
    if (!pq->heap) {
        perror("Error al asignar memoria para el heap");
        free(pq);
        exit(EXIT_FAILURE);
    }
    pq->size = 0;
    pq->capacity = capacidad;
    return pq;
}

// Función para liberar la cola de prioridad
void liberarColaPrioridad(PriorityQueue *pq) {
    free(pq->heap);
    free(pq);
}

// Función para mantener la propiedad del heap (up-heap)
void upHeap(PriorityQueue *pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->heap[index].frecuencia <= pq->heap[parent].frecuencia) {
            break;
        }
        // Intercambiar
        PalabraConFrecuencia temp = pq->heap[index];
        pq->heap[index] = pq->heap[parent];
        pq->heap[parent] = temp;
        index = parent;
    }
}

// Función para insertar en el heap
void insertar(PriorityQueue *pq, const char *palabra, int frecuencia) {
    if (pq->size >= pq->capacity) {
        fprintf(stderr, "Cola de prioridad llena\n");
        return;
    }

    PalabraConFrecuencia nuevoNodo;
    strncpy(nuevoNodo.palabra, palabra, WORD_LENGTH - 1);
    nuevoNodo.palabra[WORD_LENGTH - 1] = '\0';  // Asegurarse de que esté terminada en nulo
    nuevoNodo.frecuencia = frecuencia;

    pq->heap[pq->size] = nuevoNodo;
    pq->size++;

    // Mantener la propiedad del heap
    upHeap(pq, pq->size - 1);
}

// Función para mantener la propiedad del heap (down-heap)
void downHeap(PriorityQueue *pq, int index) {
    int leftChild, rightChild, largest;
    while (index < pq->size) {
        leftChild = 2 * index + 1;
        rightChild = 2 * index + 2;
        largest = index;

        if (leftChild < pq->size && pq->heap[leftChild].frecuencia > pq->heap[largest].frecuencia) {
            largest = leftChild;
        }
        if (rightChild < pq->size && pq->heap[rightChild].frecuencia > pq->heap[largest].frecuencia) {
            largest = rightChild;
        }
        if (largest == index) {
            break;
        }
        // Intercambiar
        PalabraConFrecuencia temp = pq->heap[index];
        pq->heap[index] = pq->heap[largest];
        pq->heap[largest] = temp;
        index = largest;
    }
}

// Función para eliminar la raíz del heap
PalabraConFrecuencia eliminarRaiz(PriorityQueue *pq) {
    if (pq->size == 0) {
        fprintf(stderr, "Cola de prioridad vacía\n");
        exit(EXIT_FAILURE);
    }
    PalabraConFrecuencia raiz = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];  // Mover el último elemento a la raíz
    pq->size--;

    // Mantener la propiedad del heap
    downHeap(pq, 0);

    return raiz;
}

// Modificación de la función filtrarPalabras
void filtrarPalabras(PriorityQueue *pq, char letras_presentes[], LetraPosicionada letras_incorrectas[], LetraPosicionada letras_correctas[], int letrasPresentes, int letrasCorrectas, int letrasIncorrectas) {
    PriorityQueue *nuevaPQ = crearColaPrioridad(pq->capacity);  // Crear una nueva cola de prioridad para almacenar palabras válidas

    for (int i = 0; i < pq->size; i++) {
        PalabraConFrecuencia actual = pq->heap[i];
        bool esValida = true;

        // Verificar letras correctas
        for (int i = 0; i < WORD_LENGTH - 1 && esValida; i++) {
            for (int j = 0; j < letrasCorrectas; j++) {
                if (isPositionSet(&letras_correctas[j], i)) {  // Revisar si la posición está marcada como correcta
                    if (letras_correctas[j].letra != actual.palabra[i]) {
                        printf("le falta letra correcta\n");
                        esValida = false;
                        break;
                    }
                }
            }

            // Verificar letras incorrectas
            for (int j = 0; j < letrasIncorrectas && esValida; j++) {
                if (isPositionSet(&letras_incorrectas[j], i)) {  // Revisar si la posición está marcada como incorrecta
                    if (letras_incorrectas[j].letra == actual.palabra[i] && letras_incorrectas[j].letra != '\0') {
                        printf("tiene letra incorrecta\n");
                        esValida = false;
                        break;
                    }
                }
            }
        }

        // Verificar que todas las letras presentes estén en la palabra, en cualquier posición
        for (int j = 0; j < letrasPresentes && esValida; j++) {
            bool presente = false;
            for (int i = 0; i < WORD_LENGTH - 1; i++) {
                if (letras_presentes[j] == actual.palabra[i]) {
                    presente = true;
                    break;
                }
            }
            if (!presente) {
                printf("le falta letra presente\n");
                esValida = false;  // Si una letra presente no está en la palabra, la palabra es inválida
            }
        }

        // Si la palabra es válida, agregarla a la nueva cola de prioridad
        if (esValida) {
            insertar(nuevaPQ, actual.palabra, actual.frecuencia);
        } else {
            printf("Palabra eliminada: %s\n", actual.palabra);  // Mensaje de depuración
        }
    }

    // Reemplazar la cola de prioridad original con la nueva
    free(pq->heap);  // Liberar la memoria de la antigua
    pq->heap = nuevaPQ->heap;
    pq->size = nuevaPQ->size;
    free(nuevaPQ);  // Liberar la nueva cola de prioridad
}

void cargarPalabrasDesdeArchivo(PriorityQueue **pq, const char *nombre_archivo) {
    FILE *file = fopen(nombre_archivo, "r");
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo %s\n", nombre_archivo);
        return;
    }

    char linea[256];  // Buffer para leer líneas del archivo
    char palabra[WORD_LENGTH];
    int frecuencia;

    while (fgets(linea, sizeof(linea), file)) {
        // Cambiamos el separador a ',' y analizamos la línea leída
        if (sscanf(linea, "%49[^,],%d", palabra, &frecuencia) != 2) {
            fprintf(stderr, "Formato de línea inválido: %s", linea);
            continue;  // Ignorar líneas mal formateadas
        }

        // Verificar que la frecuencia sea positiva
        if (frecuencia < 0) {
            fprintf(stderr, "Frecuencia negativa encontrada para la palabra: %s\n", palabra);
            continue;  // Saltar esta entrada
        }

        // Asegúrate de que no hay desbordamiento de buffer
        if (strlen(palabra) >= WORD_LENGTH) {
            fprintf(stderr, "Palabra demasiado larga: %s\n", palabra);
            continue;  // Ignorar palabras demasiado largas
        }

        // Insertar la palabra en la cola de prioridad
        insertar(*pq, palabra, frecuencia);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    FILE *file = freopen("salida.txt", "w", stdout);
    // Verificar si se pasó una palabra inicial
    const char *palabra_jugador = NULL;
    if (argc > 1) {
        palabra_jugador = argv[1];  // Tomar la palabra inicial desde el argumento
    }

    // Crear cola de prioridad para almacenar palabras
    PriorityQueue *pq = crearColaPrioridad(MAX_WORDS);

    // Cargar palabras desde el archivo 3.txt
    cargarPalabrasDesdeArchivo(&pq, "3.txt");  // Asegúrate de que esta función cargue palabras en la cola de prioridad

    // Definir el juego
    WordleGame juego;                       // Suponiendo que WordleGame está definido en struc_juego.h
    char palabras[MAX_WORDS][WORD_LENGTH];  // Array para almacenar las palabras
    readWords("3.txt", palabras);

    // Inicializar el juego con la lista de palabras y la palabra del jugador
    if (initialize_game(&juego, palabras, palabra_jugador) != 0) {
        fprintf(stderr, "Error al inicializar el juego\n");
        liberarColaPrioridad(pq);  // Liberar memoria antes de salir
        return EXIT_FAILURE;
    }

    // Ciclo principal para adivinar la palabra
    char palabra_adivinada[WORD_LENGTH] = {'a', 'i', 'r', 'e', 'o', '\0'};  // Para almacenar la palabra adivinada
    LetraPosicionada letras_correctas[WORD_LENGTH - 1] = {0};
    LetraPosicionada letras_incorrectas[MAX_ALFA - WORD_LENGTH] = {0};  // Letras correctas en las posiciones correctas
    char letras_presentes[WORD_LENGTH - 1] = {0};
    int letrasPresentes = 0;
    int letrasCorrectas = 0;
    int letrasIncorrectas = 0;

    for (int intento = 0; intento < MAX_ATTEMPTS; intento++) {
        if (intento == 0) {
            // Primer intento: adivinar "aireo"
            strncpy(palabra_adivinada, "aireo", WORD_LENGTH);
            printf("Intento %d: Adivinando la palabra fija: %s\n", intento + 1, palabra_adivinada);
        } else {
            // Elegir la primera palabra de la cola de prioridad (la más frecuente) a partir del segundo intento
            if (pq->size > 0) {
                // Obtener la palabra más frecuente (raíz del heap)
                PalabraConFrecuencia palabraActual = eliminarRaiz(pq);
                strncpy(palabra_adivinada, palabraActual.palabra, WORD_LENGTH);  // Copiar la palabra adivinada

                printf("Intento %d: Adivinando la palabra: %s con frecuencia %d\n", intento + 1, palabraActual.palabra, palabraActual.frecuencia);
            } else {
                printf("No hay más palabras para adivinar.\n");
                break;  // Salir si no hay más palabras
            }
        }

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

        // Procesar el feedback de la adivinanza
        for (int i = 0; i < WORD_LENGTH - 1; i++) {
            int estado = juego.feedback[i];

            // Chequear letras correctas
            if (estado == CORRECTO) {
                bool yaCorrecta = false;

                // Verificar si la letra ya está en letras_correctas
                for (int k = 0; k < letrasCorrectas; k++) {
                    if (letras_correctas[k].letra == palabra_adivinada[i]) {
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
                    letras_correctas[letrasCorrectas].letra = palabra_adivinada[i];
                    setPosition(&letras_correctas[letrasCorrectas], i);  // Marcar posición como correcta
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
                        break;
                    }
                }

                // Verificar si la letra está en letras_presentes
                bool enPresentes = false;
                for (int j = 0; j < letrasPresentes; j++) {
                    if (letras_presentes[j] == palabra_adivinada[i]) {
                        enPresentes = true;
                        break;
                    }
                }

                // Solo marcar como incorrecta si no está en letras_correctas
                if (!yaIncorrecta && !enPresentes) {
                    bool letraEsCorrectaEnOtraPosicion = false;

                    // Verificar si la letra es correcta en alguna otra posición
                    for (int k = 0; k < letrasCorrectas; k++) {
                        if (letras_correctas[k].letra == palabra_adivinada[i]) {
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
                        letras_incorrectas[letrasIncorrectas].letra = palabra_adivinada[i];
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
                    if (letras_presentes[k] == palabra_adivinada[i]) {
                        yaPresente = true;
                        break;
                    }
                }

                // Si la letra no está en letras_presentes, se agrega
                if (!yaPresente) {
                    letras_presentes[letrasPresentes] = palabra_adivinada[i];
                    letrasPresentes++;
                }

                // Verificar si la letra no está en letras_incorrectas, marcarla como incorrecta en la posición actual
                bool letraIncorrecta = false;
                for (int j = 0; j < letrasIncorrectas; j++) {
                    if (letras_incorrectas[j].letra == palabra_adivinada[i]) {
                        letraIncorrecta = true;
                        setPosition(&letras_incorrectas[j], i);  // Marcar la posición como inválida
                        break;
                    }
                }

                // Si la letra no está en letras_incorrectas, la agregamos y marcamos la posición actual como incorrecta
                if (!letraIncorrecta) {
                    letras_incorrectas[letrasIncorrectas].letra = palabra_adivinada[i];
                    setPosition(&letras_incorrectas[letrasIncorrectas], i);  // Marcar la posición actual como incorrecta
                    letrasIncorrectas++;
                }
            }
        }
        filtrarPalabras(pq, letras_presentes, letras_incorrectas, letras_correctas, letrasPresentes, letrasCorrectas, letrasIncorrectas);
        // Filtrar palabras según la retroalimentación
    }

    // Liberar memoria
    liberarColaPrioridad(pq);

    return EXIT_SUCCESS;
}
