// Libreria estandar de c
#include <stdlib.h>
/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>
/*funciones de strings como strcpy, strcat o strlen*/
#include <string.h>
// Archivo de listas ligadas
#include "queue.h"

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  /* Imprime el mensaje en la consola */
  printf("Prueba de procesos\n");

  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct Queue esta definida en el
  // archivo queue.h junto con todas sus funciones publicas. En le archivo
  // queue.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la CPU
  CPU* my_cpu = CPU_init();
  if (my_cpu){
    printf("su cpu fue creada\n");
  }

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type = NEW_PROCESS;
  Queue* all_process = ll_init(type);

  // Info para crear el proceso
  char* name;
  name = "PROCESS1";
  uint32_t arreglo[2] = {2, 4};

  // Agrego 10 elementos a la lista ligada
  for (uint32_t i = 0; i < 10; i++)
  {
    ll_add_new(all_process, name, 9 - i, 9 - i, arreglo, i);
  }
  Process* actual = ll_get(all_process, 5);
  if (actual->state == READY){
  // Imprimo el elemento de la posicion 5
    printf("El elemento en la posicion %d y esta READY es %d,  tiene el nombre %s, time > %d\n", 5, actual-> value, actual->name, actual->time_start);
    for (int j = 0; j < 2; j++){
      printf("soy el arreglo %d\n", actual->arreglo[j]);
    }
  }
  else {
    printf("El elemento en la posicion %d y esta no esta ready es %d \n", 5, actual-> value);
  }

  //Process* process_del = ll_out(all_process);
  //printf("soy el elemento con tiempo = %d y valor %d\n", process_del->time_start, process_del->value);

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type2 = WAITING_PROCESS;
  Queue* waiting_process = ll_init(type2);

  // Info saco un proceso de all a waiting
  actual = ll_out(all_process);
  actual -> state = WAITING;
  ll_append(waiting_process, actual);
  actual = ll_out(all_process);
  actual -> state = WAITING;
  ll_append(waiting_process, actual);

  actual = ll_get(waiting_process, 1);
  if (actual->state == READY){
  // Imprimo el elemento de la posicion 5
    printf("El elemento en la posicion %d y esta READY es %d,  tiene el nombre %s, time > %d\n", 5, actual-> value, actual->name, actual->time_start);
    for (int j = 0; j < 2; j++){
      printf("soy el arreglo %d\n", actual->arreglo[j]);
    }
  }
  else {
    printf("El elemento en la posicion %d y esta no esta ready es %d \n", 5, actual-> time_start);
  }








  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(all_process);
  ll_destroy(waiting_process);
  free(my_cpu);

  // Como ejercicio puedes probar el programa usando valgrind para ver que no
  // hay leaks de memoria y luego eliminar la linea que llama a ll_destroy para
  // ver que detecta los leaks de memoria de la lista

  return 0;
}
