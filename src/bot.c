#include "bot.h"


// Modificación de la función filtrarPalabras
void filtrarPalabras(Heap *heap, char letras_presentes[], LetraPosicionada letras_incorrectas[], LetraPosicionada letras_correctas[], int letrasPresentes, int letrasCorrectas, int letrasIncorrectas, int desfasajeIncorrectas) {
    Heap *nuevaheap = crearHeap(heap->capacity);  // Crear una nueva cola de prioridad para almacenar palabras válidas

    for (int i = 0; i < heap->size; i++) {
        PalabraConFrecuencia actual = heap->data[i];
        bool esValida = true;

        // Verificar letras correctas
        for (int i = 0 ; i < WORD_LENGTH - 1 && esValida; i++) {
            for (int j = 0; j < letrasCorrectas; j++) {
                if (isPositionSet(&letras_correctas[j], i)) {  // Revisar si la posición está marcada como correcta
                    if (letras_correctas[j].letra != actual.palabra[i]) {
                        esValida = false;
                        break;
                    }
                }
            }

            // Verificar letras incorrectas
            for (int j = 0 + desfasajeIncorrectas; j < letrasIncorrectas && esValida; j++) {
                if (isPositionSet(&letras_incorrectas[j], i)) {  // Revisar si la posición está marcada como incorrecta
                    if (letras_incorrectas[j].letra == actual.palabra[i]) {
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
                esValida = false;  // Si una letra presente no está en la palabra, la palabra es inválida
            }
        }

        // Si la palabra es válida, agregarla a la nueva cola de prioridad
        if (esValida) {
            insertar(nuevaheap, actual.palabra, actual.frecuencia);
        }
    }

    // Reemplazar la cola de prioridad original con la nueva
    free(heap->data);  // Liberar la memoria de la antigua
    heap->data = nuevaheap->data;
    heap->size = nuevaheap->size;
    free(nuevaheap);  // Liberar la nueva cola de prioridad
}