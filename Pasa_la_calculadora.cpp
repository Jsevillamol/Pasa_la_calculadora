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

void saludar ();//Implementada

//Dependiendo de quien gane, la despedida sera distinta
void despedirse (tJugador ganador);//Implementada

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona devuelve Nadie.
tJugador pasaCalculadora();

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza();//Implementada
//Devuelve true si nuevo está en la misma fila que ultimo
bool mismaFila(int ultimo, int nuevo);//Implementada


//Devuelve true si nuevo está en la misma columna que ultimo
bool mismaColumna(int ultimo, int nuevo);//Implementada

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo
//Si ultimo == 0, este es el primer digito de la partida, y devuelve true
bool digitoValido(int ultimo, int nuevo);//Implementada


//FUNCIONES DE IA NIVEL 1
//Devuelve un dígito del 1 al 9
int digitoAleatorio();//Implementada

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
}
//Saluda al jugador y le pregunta su nombre
saludar(string nombre){

	std::cout << "¡Bienvenido a Pasa la calculadora!" << endl;
	std::cout << "¿Como te llamas?";
	std::cin >> nombre;
	std::cout << "Hola" << nombre;
}
//Se despide del jugador, la despedida varia segun gane el jugador, el autómata o ninguno de ellos (el jugador abandone)
despedirse(string nombre){

	if (ganador == Nadie){
		std::cout << "¿Abandonas? Ohhh..." << endl;
		std::cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
	else if (ganador == jugador){
		std::cout << "Enhorabuena, has ganado" << endl;
		std::cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
	else {
		std::cout << "Lo siento, he ganado" << endl;
		std::cout << "Hasta la proxima " << nombre << "(pulsa una tecla)";
	}
}
	ganador = pasaCalculadora();
	despedirse(ganador)
	return 0;
	}
//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona devuelve Nadie.
tJugador pasaCalculadora(){
	//Variables
	tJugador turno;
	int total = 0, ultimoDigito = 0;
	const int META=31;
}
//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
quienEmpieza(){
	if (cstlib::rand() % 2)
	cout << "Tu empiezas";
	return Jugador;
	else cout"Empiezo yo"; 
	return Automata;
}
//Define que números se encuentran en la misma fila que el ultimo pulsado
mismaFila(int ultimo, int nuevo, double filaUltimo, filaNuevo){
	filaUltimo = (ultimo/3);
	filaNuevo = (nuevo/3);
	return ceil(filaUltimo) == ceil(filaNuevo);
}
//Define que números se encuentran en la misma columna que el ultimo
mismaColumna(int ultimo, int nuevo, columnaUltimo, columnaNuevo){
	columnaUltimo = (ultimo % 3);
	columnaNuevo = (nuevo % 3);
	return columnaUltimo == columnaNuevo;
}
//Determina que digitos se pueden pulsar en función de las reglas del juego
digitoValido(int ultimo, int nuevo){
	return ((mismaFila(int ultimo, int nuevo, double filaUltimo, filaNuevo))||(mismaColumna(int ultimo, int nuevo, int columnaUltimo, int columnaNuevo)))&&(ultimo!=nuevo);
}
//Genera un dígito aleatorio
digitoAleatorio(){
	return (cstlib::rand() % 9) + 1;
}
	//Inicializar partida
	cstlib::srand(time(NULL))//Semilla
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
