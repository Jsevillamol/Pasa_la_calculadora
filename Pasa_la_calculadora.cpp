/*-------------------------------
PASA LA CALCULADORA
Autores: 
Jaime Sevilla Molina
Victor
Fecha
2014/11
Version: 1.0
---------------------------------*/

//BIBLIOTECAS
#include <iostream>
#include <string>
#include <cstdlib>

const int META = 31;

typedef enum tJugador
{
	Nadie,
	Jugador,
	Automata
};

//FUNCIONES
//FUNCIONES DE JUEGO

void saludar(); //to do Victor
void despedirse(); //to do Victor

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona devuelve Nadie.
tJugador pasaCalculadora(); //to do Jaime

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza(); //to do Victor

//Devuelve true si nuevo est� en la misma fila que ultimo
bool mismaFila(int ultimo, int nuevo); //to do Victor

//Devuelve true si nuevo est� en la misma columna que ultimo
bool mismaColumna(int ultimo, int nuevo); //to do Victor

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo
bool digitoValido(int ultimo, int nuevo); //to do Victor


//FUNCIONES DE IA NIVEL 1
//Devuelve un d�gito.
int digitoAleatorio(); //to do Victor

//Devuelve un d�gito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo); //to do Jaime

//FUNCIONES DE JUGADOR
//Pide un d�gito al jugador. S�lo devolver� un valor v�lido (entre 0 y 9).
//Para un valor no v�lido, mostrar� un error.
int digitoPersona(); //to do Jaime

//Pide un d�gito al jugador mostrando el teclado. S�lo devolver� un valor 
//que cumpla las reglas del juego o 0. Para un valor no v�lido, mostrar� un error.
int digitoPersona(int ultimo); //to do Jaime

/* Las funciones a continuacion se implementaran en un futuro
//FUNCIONES DE MEN�
//Muestra el men�, pide la opci�n y la devuelve como resultado. S�lo
//devolver� una opci�n v�lida. Para cada valor no v�lido, mostrar� un error.
int menu();

//Muestra en la consola el contenido del archivo de texto nombArch. 
//Si el archivo no se encuentra, devuelve false, en otro caso true.
bool mostrar(string nombArch);


//FUNCIONES DE INFORME
//Actualiza el archivo informePC.txt con los tres argumentos. En caso de no encontrar
//el archivo, lo crea y devuelve false; en otro caso devuelve true.
bool actInforme(int jugadas, int ganadas, int abandonos);


//FUNCIONES DE DIFICULTAD AVANZADA
int bot_dificil(int ultimo);

-----------------------------------------------------------------------------------*/


int main(){

	return 0;
	}