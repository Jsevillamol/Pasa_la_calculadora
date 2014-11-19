/*-------------------------------
PASA LA CALCULADORA
Autores: 
Jaime Sevilla Molina
Victor Gonzalez
Fecha
2014/11
Version: 2.0
---------------------------------*/

//BIBLIOTECAS
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

//TIPOS PROPIOS
typedef enum tJugador
{
	Nadie,
	Jugador,
	Automata
};

typedef enum tDificultad
{
		Facil,
		Dificil,
		Imposible
};

//FUNCIONES
//FUNCIONES DE JUEGO

string saludar ();
void despedirse (tJugador ganador, string nombre);
int menu();
bool acerca();
tJugador pasaCalculadora();
tJugador quienEmpieza();

tDificultad seleccionar_dificultad();
bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);

//FUNCIONES DE IA NIVEL 1
int digitoAleatorio();

int digitoAutomata(int ultimo);

//FUNCIONES DE JUGADOR

int digitoPersona();

int digitoPersona(int ultimo);

char mNumero(int ultimo, int n);

void mostrarCalculadora(int ultimo);


/* Las funciones a continuacion se implementaran en un futuro
//FUNCIONES DE INFORME
//Actualiza el archivo informePC.txt con los tres argumentos. En caso de no encontrar
//el archivo, lo crea y devuelve false; en otro caso devuelve true.
bool actInforme(int jugadas, int ganadas, int abandonos);
*/

//FUNCIONES DE DIFICULTAD AVANZADA
int botDificil(int ultimo);


int main()
{
	tJugador ganador;
	int opcion;
	
	string nombre = saludar();	
	//Bucle Menu
	do
	{
		opcion = menu();
		if(opcion == 1){	
			ganador = pasaCalculadora();
			despedirse(ganador, nombre);
		}
		else if(opcion == 2) acerca();
	
	}
	while(opcion != 0);
	
	cout << "Hasta la proxima " << nombre << "(pulsa enter)";
	cin;

	return 0;
}
	
//Saluda al jugador y le pregunta su nombre
string saludar()
{
	string nombre;
	cout << "Bienvenido a Pasa la calculadora!" << endl;
	cout << "Como te llamas? ";
	cin >> nombre;
	cout << "Hola " << nombre << endl << endl;
	return nombre;
}

//Se despide del jugador, la despedida varia segun gane el jugador, el automata o ninguno de ellos (el jugador abandone)
void despedirse(tJugador ganador, string nombre)
{
	if (ganador == Nadie){
		cout << "Abandonas, " << nombre << "? Ohhh..." << endl << endl;
	}
	else if (ganador == Jugador){
		cout << "Enhorabuena " << nombre << ", has ganado!" << endl << endl;
	}
	else /*if (ganador == Automata)*/{
		cout << "Lo siento " << nombre << ", he ganado" << endl << endl;
	}
}

//Proporciona al jugador la posibilidad de jugar, ver las instrucciones del juego o salir.
int menu()
{
	int seleccionar = -1; //Error flag
	cout << "1 - Jugar" << endl;
	cout << "2 - Acerca de" << endl;
	cout << "0 - Salir" << endl;
	
	do
	{
		cin.sync(); //Por si quedan datos basura en el buffer
		cin >> seleccionar;

		if(cin.fail())
		{
			cout << "Error! Introduce un digito" << endl;
			cin.clear();
		}

		else if (seleccionar < 0 || seleccionar > 2)
		{
			cout << "Error! Introduce un digito entre 0 y 2" << endl;
			seleccionar = -1;
		}
		
	}
	while (seleccionar == -1);

	return seleccionar;
}

//Muestra el archivo "acerca.txt" siempre que este no contenga errores
bool acerca()
{

	bool ok;
	ifstream acerca;
	char c;

	acerca.open("acerca.txt");
	
	if(acerca.is_open())
	{
		acerca.get(c); //Lee el primer caracter

		while(!acerca.fail()) //Mientras la lectura no falle
		{
			cout << c;
			acerca.get(c); //Lee el siguiente caracter
		}

		ok = true;
		acerca.close();

	}
	else
	{
		ok = false;
		cout << "Error, el archivo 'acerca.txt' no existe" << endl;
	}

	return ok;
}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona, devuelve Nadie.
tJugador pasaCalculadora()
{
	//Variables
	tJugador turno; tDificultad dificultad;
	int total = 0, ultimoDigito = 0;
	const int META=31;
		//Inicializar partida

	dificultad = seleccionar_dificultad();

	srand(time(NULL));//Semilla
	turno = quienEmpieza();

	//Bucle de juego
	do
	{
		//Turno jugador
		if (turno == Jugador)
		{
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Automata;
		}
		//Turno bot
		else /*if (turno == Automata)*/
		{
			ultimoDigito = digitoAutomata(ultimoDigito);
			turno = Jugador;
		}
		total += ultimoDigito;
		cout << "Total = " << total << endl;
	}
	while ((total < META) && (ultimoDigito != 0));
	
	if (ultimoDigito == 0) turno = Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}

//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
tJugador quienEmpieza()
{
	if (rand() % 2)
	{
		cout << "Tu empiezas" << endl;
		return Jugador;
	}
	else
	{
		cout << "Empiezo yo" << endl;
		return Automata;
	}
}

//Define que numeros se encuentran en la misma fila que el ultimo pulsado
bool mismaFila(int ultimo, int nuevo)
{
	int filaUltimo, filaNuevo;
	filaUltimo = ((ultimo - 1)/3);
	filaNuevo = ((nuevo - 1)/3);
	return (filaUltimo) == (filaNuevo);
}

//Define que numeros se encuentran en la misma columna que el ultimo
bool mismaColumna(int ultimo, int nuevo)
{
	int columnaUltimo, columnaNuevo;
	columnaUltimo = (ultimo % 3);
	columnaNuevo = (nuevo % 3);
	return columnaUltimo == columnaNuevo;
}

//Determina que digitos se pueden pulsar en funcion de las reglas del juego
bool digitoValido(int ultimo, int nuevo)  //hay un bug
{
	if (ultimo == 0) return true;//Si es el primer turno, todos los numeros valen

	return ((mismaFila(ultimo, nuevo))||(mismaColumna(ultimo, nuevo)))&&(ultimo!=nuevo);
}

//Genera un digito aleatorio
int digitoAleatorio()
{
	return (rand() % 9) + 1;
}

//Devuelve un digito que cumpla las reglas del juego con respecto a ultimo.
int digitoAutomata(int ultimo)
{
	int digito;

	do
	{
	digito = digitoAleatorio();
	}
	while (!digitoValido(ultimo, digito));

	cout << "Elijo el numero " << digito << endl;

	return digito;
}

//Pide un digito al jugador. Solo devolvera un valor valido (entre 0 y 9).
//Para un valor no valido, mostrara un error.
int digitoPersona()
{
	int digito = -1;

	do
	{
		cin.sync(); //Por si quedan datos basura en el buffer
		cin >> digito;

		if (cin.fail())
		{
			cout << "Error! Introduce un digito" << endl;
			cin.clear();
		}

		else if (digito < 0 || digito > 9) 
		{
		cout << "Error! Introduce un digito entre 0 y 9" << endl;
		digito = -1;
		}
		
	}
	while (digito == -1);

	return digito;
}

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo)
{
	int digito = -1; //-1 es mi error flag
	
	mostrarCalculadora(ultimo);

	do
	{
		digito = digitoPersona();
		if (!digitoValido(ultimo, digito))
		{
			cout << "Error! El digito debe estar en la misma fila y columna que el ultimo" << endl;
			digito = -1;
		}
	}
	while (digito == -1);

	cout << "Has elegido el " << digito << endl;

	return digito;
}

//Determina si el numero de la calculadora se muestra o no, en funcion de si es valido
char mNumero(int ultimo, int n)
{
	if(digitoValido(ultimo, n))
	{
		//system ("color 02");
		return char (n+int('0'));
		
	}
	else
	{
		//system ("color 04");
		return ' ';
	}
}

//Muestra los botones de la calculadora (solo los que se pueden pulsar en cada turno)
void mostrarCalculadora(int ultimo)
{
	for (int i = 7; i<10; i++)
	{
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
	for (int i = 4; i<7; i++)
	{
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
	for (int i = 1; i<4; i++)
	{
		cout << setw(3) << mNumero(ultimo, i);
	}
	cout << endl;
	//system ("color 07");
}

tDificultad seleccionar_dificultad(){
	cout << "Elige dificultad:" << endl;
	cout << "1 - Facil" << endl;
	cout << "2 - Dificil" << endl;
	cout << "3 - Imposible" << endl;

	if 


int botDificil(int total, int ultimo)
{
	for (int i=1, i<10, i++)
	{
		if digitoValido(ultimo, i)
		{
			if (total + i == 30) return i; //Intenta ganar dejando la suma en 30
			else if ((total + i == 29) && (!digitoValido(i, 1))) return i; 
			//Si la suma queda en 29 y el adversario no puede coger el 1, ganamos
			else if ((total + i == 28) && (i==6 || i==9)) return i; 
			//Si la suma queda en 28 y el adv no puede coger el 1 o el 2, ganamos
		}
	}
	return digitoAutomata(ultimo);
}