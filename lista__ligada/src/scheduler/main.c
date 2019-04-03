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
int main(int argument_count, char** arguments)
{
  /* Imprime el mensaje en la consola */
//  printf("Prueba de procesos\n");

  if (argument_count!= 4 && argument_count!= 5){
    printf("Mal modo de uso\n" );
    printf("Modo de uso: %s <file> <output> <version>\n", arguments[0]);
    return 1;
  }

  /* El primer parametro es el nombre del archivo */
  char* filename = arguments[1];

  /* El segundo parametro es el nombre del output*/
  char* output_name = arguments[2];

  /* El tercer parametro es el tipo de simulacion*/
  char* tipo_sim = arguments[3];

  uint32_t q = 0;
  uint8_t no_np = strcmp(tipo_sim, "np");

  if (argument_count == 5){
    q = atoi(arguments[4]);
  }
  else{
    if (no_np == 0){
      q = 0;
      printf("soy q = 0\n" );
    }
    else{
      q = 3;
    }
  }

  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct Queue esta definida en el
  // archivo queue.h junto con todas sus funciones publicas. En le archivo
  // queue.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la CPU
  CPU* my_cpu = CPU_init();
  if (my_cpu){
//    printf("su cpu fue creada\n");
  }

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type = NEW_PROCESS;
  Queue* all_process = ll_init(type);

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type2 = WAITING_PROCESS;
  Queue* waiting_process = ll_init(type2);

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type3 = READY_PROCESS;
  Queue* ready_process = ll_init(type3);

  // Creo la lista ligada, necesito definir el tipo
  TypeQueue type4 = FINISHED_PROCESS;
  Queue* finished_process = ll_init(type4);

  //Leemos los archivos y creamos los procesos
  FILE* file = fopen(filename, "r");
  char name0[256];
  uint32_t priority0, time_start0, N0;
  while (fscanf(file, "%s %d %d %d\t", name0, &priority0, &time_start0, &N0) != EOF) {
    //fscanf(file, "%s %d %d %d\t", name0, &priority0, &time_start0, &N0);
//    printf("%s %d %d %d \n", name0, priority0, time_start0, N0);
    uint32_t* arreglo = malloc(sizeof(uint32_t)*(2*N0-1));
    for (uint32_t i = 0 ; i<(2*N0-1); i++){
      fscanf(file, "%d", &arreglo[i]);
//      printf("%d\n", arreglo[i] );
    }
    ll_add_new(all_process, name0, time_start0, priority0, N0, arreglo, q);
  }
  fclose(file);

  ll_id(all_process);
  uint32_t t = 0;
  Process* actual;
///////////////////////////////INICIA SIMULACION////////////////////////////////////

  while (t < 50){
//    printf("\n\n!!!!!!!!! ESTAMOS EN TIEMPO %d !!!!!!!!!!!!!!!\n\n", t);
    ////// PRIMERA ETAPA: PASAR PROCESOS DE NEW a READY y de WAITING a READY y liberar la CPU si termino su uso//////
    /* Primero entramos a READY los procesos que llegan */
    if (all_process-> count > 0){
      //printf(" proceso %d , datos %d\n", all_process->type, all_process ->count );
      actual = ll_get(all_process, 0);
//      process_print(actual);
      if (actual->time_start == t){
        //si es la hora de que ingrese un proceso, ingresa a la fila READY_PROCESS
        actual = ll_out(all_process);
        actual -> state = READY;
        ll_append(ready_process, actual);
      }
    }
    /* Revisamos si se le acabo el q al proceso en cpu, pero solo si estamos en una simulacion p */
    if (my_cpu->process && no_np){
      printf("entre\n");
      process_print(my_cpu -> process);
      printf("my q es %d\n",my_cpu->process->q );
       if(my_cpu->process->q == 0 ){
         //me bloqueo y libero la memoria
         actual = my_cpu -> process;
         actual -> q = q;
         actual -> n_interrupt ++;
         my_cpu -> process = NULL;
         my_cpu -> use = false;
         process_print(actual);
         printf("mi q es %d\n", actual->q);
         CPU_print(my_cpu);

         //ahora vemos si alcanzo a terminar su proceso o si pasa a ready
         if (actual->rafagas[(actual->turn)*2]== 0){
           // si termino su rafaga, vemos si le quedan mas
           if (actual-> turn == (actual-> N) - 1){
             // el proceso termino todos sus rafagas
             actual-> state = FINISHED;
             ll_append(finished_process, actual);
           }
           else{
             actual -> state = WAITING;
   //          process_print(actual);
             ll_append(waiting_process, actual);
           }
         }
         else {
           //si no alcanzo a terminar
           actual -> state = READY;
           ll_append(ready_process, actual);
         }
       }
    }

    /* Revisamos si el proceso de cpu ya termino, si es asi liberamos*/
    if (CPU_stop(my_cpu)){
      if (my_cpu -> process){
//        printf("liberamos cpu\n");
        actual = my_cpu -> process;
        actual -> q = q;
        my_cpu -> process = NULL;
        my_cpu -> use = false;
//        process_print(actual);
        if (actual-> turn == (actual-> N) - 1){
          // el proceso termino todos sus rafagas
          actual-> state = FINISHED;
          ll_append(finished_process, actual);
        }
        else{
          actual -> state = WAITING;
//          process_print(actual);
          ll_append(waiting_process, actual);
        }
      }
    }

    /* Revisamos si hay que pasar algun processo de la cola waiting a ready */
    if (waiting_process->count){
      actual = waiting_process->start;
      for (uint32_t i = 0; i < waiting_process->count; i++){
        if (actual->state == READY){
//          printf("SACAMOS UN PROCESO DE WAITING\n" );
//          process_print(actual);
          actual = ll_get_out(waiting_process, i);
          actual -> turn ++;
          ll_append(ready_process, actual);
        }
        actual = actual -> next;
      }
    }

    //////SEGUNDA ETAPA: cargar un proceso en la cpu si se encuentra libre //////
    if (!my_cpu->use){
      actual = ll_out(ready_process);
      if (actual){
        actual -> state = RUNNING;
        my_cpu -> use = true;
        my_cpu -> process = actual;
        if (actual-> n_cpu == 0 ){
          actual -> response_t = t - actual->time_start; //Estadisticas
        }
        actual -> n_cpu ++; //Estadisticas
      }
    }

/*    printf("   ESTADOS\n" );
    CPU_print(my_cpu);
    ll_print(all_process);
    ll_print(ready_process);
    ll_print(waiting_process);
    ll_print(finished_process);
*/
    //////TERCERA ETAPA: actualizar tiempos ////////
    CPU_time(my_cpu);
    waiting_time(waiting_process);
    if (my_cpu->process && no_np){
      my_cpu->process->q -= 1;
    }

    //Estadisticas
    turnaround_act(waiting_process);
    turnaround_act(ready_process);
    if (my_cpu->process){
      my_cpu -> process -> turnaround ++;
    }
    waiting_act(waiting_process);
    waiting_act(ready_process);

    t++;
  }

  oficial_print(all_process);
  oficial_print(ready_process);
  oficial_print(waiting_process);
  oficial_print(finished_process);

/*
  Process* actual = ll_get(all_process, 2);
  if (actual->state == READY){
  // Imprimo el elemento de la posicion 5
    printf("El elemento en la posicion %s y esta READY en el tiempo %d,  prioridad %d\n", actual-> name, actual->time_start, actual->priority);
    for (int j = 0; j < (actual -> N) * 2 -1; j++){
      printf("soy el arreglo %d\n", actual->rafagas[j]);
    }
  }
  else {
    printf("El elemento nombre %s y esta no esta ready  \n", actual-> name);
  }

  //Process* process_del = ll_out(all_process);
  //printf("soy el elemento con tiempo = %d y valor %d\n", process_del->time_start, process_del->value);

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
    printf("El elemento en la posicion %s y esta READY en el tiempo %d,  prioridad %d\n",  actual-> name, actual->time_start, actual->priority);
    for (int j = 0; j < 2; j++){
      printf("soy el arreglo %d\n", actual->rafagas[j]);
    }
  }
  else {
    printf("El elemento nombre %s y esta no esta ready  \n", actual-> name);
  }





*/


  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(all_process);
  ll_destroy(waiting_process);
  ll_destroy(ready_process);
  ll_destroy(finished_process);
  CPU_destroy(my_cpu);

  // Como ejercicio puedes probar el programa usando valgrind para ver que no
  // hay leaks de memoria y luego eliminar la linea que llama a ll_destroy para
  // ver que detecta los leaks de memoria de la lista

  return 0;
}
