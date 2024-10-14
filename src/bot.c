#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "struc_juego.h"
#include "struct_bot.h"


// Modificación de la función filtrarPalabras
void filtrarPalabras(Heap *heap, char letras_presentes[], LetraPosicionada letras_incorrectas[], LetraPosicionada letras_correctas[], int letrasPresentes, int letrasCorrectas, int letrasIncorrectas, int desfasajePresentes, int desfasajeCorrectas, int desfasajeIncorrectas) {
    Heap *nuevaheap = crearHeap(heap->capacity);  // Crear una nueva cola de prioridad para almacenar palabras válidas

    for (int i = 0; i < heap->size; i++) {
        PalabraConFrecuencia actual = heap->data[i];
        bool esValida = true;

        // Verificar letras correctas
        for (int i = 0 + desfasajeCorrectas; i < WORD_LENGTH - 1 && esValida; i++) {
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
            for (int j = 0 + desfasajeIncorrectas; j < letrasIncorrectas && esValida; j++) {
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
        for (int j = 0 + desfasajePresentes; j < letrasPresentes && esValida; j++) {
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
            insertar(nuevaheap, actual.palabra, actual.frecuencia);
        } else {
            printf("Palabra eliminada: %s\n", actual.palabra);  // Mensaje de depuración
        }
    }

    // Reemplazar la cola de prioridad original con la nueva
    free(heap->data);  // Liberar la memoria de la antigua
    heap->data = nuevaheap->data;
    heap->size = nuevaheap->size;
    free(nuevaheap);  // Liberar la nueva cola de prioridad
}