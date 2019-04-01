// Importe de librerías

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Ahora, definimos dos estructuras: Person y QueuePeople

struct persona {
  char name[30];
};

// Ahora podemos llamar a la estructura persona con Person (en vez de
// struct persona).
typedef struct persona Person;


typedef struct colapersonas {
  Person** people;
  int length;
} QueuePeople;

// Notar que en QueuePeople hacemos lo mismo que en Person, pero de forma
// abreviada.

int main(int argc, char* argv[]){

    // argc es la cantidad de argumentos de nuestro programa, podemos manejar
    // desde aquí que el uso del programa sea el correcto. En este caso, queremos
    // solo un parámetro (pero indicamos dos porque se incluye el nombre del
    // ejecutable).
    if(argc != 2)
    {
      printf("Mal uso de parámetros.\n");
      exit(2);
    }

    // Inicializamos la cola de personas.
    QueuePeople* cola = malloc(sizeof(QueuePeople));

    // El argumento ingresado corresponde al número de personas. Lo asignamos.
    // Con atoi podemos hacer la conversión string -> int.
    cola -> length = atoi(argv[1]);

    // Solicitamos memoria para la cola de personas. Notar que pedimos un
    // espacio equivalente a la cantidad de personas.
    cola -> people = malloc(sizeof(Person*)*(cola -> length));

    // Declaramos el índice
    int i;

    // Vamos ingresando el nombre de cada persona.
    for (i = 0; i < cola -> length; i++){
      // Solicitamos espacio para la persona nueva.
      Person* persona_nueva = malloc(sizeof(Person));
      printf("Ingrese el nombre de la persona %i: ", i + 1);
      // Asignamos el nombre.
      scanf("%s", persona_nueva -> name);
      // Le asignamos su posición en el arreglo.
      cola -> people[i] = persona_nueva;
    }

    // Ahora, imprimimos la cola.
    printf("La cola de personas es:\n |");
    for (i = 0; i < cola -> length; i++){
      printf("%s|", cola -> people[i] -> name);
    }
    printf("\n");

    // Finalmente, liberamos el espacio de memoria solicitado.
    // Regla fundamental: Un free por cada malloc.
    for (i = 0; i < cola -> length; i++){
      free(cola -> people[i]);
    }
    free(cola -> people);
    free(cola);
}
