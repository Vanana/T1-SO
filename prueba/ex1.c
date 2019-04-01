// Importe de librerías

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Función auxiliar (para mostrar que se puede hacer tanto en main como aquí)
void print_phrase(char* phrase){
  printf("Tu frase es: %s\n", phrase);
}

// Función principal.
int main(int argc, char *argv[]) {

  // En este programa imprimiremos una frase ingresada por los usuarios. Lo
  // haremos dos veces: De forma directa y con la función externa.

  // Declaramos el string de la frase (máximo 120 caracteres).
  char phrase[120];

  // Solicitamos la frase.
  printf("Ingresa una frase: ");

  // La asociamos al string. Notar que no se usa & con la variable ya que es un
  // arreglo, por lo que hace referencia a su puntero.
  scanf("%s", phrase);

  // Imprimimos directamente.
  printf("Tu frase es: %s\n", phrase);

  // Lo mismo, pero con la función externa.
  print_phrase(phrase);

}
