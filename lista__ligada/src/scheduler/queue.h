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
  uint32_t PID;
  char name[255];
  uint32_t time_start;
  uint32_t priority;
  State state;
  uint32_t turn; // es el indice de la rafaga que se esta ejecutando
  uint32_t N; // cantidad de rafagas1
  uint32_t* rafagas; //arreglo de largo 2*N -1, indices impares A, indices pares B

  // para p //
  uint32_t q;

  // En este caso no podemos referirnos a la estructura como Process ya que aun
  // no esta completo el typedef

  /** Puntero al nodo anterior */
  struct process* last;
  /** Puntero al nodo siguiente */
  struct process* next;

  struct process* next_print; //se usa para calcular el output

  //Estadisticas
  uint32_t n_cpu;
  uint32_t n_interrupt;
  uint32_t turnaround;
  uint32_t response_t;
  uint32_t waiting;

} Process;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista
    La lista tendra tipo, que puede ser
      - NEW_PROCESS que son los procesos que aun no ingresan al sistema (ordenados por time)
      - READY_PROCESS que son los procesos en estado ready (ordenados por priority)
      - WAITING_PROCESS que son los procesos en estado waiting (ordenados por tiempo de waiting)
      - FINISHED_PROCESS que son los procesos en estado finish (ordenados por orden de llegada)
*/

// Definición de enumeradores para los tipos de colas.
typedef enum type_queue {NEW_PROCESS, READY_PROCESS, WAITING_PROCESS, FINISHED_PROCESS} TypeQueue;

typedef struct queue
{
  /* tipo de cola */
  TypeQueue type;
  /** Nodo inicial de la lista ligada */
  Process* start;
  /** Nodo final de la lista ligada */
  Process* end;
  /** Contador de elementos de la lista ligada */
  uint32_t count;
  /* Vamos a hacer un id contador, que usaremos para la lista ready */
  uint32_t ID;
} Queue;

typedef struct cpu {
  Process* process;
  bool use;
} CPU;

///////////////////////////// Funcione publicas ///////////////////////////

/*Constructor de una CPU*/
CPU* CPU_init();

/* Indica si el proceso que esta en cpu ya termino su ejecucion */
bool CPU_stop(CPU* cpu);

/* Poner un proceso en la cpu */
void use_CPU(CPU* cpu, Process* p);

/* Actualizo los tiempos de la cpu*/
void CPU_time(CPU* cpu);

/*Imprime la cpu*/
void CPU_print(CPU* cpu);

/** Funcion que destruye la cpu liberando la memoria utilizada */
void CPU_destroy(CPU* cpu);

/*  Imprimir un proceso */
void process_print(Process* process);

/** Constructor de una lista ligada. La crea vacia inicialmente */
Queue* ll_init(TypeQueue type);

/** Funcion que agrega un elemento(crea el proceso) de manera ordenada (segun time) de la lista ligada */
Process* ll_add_new(Queue* ll, char* name, uint32_t time_start, uint32_t priority, uint32_t N, uint32_t* rafagas, uint32_t q);

/** Asignamos los PID por orden de llegada*/
void ll_id(Queue * ll);

/* Funcion que agrega un process a la lista WAITING, READY o FINISHED*/
void ll_append( Queue* ll, Process* process);

/* Funcion que reordena la lista READY para que el que ha esperado mas ahora esta al inicio de la lista */
void ready_new_order(Queue* ll);

/* Funcion que saca el primer elemento(proceso) de la lista */
Process* ll_out(Queue* ll);

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
Process* ll_get(Queue* ll, uint32_t position);

/** Funcion que saca el valor de la lista ligada en la posicion dada */
Process* ll_get_out(Queue* ll, uint32_t position);

/* Actualiza los tiempos de espera*/
void waiting_time(Queue* ll);

/** Imprime una lista */
void ll_print(Queue* ll);

/** Imprime oficial */
void oficial_print(Queue* ll);

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(Queue* ll);


///ESTADISTICAS//

/* Actualizamos turnaround, recordar que la cpu tambien hay que actualizar*/
void turnaround_act(Queue* ll);

/*Actualizamos waiting time de los procesos*/
void waiting_act(Queue* ll);
