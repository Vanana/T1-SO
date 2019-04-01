// Esta linea de codigo hace que no hayan problemas al importar un mismo archivo
// desde otros archivos
#pragma once

// Definición de enumeradores para los estados de un process.
typedef enum state {READY, RUNNING, WAITING, FINISHED} State;


/** Estructura de un nodo de la lista ligada. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct process
{
  
  State state;


  // En este caso no podemos referirnos a la estructura como Process ya que aun
  // no esta completo el typedef

  /** Puntero al nodo anterior */
  struct process* last;
  /** Puntero al nodo siguiente */
  struct process* next;
  /** Numero almacenado en el nodo */
  int value;
} Process;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct queue
{
  /** Nodo inicial de la lista ligada */
  Process* start;
  /** Nodo final de la lista ligada */
  Process* end;
  /** Contador de elementos de la lista ligada */
  int count;
} Queue;

///////////////////////////// Funcione publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
Queue* ll_init();

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(Queue* ll, int value);

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
Process* ll_get(Queue* ll, int position);

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(Queue* ll);
