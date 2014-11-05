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
 
string nombre;
int columnaUltimo, columnaNuevo, x;
double filaUltimo, filaNuevo;

typedef enum tJugador
{
	Nadie,
	Jugador,
	Automata
};

//FUNCIONES
//FUNCIONES DE JUEGO

void saludar (){
	cout << "¡Bienvenido a Pasa la calculadora!" << endl;
	cout << "¿Como te llamas?";
	cin >> nombre;
	cout << Hola nombre;
}

//Dependiendo de quien gane, la despedida sera distinta
void despedirse (tJugador ganador){
	if (ganador == Nadie){
		cout << "¿Abandonas? Ohhh..." << endl;
		cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
	else if (ganador == jugador){
		cout << "Enhorabuena, has ganado" << endl;
		cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
	else {
		cout << "Lo siento, he ganado" << endl;
		cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona devuelve Nadie.
tJugador pasaCalculadora();

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza(){
	if (rand() % 2)
	return Jugador;
	else return Automata;
}

//Devuelve true si nuevo está en la misma fila que ultimo
bool mismaFila(int ultimo, int nuevo){
	filaUltimo = (ultimo/3);
	filaNuevo = (nuevo/3);
	return ceil(filaUltimo) == ceil(filaNuevo);
} 


//Devuelve true si nuevo está en la misma columna que ultimo
bool mismaColumna(int ultimo, int nuevo){
	columnaUltimo = (ultimo % 3);
	columnaNuevo = (nuevo % 3);
	return columnaUltimo == columnaNuevo;
}

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo
//Si ultimo == 0, este es el primer digito de la partida, y devuelve true
bool digitoValido(int ultimo, int nuevo){
	return ((mismaFila(int ultimo, int nuevo))||(mismaColumna(int ultimo, int nuevo)))&&(ultimo!=nuevo);
}


//FUNCIONES DE IA NIVEL 1
//Devuelve un dígito del 1 al 9
/*int digitoAleatorio(){
	if (digitoValido == true)
	return (rand() % 9) + 1;
}Esta no se si está bien del todo*/

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo);

//FUNCIONES DE JUGADOR
//Pide un dígito al jugador. Sólo devolverá un valor válido (entre 0 y 9).
//Para un valor no válido, mostrará un error.
int digitoPersona(); //to do Jaime

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo); //to do Jaime

//Muestra los botones de la calculadora
/*void mostrarCalculadora(){
	cout << "7     8     9" << endl;
	cout << "4     5     6" << endl;
	cout << "1     2     3";
} Estoy casi seguro de que esta está mal*/

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
int botDificil(int ultimo);

-----------------------------------------------------------------------------------*/


int main(){
	tJugador ganador;

	saludar();
	ganador = pasaCalculadora();
	despedirse(ganador);
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
	//Semilla aleatoria aqui
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
		std::cout << "Total = " << total;
	}while ((total < META) && (ultimoDigito != 0));
	
	if (ultimoDigito == 0) turno = Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo){
	int digito;

	do{
	digito = digitoAleatorio();
	}while (!digitoValido(ultimo, digito));

	std::cout << "Elijo el numero " << digito;

	return digito;
}

//Pide un digito al jugador. Solo devolvera un valor valido (entre 0 y 9).
//Para un valor no válido, mostrará un error.
int digitoPersona(){
	int digito = -1;

	mostrarCalculadora();

	while (digito == -1){
		try{
			std::cin.sync(); //Por si quedan datos basura en el buffer
			std::cin >> digito;
			if (digito < 0 || digito > 9) throw;
		}catch(...){
			std::cout << "Error! Introduce un digito entre 0-9";
			digito = -1;
		}
	}

	return digito;
}

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo){
	int digito = -1; //-1 es mi error flag

	while (digito == -1){
		try{
			digito = digitoPersona();
			if (!digitoValido(ultimo, digito)) throw;
		}catch(...){
			std::cout << "Error! El digito debe estar en la misma fila y columna que el ultimo";
			digito = -1;
		}
	}

	std::cout << "Has elegido el" << digito;

	return digito;
}
