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
  // y luego la usaremos normalmente. La struct AllQueue esta definida en el
  // archivo queue.h junto con todas sus funciones publicas. En le archivo
  // queue.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la lista ligada
  AllQueue* all_process = ll_init();
  char* name;
  name = "PROCESS1";
  uint32_t arreglo[2] = {2, 4};

  // Agrego 10 elementos a la lista ligada
  for (uint32_t i = 0; i < 10; i++)
  {
    ll_append(all_process, i, 9 - i, name, arreglo);
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

  Process* process_del = ll_out(all_process);
  printf("soy el elemento con tiempo = %d y valor %d\n", process_del->time_start, process_del->value);

  actual = ll_get(all_process, 5);
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

  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(all_process);

  // Como ejercicio puedes probar el programa usando valgrind para ver que no
  // hay leaks de memoria y luego eliminar la linea que llama a ll_destroy para
  // ver que detecta los leaks de memoria de la lista

  return 0;
}
