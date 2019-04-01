// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>

// Importamos el archivo .h correspondiente
#include "linked_list.h"

//////////////////////////// Funciones privadas ///////////////////////////

// Las funciones que tienen el keyword "static" solo pueden ser llamadas desde
// el archivo en el que fueron definidas. Sirve por ejemplo para hacer funciones
// internas ajenas al uso que se le va a dar a la estructura.
// Por ejemplo la funcion node_init solo se va a usar al agregar elementos a la
// lista ligada pero no se va a llamar desde el programa principal

/** Funcion que crea un nodo de la lista ligada a partir de un valor */
static Process* node_init(uint32_t PID, uint32_t start_time, char name, uint32_t priority, uint32_t cant, uint32_t** A,  uint32_t** B)
{
  // Pido la memoria para el nodo
  Process* process = malloc(sizeof(Process));

  // Inicializamos
  process -> PID = PID;
  process -> start_time = start_time;
  process -> name = name;
  process -> priority = priority;
  process -> state = READY;
  process -> turn = 0;
  process -> cant = cant;
  process -> A = A;
  process -> B = B;

  // Inicializo los referencias en NULL
  process -> last = NULL;
  process -> next = NULL;

  // Le doy el valor correspondiente
  process -> value = value;

  // Retorno el nodo
  return process;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void nodes_destroy(Process* process)
{
  // Si el nodo no es NULL, llama recursivamente al siguiente
  // y libero el nodo actual
  if (process)
  {
    nodes_destroy(process -> next);
    free(process);
  }
}

//////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
Queue* ll_init()
{
  // Primero pido la memoria para la lista ligada
  Queue* ll = malloc(sizeof(Queue));

  // Luego inicializo en 0 el contador y en NULL las referencias
  ll -> length = 0;
  ll -> process = NULL;
  // Retorno la lista ligada
  return ll;
}

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(Queue* ll, PID)
{
  uint32_t* A = {2,1}
  uint32_t* B = {3}
  char name[]

  // Primero creo un nodo nuevo
  Process* process = node_init(PID, 0, name, priority, cant, *A, *B);

  // Luego lo agrego a la lista ligada
  if (!ll -> count)
  {
    // En el caso de que este vacia la lista dejo el nodo como inicial y final
    ll -> start = node;
    ll -> end = node;
  }
  else
  {
    // En otro caso lo conecto con el ultimo nodo de la lista y lo dejo como
    // nodo final de la lista
    ll -> end -> next = node;
    node -> last = ll -> end;
    ll -> end = node;
  }

  // Sumo 1 al numero de nodos
  ll -> count++;
}

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
int ll_get(LinkedList* ll, int position)
{
  // Si no hay suficientes nodos, hago un error
  if (position >= ll -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la lista\n");
    // Este comando hace terminar el programa
    exit(1);
  }

  // Me muevo por los nodos hasta encontrar la posicion que busco
  Node* actual = ll -> start;
  for (int i = 0; i < position; i++)
  {
    actual = actual -> next;
  }

  // Retoro el valor del nodo correspondiente
  return actual -> value;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(LinkedList* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  nodes_destroy(ll -> start);

  // Luego libero la memoria de la lista
  free(ll);
}
