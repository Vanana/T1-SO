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
static Process* process_init(char* name, uint32_t time_start, uint32_t priority, uint32_t N, uint32_t* rafagas, uint32_t q)
{
  // Pido la memoria para el nodo
  Process* process = malloc(sizeof(Process));

  // Inicializo los Atributos
  process -> PID = 0;
  strcpy(process -> name, name);
  process -> time_start = time_start;
  process -> priority = priority;
  process -> state = READY;
  process -> turn = 0;
  process -> N = N;
  process -> rafagas = rafagas;

  process -> q = q;

  // Inicializo los referencias en NULL
  process -> last = NULL;
  process -> next = NULL;

  //Estadisticas
  process -> n_cpu = 0;
  process -> n_interrupt = 0;
  process -> turnaround = 0;
  process -> response_t = 0;
  process -> waiting = 0;

  // Retorno el nodo
  return process;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void process_destroy(Process* process)
{
  // Si el nodo no es NULL, llama recursivamente al siguiente
  // y libero el nodo actual
  if (process)
  {
    process_destroy(process -> next);
    free(process -> rafagas);
    free(process);
  }
}
void process_print(Process* process){
  if (process){
    printf("nombre %s, pid %d, time %d, p %d, state %d, turn %d, N %d, ", process->name, process->PID, process->time_start, process->priority, process->state, process->turn, process->N);
    for (uint32_t i = 0 ; i<(2*process->N -1); i++){
      printf(" %d ", process->rafagas[i] );
    }
    printf("\n");
  }
}
static void process_printall(Process* process){
  if (process){
    process_print(process);
    process_printall(process-> next);
  }
}

void oficial_process_print(Process* process){
  if (process){
    printf("%s, %d, %d, %d, %d, %d\n", process->name, process->n_cpu, process->n_interrupt, process->turnaround, process->response_t, process->waiting);
  }
}

static void oficial_process_printall(Process* process){
  if (process){
    oficial_process_print(process);
    oficial_process_printall(process-> next);
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

/* Indica si el proceso que esta en cpu ya termino su ejecucion */
bool CPU_stop(CPU* cpu)
{
    Process* p = cpu -> process;
    if (p){
//      printf("ESTOY EN FUNCION CPU STOP\n" );
//      process_print(p);
      if (p->rafagas[(p->turn)*2]== 0){
        return true;
      }
      return false;
    }
    else{
      return true;
    }
}

/* Poner un proceso en la CPU*/
void use_CPU(CPU* cpu, Process* p){
  if (!cpu->use && p->state == READY){
    p -> state = RUNNING;
    cpu -> process = p;
    cpu -> use = true;
  }
  else{
    printf("ERROR!!! la CPU esta en uso\n" );
  }
}

/* Actualizo los tiempos de la cpu*/
void CPU_time(CPU* cpu) {
  if (cpu->use){
    Process* p = cpu->process;
    p->rafagas[(p->turn)*2] -= 1;
//    printf("Se actualizo el proceso ---> " );
//    process_print(p);
  }
}

/* Imprimir el estado de la cpu*/
void CPU_print(CPU* cpu) {
  if (cpu->use){
    printf("La cpu esta ocupada por el proceso ---> " );
    process_print(cpu->process);
  }
  else{
    printf("La cpu esta libre \n" );
  }
}

/** Funcion que destruye la cpu liberando la memoria utilizada */
void CPU_destroy(CPU* cpu)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  if (cpu -> process){
    process_destroy(cpu -> process);
  }
  // Luego libero la memoria de la lista
  free(cpu);
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
void ll_add_new(Queue* ll, char* name, uint32_t time_start, uint32_t priority, uint32_t N, uint32_t* rafagas, uint32_t q)
{
  // Solo las listas NEW_PROCESS pueden crear elementos
  if (ll -> type == NEW_PROCESS){
    /*La lista new process crea los procesos
     y luego ingresa los procesos ordenados por time_start*/

    // Primero creo un nodo nuevo
    Process* process = process_init(name, time_start, priority, N, rafagas, q);
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
            if (aux_process-> next-> time_start > time_start){
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

/** Asignamos los PID por orden de llegada*/
void ll_id(Queue * ll){
  if (ll -> start){
    uint32_t id = 0;
    Process* p = ll -> start;
    p -> PID = id;
//    printf("nombre %s, id %d, tiempo %d\n", p->name, p->PID, p->time_start);
    while (p -> next) {
      id++;
      p = p -> next;
      p -> PID = id;
//      printf("nombre %s, id %d, tiempo %d\n", p->name, p->PID, p->time_start);
    }
  }
}

/* Funcion que agrega un process a la lista WAITING, READY o FINISHED*/
void ll_append( Queue* ll, Process* process){
  if (ll->type == WAITING_PROCESS || ll->type == FINISHED_PROCESS){
    //agrego los procesos en el orden que van llegando, van pasando al final de la cola
    // Solo entran los procesos waiting
    if(process -> state == WAITING || process -> state == FINISHED){
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
          // si tengo mayor prioridad que el primero, soy el primero
          ll -> start -> last = process;
          process -> next = ll -> start;
          process -> last = NULL;
          ll -> start = process;
        }
        else if (ll -> start -> priority == process -> priority){
          // si tengo la misma prioridad comparo por pid mayor
          if (ll -> start -> PID < process -> PID){
            // si tengo mayor PID, soy primera.
            ll -> start -> last = process;
            process -> next = ll -> start;
            process -> last = NULL;
            ll -> start = process;
          }
          else {
            Process* aux_process;
            aux_process = ll -> start;
            int i = 0;
            while (i < ll-> count){
              // si hay un nodo despues de aux process
              if (aux_process -> next){
                // reviso si voy al medio de aux process y aux process next
                if (aux_process -> next -> priority == process -> priority){
                  if (aux_process-> next-> PID < process -> PID){
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
                  process -> next = aux_process -> next;
                  process -> next -> last = process;
                  aux_process -> next = process;
                  process -> last = aux_process;
                  break;
                }
              }
              else{
                // si no hay otro process es por que voy al final
                aux_process -> next = process;
                process -> last = aux_process;
                ll -> end = process;
                process -> next = NULL;
                break;
              }
              i ++;
            }
          }

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
              if (aux_process-> next-> priority < process -> priority){
                process -> next = aux_process -> next;
                process -> next -> last = process;
                aux_process -> next = process;
                process -> last = aux_process;
                break;
              }
              else if (aux_process-> next-> priority == process -> priority){
                int j = i;
                Process* aux_process2 = aux_process;
                while (j < ll-> count){
                  // si hay un nodo despues de aux process
                  if (aux_process2 -> next){
                    // reviso si voy al medio de aux process y aux process next
                    if (aux_process2 -> next -> priority == process -> priority){
                      if (aux_process2-> next-> PID < process -> PID){
                        process -> next = aux_process2 -> next;
                        process -> next -> last = process;
                        aux_process2 -> next = process;
                        process -> last = aux_process2;
                        break;
                      }
                      else{
                        // si no voy al medio, significa que puede que vaya despues
                        aux_process2 = aux_process2 -> next;
                      }
                    }
                    else{
                      process -> next = aux_process2 -> next;
                      process -> next -> last = process;
                      aux_process2 -> next = process;
                      process -> last = aux_process2;
                      break;
                    }
                  }
                  else{
                    // si no hay otro process es por que voy al final
                    aux_process2 -> next = process;
                    process -> last = aux_process2;
                    ll -> end = process;
                    process -> next = NULL;
                    break;
                  }
                  j ++;
                }
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
      ll -> count ++;
    }
    else{
      printf("El proceso no puede agregarse a la cola READY_PROCESS no esta en estado READY\n" );
    }
  }
}

/* Funcion que saca el primer elemento de la lista */
Process* ll_out(Queue* ll)
{
  if (ll-> count > 0){
    Process* aux = ll -> start;
    ll -> start = ll -> start -> next;
    if (ll->start){
      ll -> start -> last = NULL;
    }
    ll -> count -= 1;
    aux -> next = NULL;
    aux -> last = NULL;
    return aux;
  }
  else{
//    printf("No quedan mas datos en la cola\n" );
    return NULL;
  }
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

/** Funcion que saca el valor de la lista ligada en la posicion dada */
Process* ll_get_out(Queue* ll, uint32_t position)
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
  if (ll->start == actual){
    ll->start = actual -> next;
    if (ll->start){
      ll->start->last = NULL;
    }
  }
  else if (ll->end == actual){
    ll->end = ll->end->last;
    ll->end->next = NULL;
  }
  else{
    actual ->next ->last = actual ->last;
    actual ->last ->next = actual ->next;
  }
  actual -> next = NULL;
  actual -> last = NULL;
  ll->count -= 1;
  return actual;
}

/* Actualiza los tiempos de espera*/
void waiting_time(Queue* ll){
  if (ll->type == WAITING_PROCESS){
    Process* actual = ll->start;
//    printf("--------------------------------estoy actualizando waiting-------------------\n");
    if(actual){
      for (uint32_t i = 0; i < ll->count; i++){
        actual -> rafagas[(actual->turn)*2 +1] -= 1;
//        printf("estado actualizado\n");
//        process_print(actual);
        if (actual-> rafagas[(actual->turn)*2 +1] == 0){
          actual-> state = READY;
        }
        if (actual->next){
          actual = actual-> next;
        }
      }
    }
  }
}

/** Imprime una lista */
void ll_print(Queue* ll)
{
  printf("lista tipo %d, tengo %d procesos\n", ll->type, ll->count);
  // Primero libero la memoria de todos los nodos de manera recursiva
  if (ll -> start){
    process_printall(ll -> start);
  }
}

/** Imprime oficial */
void oficial_print(Queue* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  if (ll -> start){
    oficial_process_printall(ll -> start);
  }
}

/////////////////ESTADISTICAS////////////////////////////

/* Actualizamos turnaround, recordar que la cpu tambien hay que actualizar*/
void turnaround_act(Queue* ll) {
  if (ll->type != NEW_PROCESS && ll->type != FINISHED_PROCESS){
    Process* actual = ll->start;
    if (actual){
      for (uint32_t i = 0; i < ll->count; i++){
        actual -> turnaround ++;
        if (actual->next){
          actual = actual-> next;
        }
      }
    }
  }
}

/*Actualizamos waiting time de los procesos*/
void waiting_act(Queue* ll) {
  if (ll->type != NEW_PROCESS && ll->type != FINISHED_PROCESS){
    Process* actual = ll->start;
    if (actual){
      for (uint32_t i = 0; i < ll->count; i++){
        actual -> waiting ++;
        if (actual->next){
          actual = actual-> next;
        }
      }
    }
  }
}




/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(Queue* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  if (ll -> start){
    process_destroy(ll -> start);
  }
  // Luego libero la memoria de la lista
  free(ll);
}
