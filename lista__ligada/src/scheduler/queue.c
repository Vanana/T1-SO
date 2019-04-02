// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>
/*funciones de strings como strcpy, strcat o strlen*/
#include <string.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de bool */
#include <stdbool.h>
// Importamos el archivo .h correspondiente
#include "queue.h"

//////////////////////////// Funciones privadas ///////////////////////////

// Las funciones que tienen el keyword "static" solo pueden ser llamadas desde
// el archivo en el que fueron definidas. Sirve por ejemplo para hacer funciones
// internas ajenas al uso que se le va a dar a la estructura.
// Por ejemplo la funcion process_init solo se va a usar al agregar elementos a la
// lista ligada pero no se va a llamar desde el programa principal

/** Funcion que crea un nodo de la lista ligada a partir de un valor */
static Process* process_init(char* name, uint32_t time_start,uint32_t priority, uint32_t* arreglo, uint32_t value)
{
  // Pido la memoria para el nodo
  Process* process = malloc(sizeof(Process));

  // Inicializo el estado
  process -> state = READY;
  // Inicializo los Atributos
  process -> PID = 0;
  strcpy(process -> name, name);
  process -> priority = priority;
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

/*Constructor de una CPU*/
CPU* CPU_init()
{
    CPU* c = malloc(sizeof(CPU));
    c -> process = NULL;
    c -> use  = false;

    return c;
}


/** Constructor de una lista ligada. La crea vacia inicialmente */
Queue* ll_init(TypeQueue type)
{
  // Primero pido la memoria para la lista ligada
  Queue* ll = malloc(sizeof(Queue));

  // Luego inicializo en 0 el contador y en NULL las referencias
  ll -> type = type;
  ll -> count = 0;
  ll -> start = NULL;
  ll -> end = NULL;
  ll -> ID = 0;

  // Retorno la lista ligada
  return ll;
}

/**  NEW_PROCESS>> Funcion que crea y agrega un elemento al final de la lista ligada */
void ll_add_new(Queue* ll, char* name, uint32_t time_start, uint32_t priority, uint32_t* arreglo, uint32_t value)
{
  // Solo las listas NEW_PROCESS pueden crear elementos
  if (ll -> type == NEW_PROCESS){
    /*La lista new process crea los procesos
     y luego ingresa los procesos ordenados por time_start*/

    // Primero creo un nodo nuevo
    Process* process = process_init(name, time_start, priority, arreglo, value);
    // Luego lo agrego a la lista ligada
    if (!ll -> count)
    {
      // En el caso de que este vacia la lista dejo el nodo como inicial y final
      ll -> start = process;
      ll -> end = process;
    }
    else // Lo vamos a ingresar ordenadamente segun su tiempo de llegada
    {
      Process* aux_process;
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
  else{
    printf("no puedo agregar procesos\n" );
  }
}

/* Funcion que agrega un process a la lista WAITING, READY o FINISHED*/
void ll_append( Queue* ll, Process* process){
  if (ll->type == WAITING_PROCESS){
    //agrego los procesos en el orden que van llegando, van pasando al final de la cola
    // Solo entran los procesos waiting
    if(process -> state == WAITING){
      // En el caso de que este vacia la lista, dejo el nodo como inicial y final
      if (!ll -> count)
      {
        ll -> start = process;
        ll -> end = process;
        process -> last = NULL;
        process -> next = NULL;
      }
      else // Lo vamos a ingresar ordenadamente segun su tiempo de llegada
      {
        process -> last = ll -> end;
        process -> next = NULL;
        ll -> end -> next = process;
        ll -> end = process;
      }
      ll -> count++;
    }
    else{
      printf("El proceso no puede agregarse a la cola WAITING_PROCESS no esta en estado WAITING\n" );
    }
  }
  else if(ll->type == READY_PROCESS) {
    if (process -> state == READY){
      if (!process -> PID){
        // es la primera vez que ingresa al sistema
        process -> PID = ll -> ID;
        ll-> ID ++;
      }
      // si esta vacia lo agregamos
      if (!ll -> count)
      {
        ll -> start = process;
        ll -> end = process;
        process -> last = NULL;
        process -> next = NULL;
      }
      else{
      // ingresamos ordenadamente por prioridad (mayor a menor, si hay empate mayor a menor pid) el proceso
        if (ll -> start -> priority < process -> priority){
          ll -> start -> last = process;
          process -> next = ll -> start;
          process -> last = NULL;
          ll -> start = process;
        }
        // si voy despues del primero
        else{
          Process* aux_process;
          aux_process = ll -> start;
          int i = 0;
          while (i < ll-> count){
            // si hay un nodo despues de aux process
            if (aux_process -> next){
              // reviso si voy al medio de aux process y aux process next
              if (aux_process-> next-> priority > process -> priority){
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
    }
    else{
      printf("El proceso no puede agregarse a la cola READY_PROCESS no esta en estado READY\n" );
    }
  }

}


/* Funcion que saca el primer elemento de la lista */
Process* ll_out(Queue* ll)
{
  Process* aux = ll -> start;
  ll -> start = ll -> start -> next;
  ll -> count -= 1;
  return aux;
}

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
Process* ll_get(Queue* ll, uint32_t position)
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
void ll_destroy(Queue* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  processs_destroy(ll -> start);

  // Luego libero la memoria de la lista
  free(ll);
}
