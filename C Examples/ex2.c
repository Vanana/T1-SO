
#include <stdio.h>
/*stdio entrega funciones útiles como el printf, open, scanf y NULL*/
#include <stdlib.h>
/*stdlib tiene funciones como atoi, malloc, exit, rand y NULL*/
#include <string.h>
/*funciones de strings como strcpy, strcat o strlen*/

struct Persona {
	int edad;
	float estatura;
	char *nombre;
};
typedef struct Persona Persona;

Persona *crear_persona(int edad_input, float estatura, char *nombre) {
	Persona *person = malloc(sizeof(Persona));
	/*Pido memoria suficiente para una persona y retorno el puntero*/

	// printf("hola soy un error de memoria %i\n", person->edad); /*La edad de la persona no está inicializada aún*/

	person->edad = edad_input;
	//(*person).edad = edad input;
	person->estatura = estatura;

	// person->nombre = nombre;
	person->nombre = malloc(sizeof(char) * (strlen(nombre) + 1));
	strcpy(person->nombre, nombre);

	return person;
};

void saludar(Persona *person) {
	printf("Hola! Soy %s, tengo %i años y estoy en la dirección de memoria %p\n",
		person->nombre, person->edad, person);
};

int main(void){
	Persona *array_personas[10]; 
	printf("Tamaño del array creado: %lu\n", sizeof(array_personas));
	printf("Tamaño de cada puntero: %lu\n", sizeof(Persona*));
	printf("Puntero array_personas: %p\n", array_personas);

	char *nombre_generico = "Cristián";

	for (int index = 0; index < 5; index++){
		array_personas[index] = crear_persona(40, 1.60, nombre_generico);
	};

	saludar(array_personas[0]);
	saludar(array_personas[1]);
	saludar(array_personas[2]);

	// saludar(array_personas[9]); /*SEGFAULT*/

	/*Liberamos la memoria*/
	for (int index = 0; index < 4; index++){
		// free(array_personas[index]->nombre); /*Memoria inaccesible*/
		free(array_personas[index]);
	};

	return 0;
};