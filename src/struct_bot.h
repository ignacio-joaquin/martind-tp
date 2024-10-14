#ifndef STRUCT_BOT_H
#define STRUCT_BOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struc_juego.h"

// Estructura para almacenar palabras con su frecuencia
typedef struct PalabraConFrecuencia {
    char palabra[WORD_LENGTH];  // Palabra
    int frecuencia;              // Frecuencia de uso
} PalabraConFrecuencia;

// Estructura de la cola de prioridad (Heap)
typedef struct Heap {
    PalabraConFrecuencia *data;  // Arreglo del data
    int size;                    // Tamaño actual del data
    int capacity;                // Capacidad del data
} Heap;

// Estructura para letras y sus posiciones
typedef struct {
    char letra;
    unsigned char posicion;  // Posiciones en formato comprimido
} LetraPosicionada;

// Prototipos de funciones
Heap *crearHeap(int capacidad);
void liberarHeap(Heap *heap);
void insertar(Heap *heap, const char *palabra, int frecuencia);
PalabraConFrecuencia eliminarRaiz(Heap *heap);

void setPosition(LetraPosicionada *letraPos, int pos);
bool isPositionSet(LetraPosicionada *letraPos, int pos);


#endif // STRUCT_BOT_H
