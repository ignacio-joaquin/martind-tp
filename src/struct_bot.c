#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "struct_bot.h"

Heap *crearHeap(int capacidad) {
    Heap *heap = malloc(sizeof(Heap));
    if (!heap) {
        perror("Error al asignar memoria para la cola de prioridad");
        exit(EXIT_FAILURE);
    }
    heap->data = malloc(sizeof(PalabraConFrecuencia) * capacidad);
    if (!heap->data) {
        perror("Error al asignar memoria para el data");
        free(heap);
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = capacidad;
    return heap;
}

void liberarHeap(Heap *heap) {
    free(heap->data);
    free(heap);
}

void upheap(Heap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index].frecuencia <= heap->data[parent].frecuencia) {
            break;
        }
        // Intercambiar
        PalabraConFrecuencia temp = heap->data[index];
        heap->data[index] = heap->data[parent];
        heap->data[parent] = temp;
        index = parent;
    }
}

// Función para insertar en el data
void insertar(Heap *heap, const char *palabra, int frecuencia) {
    if (heap->size >= heap->capacity) {
        fprintf(stderr, "Cola de prioridad llena\n");
        return;
    }

    PalabraConFrecuencia nuevoNodo;
    strncpy(nuevoNodo.palabra, palabra, WORD_LENGTH - 1);
    nuevoNodo.palabra[WORD_LENGTH - 1] = '\0';  // Asegurarse de que esté terminada en nulo
    nuevoNodo.frecuencia = frecuencia;

    heap->data[heap->size] = nuevoNodo;
    heap->size++;

    // Mantener la propiedad del data
    upheap(heap, heap->size - 1);
}

// Función para mantener la propiedad del data (down-data)
void downheap(Heap *heap, int index) {
    int leftChild, rightChild, largest;
    while (index < heap->size) {
        leftChild = 2 * index + 1;
        rightChild = 2 * index + 2;
        largest = index;

        if (leftChild < heap->size && heap->data[leftChild].frecuencia > heap->data[largest].frecuencia) {
            largest = leftChild;
        }
        if (rightChild < heap->size && heap->data[rightChild].frecuencia > heap->data[largest].frecuencia) {
            largest = rightChild;
        }
        if (largest == index) {
            break;
        }
        // Intercambiar
        PalabraConFrecuencia temp = heap->data[index];
        heap->data[index] = heap->data[largest];
        heap->data[largest] = temp;
        index = largest;
    }
}

// Función para eliminar la raíz del data
PalabraConFrecuencia eliminarRaiz(Heap *heap) {
    if (heap->size == 0) {
        fprintf(stderr, "Cola de prioridad vacía\n");
        exit(EXIT_FAILURE);
    }
    PalabraConFrecuencia raiz = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];  // Mover el último elemento a la raíz
    heap->size--;

    // Mantener la propiedad del data
    downheap(heap, 0);

    return raiz;
}

void setPosition(LetraPosicionada *letraPos, int pos) {
    letraPos->posicion |= (1 << pos);  // Establece el bit en la posición 'pos'
}

bool isPositionSet(LetraPosicionada *letraPos, int pos) {
    return (letraPos->posicion & (1 << pos)) != 0;  // Verifica si el bit en 'pos' está activo
}