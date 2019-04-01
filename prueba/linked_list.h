// Esta linea de codigo hace que no hayan problemas al importar un mismo archivo
// desde otros archivos
#pragma once

// Definición de enumeradores para los estados de un process.
typedef enum state {READY, RUNNING, WAITING, FINISHED} State;

/** Estructura de un nodo de la lista ligada. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct process
{
  uint32_t PID;
  uint32_t start_time;
  char name[255];
  uint32_t priority;
  State state;
  uint32_t turn;
  uint32_t cant; //cantidad de rafagas
  uint32_t** A; // puntero a un arreglo de rafagas
  uint32_t** B; // puntero a un arreglo de I/O brust
  struct process* next;
  struct process* last;
} Process;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct queue {
  Process* process;
  uint32_t length;
} Queue;

///////////////////////////// Funcione publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
Queue* ll_init();

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(Queue* ll, uint32_t PID);

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
int ll_get(Queue* ll, int position);

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(Queue* ll);
