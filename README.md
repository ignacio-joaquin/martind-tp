# Trabajo Practico 2

El trabajo practico fue resuelto por Gino Pirani e Ignacio Joaquin conforme a las pautas estipuladas en el enunciado
Contenidos de este README.md:
- Estructura de Modulos
- Funcionamiento Basico
- **✨Estructuras de datos✨**
- Micelaneas


## Estructura de Modulos

El proyecto consta de 5 modulos
1. bot *(bot.c y bot.h)*
2. juego *(juego.c y juego.h)*
3. main *(main.c)*
4. estructuras del juego *(struc_juego.h)*
5. estructuras del bot *(struct_bot.c y struct_bot.h)*

- ### Bot
    Como el nombre sugiere, este modulo es el encargado del funcionamiento del "bot" (asi es como llamamos a la parte del codigo que se dedica a sugerir las palabras)
- ### Juego
    Este es el archivo "core" por asi decirlo, aqui reside la logica que dicta como funciona el juego.

- ### Main
    Este archivo simplemente contiene la funcion main. Por lo tanto aqui es donde todas las funciones de todos los modulos son juntadas y atadas con alambre y un poco de magia oscura, dando asi el resultado final.

- ### Estructuras del juego
    Es en este archivo donde se almacenan las **✨Estructuras de datos✨** utilizadas en el modulo Juego. Por lo tanto estas estructuras son impresindibles para el proyecto, por eso Juego y Estructuras del juego son considerados los archivos nucleo del proyecto

- ### Estructuras del bot
    Similar al modulo previo en su funcion, almacena las estructuras de datos y funciones de las mismas que son requeridas por el bot

## Funcionamiento Basico

La función main actúa como el núcleo del juego, donde se orquesta la interacción entre los diferentes módulos y se implementa la lógica de la partida. Al iniciar, se validan los argumentos de entrada, asegurando que el usuario proporcione la palabra secreta y el modo de juego deseado. Si se activa el modo de recomendación, se cargan las palabras desde un archivo en un heap.

Posteriormente, se inicializa la estructura del juego, estableciendo la palabra secreta y configurando los parámetros iniciales, como el número de intentos permitidos. Con el juego preparado, se entra en un bucle que se repite hasta que se agoten los intentos o el jugador adivine la palabra correcta.

Dentro de este bucle, el bot ofrece sugerencias de palabras, comenzando con una recomendación predeterminada en el primer intento y utilizando la palabra *valida* más frecuente en intentos posteriores. Notese que ante dos palabras con la misma frecuencia el bot no hace ningun tipo de desempate extra, ya que no logramos encontrar ningun criterio confiable para discernir entre palabras, por lo tanto el bot simplemente elije la que este primero en el heap.  A continuación, se solicita al jugador que ingrese su palabra de cinco letras, la cual es validada antes de proceder.

Después de cada intento, se proporciona retroalimentación sobre las letras correctas, presentes o incorrectas, permitiendo al jugador ajustar su estrategia. Este proceso continúa hasta que el jugador gane, perdiendo todos los intentos o alcance la solución correcta. Al final de la partida, se liberan los recursos utilizados por el heap, asegurando una correcta gestión de la memoria.

## ✨Estructuras de Datos✨
- ### Estructuras del Juego
    
    Las estructuras de datos utilizadas en el módulo del juego son esenciales para gestionar el estado del juego y las interacciones del jugador:
    
        WordleGame: Estructura principal que contiene:
            char letters[WORD_LENGTH]: La palabra secreta.
            int attempts_left: Número de intentos restantes.
            bool won: Indica si el jugador ha ganado.
            int feedback[WORD_LENGTH - 1]: Almacena la retroalimentación del último intento.
    
        LetraPosicionada: Almacena letras junto con sus posiciones en el juego.
            char letra: La letra almacenada.
            int posicion: La posición de la letra.
    
    No creemos necesario dar explicaciones sobre porque las estructuras de datos son elegidas de esta manera, ya que es bastante trivial y no tiene un gran impacto en el codigo.

- ### Estructuras del Bot
    Este módulo se encarga de gestionar las interacciones del bot en el juego, facilitando la recomendación de palabras basadas en la frecuencia de uso. Contiene las siguientes estructuras y funciones esenciales:
    
    1. PalabraConFrecuencia: Esta estructura almacena cada palabra junto con su frecuencia de aparición. Esto permite al bot seleccionar palabras que son más comunes, mejorando la eficacia de sus recomendaciones.
    
    1. Heap: Implementa un heap para gestionar las palabras y sus frecuencias de manera eficiente. El heap permite insertar nuevas palabras y eliminar la palabra más frecuente de forma rápida, optimizando la toma de decisiones del bot durante el juego. Se utilizan tres campos:
       1. data: un arreglo dinámico que contiene las palabras y sus frecuencias.
        1. size: que indica cuántas palabras hay actualmente en el heap.
        1. capacity: que define la capacidad máxima del heap, asegurando que se puede ampliar cuando sea necesario. 
        
    1. LetraPosicionada: Esta estructura vincula una letra con su posición en el juego. Utiliza un formato comprimido para almacenar la posición de forma eficiente, lo que facilita la gestión de letras correctas e incorrectas durante el proceso de adivinanza.
    
    El módulo proporciona funciones para crear y liberar el heap, insertar nuevas palabras y eliminar la raíz del heap, así como para establecer y verificar posiciones de letras. Además, incluye una función para cargar palabras desde un archivo, permitiendo una fácil expansión de la base de datos del bot. 
    
    Se elijo un heap como estructura principal para el bot por sobre un arbol AVL (las dos opciones en consideracion) dado que la principal ventaja del arbol AVL, la busqueda de elementos, no es requerida en este caso, por lo tanto un heap implementado con un arbol binario es suficiente.

## Miscelaneo
Incluimos un **Makefile** para simplificar la compilacion, ademas encontramos algunos duplicados en 3.txt (lineas 1184 y 1185 "Colon" por ejemplo).
El programa requiere dos argumentos en la linea de comandos, el primero la palabra secreta y el segundo un 0 para jugar sin ayudas o un 1 para jugar con ayudas.
Encontramos este TP un desafio interesante, quiza hubiera sido mas interesante algo mas de tiempo para pulir cosas, pero nos encontramos satisfechos con el resultado.
Como dato curioso descubrimos que el "bot" solo tenia que hacer recomendaciones el domingo 13, lo cual fue un alivio porque hacer que el bot gane con solo 6 intentos era mucho mas complicado en casi todas las palabras con frecuencia menor a 5.


Por cualquier duda nos encontramos a su disposicion