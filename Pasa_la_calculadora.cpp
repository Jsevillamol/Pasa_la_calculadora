/*-------------------------------
PASA LA CALCULADORA
Autores: 
Jaime Sevilla Molina
Victor Gonzalez
Fecha
2014/11
Version: 3.0
---------------------------------*/

//BIBLIOTECAS
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

//VARIABLES GLOBALES
const int META=31;

//TIPOS PROPIOS
typedef enum tJugador
{
	Nadie,
	Jugador,
	Automata
};

//FUNCIONES
//FUNCIONES DE JUEGO

string saludar();
void despedirse (tJugador ganador, string nombre);
int menu();

tJugador pasaCalculadora();
tJugador quienEmpieza();

bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);

//FUNCIONES DE IA
int digitoAleatorio();
int digitoAutomata(int ultimo);

//FUNCIONES DE JUGADOR

int digitoEntre(int a, int b);

int digitoPersona();
int digitoPersona(int ultimo);

char mNumero(int ultimo, int n);
void mostrarCalculadora(int ultimo);
void mostrarCalculadora();

//FUNCIONES DE ARCHIVO
bool mostrar(string archivo);

bool stats(int ganadas, int perdidas, int abandonadas);

//FUNCIONES DE SISTEMA
void pausa();

int main()
{
	tJugador ganador;
	int opcion;
	int ganadas = 0, perdidas = 0, abandonadas = 0;
	
	string nombre = saludar();	
	//Bucle Menu
	do
	{
		opcion = menu();

		if(opcion == 1)
		{
			ganador = pasaCalculadora();
			
			if     (ganador == Jugador)  ganadas += 1;
			else if(ganador == Automata) perdidas += 1;
			else if(ganador == Nadie)    abandonadas += 1;
			
			despedirse(ganador, nombre);
		}

		else if(opcion == 2) mostrar("acerca.txt");
	}
	while(opcion != 0);
	
	stats(ganadas, perdidas, abandonadas);
	
	cout << "Hasta la proxima " << nombre << " (pulsa enter)";
	pausa();

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
	cout << "1 - Jugar" << endl;
	cout << "2 - Acerca de" << endl;
	cout << "0 - Salir" << endl;
	
	int seleccionar = digitoEntre(0,2);

	return seleccionar;
}

//Muestra el archivo "acerca.txt" siempre que este no contenga errores
bool mostrar(string archivo)
{
	bool ok;
	ifstream acerca;
	char c;

	acerca.open(archivo);
	
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

//Actualiza las estadisticas
bool stats(int ganadas, int perdidas, int abandonadas)
{
	bool ok;
	int ejecuciones, ganadas2, totales, abandonadas2;
	ifstream stats;
	ofstream actualizar;
	
	stats.open("stats.txt");
	if(stats.is_open())
	{
		stats >> ejecuciones;
		stats >> ganadas2;
		stats >> totales;
		stats >> abandonadas2;
		
		ok = true;
	}
	else
	{
		ejecuciones = 0;
		ganadas2 = 0;
		totales = 0;
		abandonadas2 = 0;
		
		cout << "El archivo 'stats.txt' no se encontro, se ha creado un nuevo archivo" << endl;
		
		ok = false;
	}
	
	stats.close();
	
	ganadas += ganadas2;
	totales += perdidas+ganadas+abandonadas;
	abandonadas += abandonadas2;
	
	actualizar.open("stats.txt");
	
	actualizar << ejecuciones + 1 << endl;
	actualizar << totales << endl;
	actualizar << ganadas << endl;
	actualizar << abandonadas << endl;
	
	actualizar.close();
	return ok;
}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona, devuelve Nadie.
tJugador pasaCalculadora()
{
	//Variables
	tJugador turno;

	int total = 0, ultimoDigito = 0;


	srand(time(NULL));//Semilla
	turno = quienEmpieza();

	//Bucle de juego
	
	//Turno jugador
	if (turno == Jugador)
	{
		ultimoDigito = digitoPersona();
		turno = Automata;
	}
	//Turno bot
	else /*if (turno == Automata)*/
	{
		ultimoDigito = digitoAleatorio();
		cout << "Elijo el numero " << ultimoDigito << endl;
		turno = Jugador;
	}
	total += ultimoDigito;
	cout << "Total = " << total << endl << endl;
	
	while ((total < META) && (ultimoDigito != 0))
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
			cout << "Elijo el numero " << ultimoDigito << endl;
			turno = Jugador;
		}
		total += ultimoDigito;
		cout << "Total = " << total << endl << endl;
	}
	
	
	if (ultimoDigito == 0) return Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}

//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
tJugador quienEmpieza()
{
	if ((rand() % 2))
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
bool digitoValido(int ultimo, int nuevo)
{
	return ((mismaFila(ultimo, nuevo))||(mismaColumna(ultimo, nuevo)))&&(ultimo!=nuevo);
}

//Genera un digito aleatorio del 1 al 9
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

	return digito;
}

//Pide un digito al jugador. Solo devolvera un valor valido (entre 0 y 9).
//Para un valor no valido, mostrara un error.
int digitoEntre(int a, int b)
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

		else if (digito < a || digito > b)
		{
		cout << "Error! Introduce un digito entre " << a << " y " << b << endl;
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
		digito = digitoEntre(0,9);
		if (!digitoValido(ultimo, digito) && (digito != 0))
		{
			cout << "Error! El digito debe estar en la misma fila y columna que el ultimo" << endl;
			digito = -1;
		}
	}
	while (digito == -1);

	cout << "Has elegido el " << digito << endl;

	return digito;
}

int digitoPersona()
{
	mostrarCalculadora();
	return digitoEntre(0,9);
}

//Permite al jugador poner en pausa el juego
void pausa()
{
	system("pausa");
}

//Determina si el numero de la calculadora se muestra o no, en funcion de si es valido
char mNumero(int ultimo, int n)
{
	if(digitoValido(ultimo, n))
	{
		return char (n+int('0'));
	}
	else
	{
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
}

void mostrarCalculadora()
{
	for (int j=2; j>=0; j--)
	{
		for (int i = 1; i<4; i++)
		{
		cout << setw(3) << 3*j+i;
		}
		cout << endl;
	}
}
