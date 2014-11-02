/*-------------------------------
PASA LA CALCULADORA
Autores: 
Jaime Sevilla Molina
Victor Gonzalez
Fecha
2014/11
Version: 1.0
---------------------------------*/

//BIBLIOTECAS
#include <iostream>
#include <cstdlib>

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
tJugador pasaCalculadora();

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza(); //to do Victor

//Devuelve true si nuevo está en la misma fila que ultimo
bool mismaFila(int ultimo, int nuevo); //to do Victor

//Devuelve true si nuevo está en la misma columna que ultimo
bool mismaColumna(int ultimo, int nuevo); //to do Victor

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo
bool digitoValido(int ultimo, int nuevo); //to do Victor


//FUNCIONES DE IA NIVEL 1
//Devuelve un dígito del 1 al 9
int digitoAleatorio(); //to do Victor

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo);

//FUNCIONES DE JUGADOR
//Pide un dígito al jugador. Sólo devolverá un valor válido (entre 0 y 9).
//Para un valor no válido, mostrará un error.
int digitoPersona(); //to do Jaime

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo); //to do Jaime

/* Las funciones a continuacion se implementaran en un futuro
//FUNCIONES DE MENÚ
//Muestra el menu, pide la opcion y la devuelve como resultado. Solo
//devolvera una opción valida. Para cada valor no valido, mostrará un error.
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
	saludar();
	pasaCalculadora();
	despedirse();
	return 0;
	}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona devuelve Nadie.
tJugador pasaCalculadora(){
	//Variables
	tJugador turno;
	int total = 0, ultimoDigito = 0;
	const int META=31;

	//Inicializar partida
	turno = quienEmpieza();
	//Semilla aleatoria aqui

	//Bucle de juego
	do{
		//Turno jugador
		if (turno == Jugador){
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Automata;
		}
		//Turno bot
		else if (turno == Automata){
			ultimoDigito = digitoAutomata(ultimoDigito);
			turno = Jugador;
		}
		total += ultimoDigito;
	}while ((total < META) && (ultimoDigito != 0));
	
	if (ultimoDigito == 0) turno = Nadie; //Si el jugador abandona, no gana nadie
	return turno;
}

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo){
	int digito;

	do{
	digito = digitoAleatorio();
	} while ((ultimo != 0) && (!digitoValido(ultimo, digito))); //Si el ultimo es 0, este es el primer digito de la partida

	return digito;
}