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
#include <string>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

typedef enum tJugador
{
	Nadie,
	Jugador,
	Automata
};

//FUNCIONES
//FUNCIONES DE JUEGO

void saludar ();

//Dependiendo de quien gane, la despedida sera distinta
void despedirse (tJugador ganador);

//Muestra un menu que permite al jugador jugar, salir, o ver las reglas del juego
int menu();

//Muestra las instrucciones del juego, siempre que su archivo no contenga errores
bool acerca()

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona, devuelve Nadie.
tJugador pasaCalculadora();

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza();
//Devuelve true si nuevo esta en la misma fila que ultimo
bool mismaFila(int ultimo, int nuevo);


//Devuelve true si nuevo esta en la misma columna que ultimo
bool mismaColumna(int ultimo, int nuevo);

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo
//Si ultimo == 0, este es el primer digito de la partida, y devuelve true
bool digitoValido(int ultimo, int nuevo);


//FUNCIONES DE IA NIVEL 1
//Devuelve un digito del 1 al 9
int digitoAleatorio();

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo);

//FUNCIONES DE JUGADOR
//Pide un digito al jugador. Solo devolvera un valor valido (entre 0 y 9).
//Para un valor no valido, mostrara un error.
int digitoPersona();

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo);

//Determina si el numero de la calculadora se muestra o no, en funcion de si es valido
char mNumero(int ultimo, int n);

//Muestra los botones de la calculadora (solo los que se pueden pulsar en cada turno)
void mostrarCalculadora(int ultimo);


/* Las funciones a continuacion se implementaran en un futuro
//FUNCIONES DE INFORME
//Actualiza el archivo informePC.txt con los tres argumentos. En caso de no encontrar
//el archivo, lo crea y devuelve false; en otro caso devuelve true.
bool actInforme(int jugadas, int ganadas, int abandonos);


//FUNCIONES DE DIFICULTAD AVANZADA
int botDificil(int ultimo);

-----------------------------------------------------------------------------------*/


int main(){
	tJugador ganador;
	
	saludar();
	
	//Bucle Menu
	do{
		opcion = menu();
		if(opcion == 1){	
			ganador = pasaCalculadora();
			despedirse(ganador);
			
		}
		if(opcion == 2) acerca();
	
	}
	while(opcion != 0);
	
	cout << "Hasta la proxima";	
	}
	return 0;
}
	
//Saluda al jugador y le pregunta su nombre
void saludar(){
	string nombre;
	cout << "¡Bienvenido a Pasa la calculadora!" << endl;
	cout << "¿Como te llamas? ";
	cin >> nombre;
	cout << "Hola " << nombre << endl;
}

//Se despide del jugador, la despedida varia segun gane el jugador, el automata o ninguno de ellos (el jugador abandone)
void despedirse(tJugador ganador){
	string nombre;
	if (ganador == Nadie){
		cout << "¿Abandonas? Ohhh..." << endl;
		cout << "Hasta la proxima " << nombre << " (pulsa una tecla)";
	}
	else if (ganador == Jugador){
		cout << "Enhorabuena, has ganado" << endl;
		cout << "Hasta la proxima " << nombre << " (pulsa una tecla)";
	}
	else{ /*(ganador == Automata)*/
		cout << "Lo siento, he ganado" << endl;
		cout << "Hasta la proxima " << nombre << " (pulsa una tecla)";
	}
}

//Proporciona al jugador la posibilidad de jugar, ver las instrucciones del juego o salir.
int menu(){
	int seleccionar;
	cout << "1 - Jugar" << endl;
	cout << "2 - Acerca de" << endl;
	cout << "0 - Salir" << endl;
	
	do{
		try{
			cin.sync(); //Por si quedan datos basura en el buffer
			cin >> seleccionar;
			if (seleccionar < 0 || seleccionar > 2) throw;
		}catch(...){
			cout << "Error! Introduce un digito entre 0 y 2";
			seleccionar = -1;
		}
	}while (seleccionar == -1);

	return seleccionar;
}

//Muestra el archivo "acerca.txt" siempre que este no contenga errores
bool acerca(){

	bool = ok;
	ifstream acerca;
	char c;

	acerca.open("acerca.txt");
	
	if(acerca.is_open()){
		acerca.get(c);
		while(!acerca.fail()){//Mientras la lectura no falle
			cout << c;
			acerca.get(c);
			ok = true;
		}
	}else{
		ok = false;
		cout << "Error, el archivo 'acerca.txt' no existe"
	}
	}
	return ok;
}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona, devuelve Nadie.
tJugador pasaCalculadora(){
	//Variables
	tJugador turno;
	int total = 0, ultimoDigito = 0;
	const int META=31;
		//Inicializar partida
	srand(time(NULL))//Semilla
	turno = quienEmpieza();

	//Bucle de juego
	do{
		//Turno jugador
		if (turno == Jugador){
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Automata;
		}
		//Turno bot
		else /*if (turno == Automata)*/{
			ultimoDigito = digitoAutomata(ultimoDigito);
			turno = Jugador;
		}
		total += ultimoDigito;
		cout << "Total = " << total;
	}while ((total < META) && (ultimoDigito != 0));
	
	if (ultimoDigito == 0) turno = Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}

//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
tJugador quienEmpieza(){
	if (rand() % 2){
		cout << "Tu empiezas" << endl;
		return Jugador;
	}
	else{ 
		cout << "Empiezo yo" << endl; 
		return Automata;
	}
}

//Define que numeros se encuentran en la misma fila que el ultimo pulsado
bool mismaFila(int ultimo, int nuevo){
	double filaUltimo, filaNuevo;
	filaUltimo = (ultimo/3);
	filaNuevo = (nuevo/3);
	return ceil(filaUltimo) == ceil(filaNuevo);
}

//Define que numeros se encuentran en la misma columna que el ultimo
bool mismaColumna(int ultimo, int nuevo){
	int columnaUltimo, columnaNuevo;
	columnaUltimo = (ultimo % 3);
	columnaNuevo = (nuevo % 3);
	return columnaUltimo == columnaNuevo;
}

//Determina que digitos se pueden pulsar en funcion de las reglas del juego
bool digitoValido(int ultimo, int nuevo){
	if (ultimo == 0) return true;
	return ((mismaFila(int ultimo, int nuevo))||(mismaColumna(int ultimo, int nuevo)))&&(ultimo!=nuevo);
}

//Genera un digito aleatorio
int digitoAleatorio(){
	return (rand() % 9) + 1;
}

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo){
	int digito;

	do{
	digito = digitoAleatorio();
	}while (!digitoValido(ultimo, digito));

	cout << "Elijo el numero " << digito << endl;

	return digito;
}

//Pide un digito al jugador. Solo devolvera un valor valido (entre 0 y 9).
//Para un valor no valido, mostrara un error.
int digitoPersona(){
	int digito;

	do{
		try{
			cin.sync(); //Por si quedan datos basura en el buffer
			cin >> digito;
			if (digito < 0 || digito > 9) throw;
		}catch(...){
			cout << "Error! Introduce un digito entre 0 y 9";
			digito = -1;
		}
	}while (digito == -1);

	return digito;
}

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo){
	int digito; //-1 es mi error flag
	
	mostrarCalculadora(ultimo);

	do{
		try{
			digito = digitoPersona();
			if (!digitoValido(ultimo, digito)) throw;
		}catch(...){
			cout << "Error! El digito debe estar en la misma fila y columna que el ultimo" << endl;
			digito = -1;
		}
	}while (digito == -1);

	cout << "Has elegido el" << digito << endl;

	return digito;
}

char mNumero(int ultimo, int n){
	if(digitoValido(ultimo, n) return char (n+int('0'));
	else return ' ';
}

void mostrarCalculadora(ultimo){
	for (int i = 7; i<10; i++){
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
	for (int i = 4; i<7; i++){
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
	for (int i = 1; i<4; i++){
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
}
