#include "bot.h"


// Modificación de la función filtrarPalabras
void filtrarPalabras(Heap *heap, char letras_presentes[], LetraPosicionada letras_incorrectas[], LetraPosicionada letras_correctas[], 
                     int cantLetrasPresentes, int letrasCorrectas, int letrasIncorrectas, int desfasajeIncorrectas) {
    
    int nuevaSize = 0;  

    for (int i = 0; i < heap->size; i++) {
        PalabraConFrecuencia actual = heap->data[i];
        bool esValida = true;

        // Verificar letras correctas e incorrectas
        for (int j = 0; j < WORD_LENGTH - 1 && esValida; j++) {
            // Verificar si la letra es correcta en su posición
            for (int k = 0; k < letrasCorrectas; k++) {
                if (isPositionSet(&letras_correctas[k], j) && letras_correctas[k].letra != actual.palabra[j]) {
                    esValida = false;
                    break;
                }
            }

            // Verificar si la letra es incorrecta en su posición
            for (int k = 0 + desfasajeIncorrectas; k < letrasIncorrectas && esValida; k++) {
                if (isPositionSet(&letras_incorrectas[k], j) && letras_incorrectas[k].letra == actual.palabra[j]) {
                    esValida = false;
                    break;
                }
            }
        }

        // Verificar que todas las letras presentes estén en la palabra
        for (int j = 0; j < cantLetrasPresentes && esValida; j++) {
            if (strchr(actual.palabra, letras_presentes[j]) == NULL) {  // Usamos strchr para verificar si la letra está en cualquier posición
                esValida = false;
            }
        }

        // Si la palabra es válida, la movemos al frente del heap
        if (esValida) {
            heap->data[nuevaSize] = actual;
            nuevaSize++;
        }
    }

    // Ajustamos el tamaño del heap al nuevo tamaño filtrado
    heap->size = nuevaSize;
}

// Función para verificar si una letra ya está en un arreglo de letras
bool letraEnArreglo(char letra, LetraPosicionada *arreglo, int tamano) {
    for (int k = 0; k < tamano; k++) {
        if (arreglo[k].letra == letra) {
            return true;
        }
    }
    return false;
}

// Función para verificar si una letra está en un arreglo simple de caracteres
bool letraEnArraySimple(char letra, char *arreglo, int tamano) {
    for (int k = 0; k < tamano; k++) {
        if (arreglo[k] == letra) {
            return true;
        }
    }
    return false;
}

// Función para agregar una letra a un LetraPosicionada
void agregarLetra(char letra, int posicion, LetraPosicionada *letras, int *cantLetras) {
    letras[*cantLetras].letra = letra;
    setPosition(&letras[*cantLetras], posicion);
    (*cantLetras)++;
}


// Función principal para procesar el feedback
void procesarFeeback(WordleGame game, char *player_word, LetraPosicionada *letras_correctas, int *cantLetrasCorrectas,
                     LetraPosicionada *letras_incorrectas, int *cantLetrasIncorrectas,
                     char *letras_presentes, int *cantLetrasPresentes) {
    for (int i = 0; i < WORD_LENGTH - 1; i++) {
        int estado = game.feedback[i];
        char letra = player_word[i];

        switch (estado) {
            case CORRECTO: {
                if (!letraEnArreglo(letra, letras_correctas, *cantLetrasCorrectas)) {
                    agregarLetra(letra, i, letras_correctas, cantLetrasCorrectas);
                } else {
                    // Si ya está, marcar la posición si no ha sido marcada
                    for (int k = 0; k < *cantLetrasCorrectas; k++) {
                        if (letras_correctas[k].letra == letra && !isPositionSet(&letras_correctas[k], i)) {
                            setPosition(&letras_correctas[k], i);
                        }
                    }
                }
                break;
            }
            case INCORRECTO: {
                if (!letraEnArreglo(letra, letras_incorrectas, *cantLetrasIncorrectas) &&
                    !letraEnArraySimple(letra, letras_presentes, *cantLetrasPresentes)) {
                    bool letraEsCorrectaEnOtraPosicion = false;
                    for (int k = 0; k < *cantLetrasCorrectas; k++) {
                        if (letras_correctas[k].letra == letra) {
                            letraEsCorrectaEnOtraPosicion = true;
                            if (!isPositionSet(&letras_correctas[k], i)) {
                                agregarLetra(letra, i, letras_incorrectas, cantLetrasIncorrectas);
                            }
                            break;
                        }
                    }

                    if (!letraEsCorrectaEnOtraPosicion) {
                        agregarLetra(letra, i, letras_incorrectas, cantLetrasIncorrectas);
                    }
                }
                break;
            }
            case PRESENTE: {
                if (!letraEnArraySimple(letra, letras_presentes, *cantLetrasPresentes)) {
                    letras_presentes[*cantLetrasPresentes] = letra;
                    (*cantLetrasPresentes)++;
                }

                bool letraIncorrecta = letraEnArreglo(letra, letras_incorrectas, *cantLetrasIncorrectas);
                if (letraIncorrecta) {
                    for (int j = 0; j < *cantLetrasIncorrectas; j++) {
                        if (letras_incorrectas[j].letra == letra) {
                            setPosition(&letras_incorrectas[j], i);
                            break;
                        }
                    }
                } else {
                    agregarLetra(letra, i, letras_incorrectas, cantLetrasIncorrectas);
                }
                break;
            }
        }
    }
}