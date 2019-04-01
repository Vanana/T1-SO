// Esta linea de codigo hace que no hayan problemas al importar un mismo archivo
// desde otros archivos
#pragma once

// Definición de enumeradores para los estados de un process.
typedef enum state {READY, RUNNING, WAITING, FINISHED} State;


/** Estructura de un nodo de la lista ligada. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct process
{
  // Atributos de process
  uint32_t time_start;
  char name[255];
  State state;
  uint32_t value;
  uint32_t* arreglo;

  // En este caso no podemos referirnos a la estructura como Process ya que aun
  // no esta completo el typedef

  /** Puntero al nodo anterior */
  struct process* last;
  /** Puntero al nodo siguiente */
  struct process* next;

} Process;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct all_queue
{
  /** Nodo inicial de la lista ligada */
  Process* start;
  /** Nodo final de la lista ligada */
  Process* end;
  /** Contador de elementos de la lista ligada */
  uint32_t count;
} AllQueue;

///////////////////////////// Funcione publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
AllQueue* ll_init();

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(AllQueue* ll, uint32_t value, uint32_t time_start, char* name, uint32_t* arreglo);

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
Process* ll_get(AllQueue* ll, uint32_t position);

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(AllQueue* ll);
