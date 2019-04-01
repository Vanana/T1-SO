#include <stdlib.h>
#include <stdio.h>

// Definición de enumeradores para los estados de una persona.
typedef enum state {AWAKE, ASLEEP, DEAD} State;

// Estructura de una persona.
typedef struct person {
  int age;
  State state;
} Person;

// Notar que, en ambos casos, se hace uso de typedef para poder declarar el
// enumerador y la estructura como objetos del programa.


int main(){

    // Programa que imprime el estado de una persona.

    // Forma 1 - Trabajamos con el contenido directo de la estructura, pero
    // tratamos al puntero como un arreglo (para nada atractivo).
    Person* p1 = malloc(sizeof(Person));
    p1[0].age = 50;
    p1[0].state = AWAKE;
    printf("State: %i\n",p1[0].state);

    // Forma 2 - Trabajamos con el contenido directo de la estructura, pero
    // solicitamos su contenido con el operador *.
    Person* p2 = malloc(sizeof(Person));
    (*p2).age = 1;
    (*p2).state = ASLEEP;
    printf("State: %i\n",(*p2).state);

    // Forma 3 - Trabajamos directamente con el puntero.
    Person* p3 = malloc(sizeof(Person));
    p3 -> age = 25;
    p3 -> state = DEAD;
    printf("State: %i\n",p3 -> state);

    // Notar que las impresiones de los estados son números, demostrando que el
    // contenido del enumerador es, efectivamente, numérico.
    
    return 0;
}
