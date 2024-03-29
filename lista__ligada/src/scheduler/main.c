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
  uint8_t si_wait = strcmp(tipo_sim, "w");

  if (argument_count == 5){
    q = atoi(arguments[4]);
  }
  else{
    if (no_np == 0){
      q = 0;
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
  Process* start;
  Process* current;
  Process* next;
  while (fscanf(file, "%s %d %d %d\t", name0, &priority0, &time_start0, &N0) != EOF) {
    //fscanf(file, "%s %d %d %d\t", name0, &priority0, &time_start0, &N0);
//    printf("%s %d %d %d \n", name0, priority0, time_start0, N0);
    uint32_t* arreglo = malloc(sizeof(uint32_t)*(2*N0-1));
    for (uint32_t i = 0 ; i<(2*N0-1); i++){
      fscanf(file, "%d", &arreglo[i]);
//      printf("%d\n", arreglo[i] );
    }
    next = ll_add_new(all_process, name0, time_start0, priority0, N0, arreglo, q);

    if (all_process->count ==1){
      start = ll_get(all_process, 0);
      current = start;
    }
    else{
      current->next_print = next;
      current = next;
    }
  }
  fclose(file);

  ll_id(all_process);
  uint32_t t = 0;
  Process* actual;
  bool continuo = true;
///////////////////////////////INICIA SIMULACION////////////////////////////////////

  while (continuo){
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
    if (my_cpu->process && no_np && si_wait !=0 ){
//      process_print(my_cpu -> process);
       if(my_cpu->process->q == 0 ){
         //me bloqueo y libero la memoria
         actual = my_cpu -> process;
         actual -> q = q;
         actual -> n_interrupt ++;
         my_cpu -> process = NULL;
         my_cpu -> use = false;
//         process_print(actual);
//         CPU_print(my_cpu);

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
        if (!actual->next) break;
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
    if (my_cpu->process && no_np && si_wait != 0){
      my_cpu->process->q -= 1;
    }

    ////// REORDENAMOS READY SI ESTAMOS EN SCHEDULER W ////////
    if (si_wait == 0){
      ready_new_order(ready_process);
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
    //vemos si aun quedan elementos en las listas y la cpu
    if (!my_cpu->use && all_process->count == 0 && ready_process->count==0 && waiting_process->count==0){
      continuo =false;
    }
  }

  //Imprimimos el output
  FILE * f_out = fopen(output_name, "w+");

  if (start){
    Process* process = start;
    while (1) {
      fprintf(f_out, "%s,%d,%d,%d,%d,%d\n", process->name, process->n_cpu, process->n_interrupt, process->turnaround, process->response_t, process->waiting);
      if (!process->next_print){
        break;
      }
      process = process -> next_print;
    }
  }
/* print antiguo
  FILE * f_out = fopen(output_name, "w+");

  if (finished_process -> start){
    Process* process = finished_process -> start;
    while (1) {
      fprintf(f_out, "%s, %d, %d, %d, %d, %d\n", process->name, process->n_cpu, process->n_interrupt, process->turnaround, process->response_t, process->waiting);
      if (!process->next){
        break;
      }
      process = process -> next;
    }
  }
*/

  // Creamos el output del archivo
/*
  oficial_print(all_process);
  oficial_print(ready_process);
  oficial_print(waiting_process);
  oficial_print(finished_process);
*/


  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(all_process);
  ll_destroy(waiting_process);
  ll_destroy(ready_process);
  ll_destroy(finished_process);
  CPU_destroy(my_cpu);


  return 0;
}
