// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>
/*funciones de strings como strcpy, strcat o strlen*/
#include <string.h>

#include <stdint.h>


// Importamos el archivo .h correspondiente
#include "queue.h"

//////////////////////////// Funciones privadas ///////////////////////////

// Las funciones que tienen el keyword "static" solo pueden ser llamadas desde
// el archivo en el que fueron definidas. Sirve por ejemplo para hacer funciones
// internas ajenas al uso que se le va a dar a la estructura.
// Por ejemplo la funcion process_init solo se va a usar al agregar elementos a la
// lista ligada pero no se va a llamar desde el programa principal

/** Funcion que crea un nodo de la lista ligada a partir de un valor */
static Process* process_init(uint32_t value, char* name, uint32_t* arreglo, uint32_t time_start)
{
  // Pido la memoria para el nodo
  Process* process = malloc(sizeof(Process));

  // Inicializo el estado
  process -> state = READY;
  // Inicializo los Atributos
  strcpy(process -> name, name);
  process -> arreglo = arreglo;
  process -> time_start = time_start;
  // Inicializo los referencias en NULL
  process -> last = NULL;
  process -> next = NULL;

  // Le doy el valor correspondiente
  process -> value = value;

  // Retorno el nodo
  return process;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void processs_destroy(Process* process)
{
  // Si el nodo no es NULL, llama recursivamente al siguiente
  // y libero el nodo actual
  if (process)
  {
    processs_destroy(process -> next);
    free(process);
  }
}

//////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
AllQueue* ll_init()
{
  // Primero pido la memoria para la lista ligada
  AllQueue* ll = malloc(sizeof(AllQueue));

  // Luego inicializo en 0 el contador y en NULL las referencias
  ll -> count = 0;
  ll -> start = NULL;
  ll -> end = NULL;

  // Retorno la lista ligada
  return ll;
}

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(AllQueue* ll, uint32_t value, uint32_t time_start, char* name, uint32_t* arreglo)
{
  // Primero creo un nodo nuevo
  Process* process = process_init(value, name, arreglo, time_start);

  Process* aux_process;
  // Luego lo agrego a la lista ligada
  if (!ll -> count)
  {
    // En el caso de que este vacia la lista dejo el nodo como inicial y final
    ll -> start = process;
    ll -> end = process;
  }
  else
  {
    // Lo vamos a ingresar ordenadamente segun su tiempo de llegada
    aux_process = ll -> start;

    if (aux_process -> time_start > time_start){
      // soy el menor, soy el primero
      ll -> start = process;
      aux_process -> last = process;
      process -> next = aux_process;
    }

    else{  // si voy despues de aux process
      int i = 0;
      while (i < ll-> count){
        // si hay un nodo despues de aux process
        if (aux_process -> next){
          // reviso si voy al medio de aux process y aux process next
          if (aux_process-> next-> time_start < time_start){
            process -> next = aux_process -> next;
            process -> next -> last = process;
            aux_process -> next = process;
            process -> last = aux_process;
            break;
          }
          else{
            // si no voy al medio, significa que puede que vaya despues
            aux_process = aux_process-> next;
          }
        }
        else{
          // si no hay otro process es por que voy al final
          aux_process -> next = process;
          process -> last = aux_process;
          ll -> end = process;
          break;
        }
        i ++;
      }
    }

  }

  // Sumo 1 al numero de nodos
  ll -> count++;
}

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
Process* ll_get(AllQueue* ll, uint32_t position)
{
  // Si no hay suficientes nodos, hago un error
  if (position >= ll -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la lista\n");
    // Este comando hace terminar el programa
    exit(1);
  }

  // Me muevo por los nodos hasta encontrar la posicion que busco
  Process* actual = ll -> start;
  for (uint32_t i = 0; i < position; i++)
  {
    actual = actual -> next;
  }
  // Retoro el valor del nodo correspondiente
  return actual;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(AllQueue* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  processs_destroy(ll -> start);

  // Luego libero la memoria de la lista
  free(ll);
}
