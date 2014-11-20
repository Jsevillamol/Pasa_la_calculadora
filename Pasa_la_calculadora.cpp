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

//VARIABLES GLOBALES
const int META=31;

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

string saludar();
void despedirse (tJugador ganador, string nombre);
int menu();
bool acerca();

tJugador pasaCalculadora(bool cheats);
tJugador quienEmpieza(tDificultad dificultad, bool cheats);

tDificultad seleccionar_dificultad();

bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);

//FUNCIONES DE IA
int digitoAleatorio();
int digitoAutomata(int ultimo);
int botDificil(int total, int ultimo);
int botImposible(int turno, int ultimoDigito, int total);

//FUNCIONES DE JUGADOR

int digitoEntre(int a, int b);

int digitoPersona(int ultimo);

char mNumero(int ultimo, int n);
void mostrarCalculadora(int ultimo);


/* Las funciones a continuacion se implementaran en un futuro
//FUNCIONES DE INFORME
//Actualiza el archivo informePC.txt con los tres argumentos. En caso de no encontrar
//el archivo, lo crea y devuelve false; en otro caso devuelve true.
bool actInforme(int jugadas, int ganadas, int abandonos);
*/



int main()
{
	tJugador ganador;
	int opcion;
	bool cheats = false;
	
	string nombre = saludar();	
	//Bucle Menu
	do
	{
		opcion = menu();
		if(opcion == 1){	
			ganador = pasaCalculadora(cheats);
			despedirse(ganador, nombre);
		}
		else if(opcion == 2) acerca();
		else if (opcion == 3) 
		{
			cheats = true;
			cout << "Trampas activadas" << endl;
		}
	
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
	
	seleccionar = digitoEntre(0,3);

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
tJugador pasaCalculadora(bool cheats)
{
	//Variables
	tJugador turno; tDificultad dificultad;

	int total = 0, ultimoDigito = 0, ronda = 1;

	//Inicializar partida
	dificultad = seleccionar_dificultad();

	srand(time(NULL));//Semilla
	turno = quienEmpieza(dificultad, cheats);

	//Bucle de juego
	do
	{
		cout << "Turno: " << ronda << endl;
		//Turno jugador
		if (turno == Jugador)
		{
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Automata;
		}
		//Turno bot
		else /*if (turno == Automata)*/
		{
			if (dificultad == Facil)
			{
				ultimoDigito = digitoAutomata(ultimoDigito);
			}
			else if (dificultad == Dificil)
			{
				ultimoDigito = botDificil(total, ultimoDigito);
			}
			else /*if dificultad == Imposible*/
			{
				ultimoDigito = botImposible(ronda, ultimoDigito, total);
			}

			cout << "Elijo el numero " << ultimoDigito << endl;

			turno = Jugador;
		}
		total += ultimoDigito;
		cout << "Total = " << total << endl << endl;
		ronda++;
	}
	while ((total < META) && (ultimoDigito != 0));
	
	if (ultimoDigito == 0) turno = Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}

//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
tJugador quienEmpieza(tDificultad dificultad, bool cheats)
{
	if ((rand() % 2 && dificultad != Imposible) || cheats)
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
	if (ultimo == 0) return true;//Si es el primer turno, todos los numeros valen

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

tDificultad seleccionar_dificultad()
{
	cout << "Elige dificultad:" << endl;
	cout << "1 - Facil" << endl;
	cout << "2 - Dificil" << endl;
	cout << "3 - Imposible" << endl;

	int opcion = digitoEntre(1,3);

	if (opcion == 1) return Facil;
	else if (opcion == 2) return Dificil;
	else /*if (opcion == 3)*/ return Imposible;
}

int botDificil(int total, int ultimo)
{
	int ganamos = 0, menos_da_una_piedra = 0;

	//Empezamos a jugar en serio cuando la partida esta a punto de acabarse
	if (total > 20)
	{
		//Busqueda de un digito que nos haga ganar
		for (int i=1; !ganamos && i<10; i++)
		{
			if (digitoValido(ultimo, i))
			{
				if (total + i == 30) ganamos = i; //Intenta ganar dejando la suma en 30
				else if ((total + i == 29) && (!digitoValido(i, 1))) ganamos = i; 
				//Si la suma queda en 29 y el adversario no puede coger el 1, ganamos
				else if ((total + i == 28) && (i==6 || i==9)) ganamos = i; 
				//Si la suma queda en 28 y el adv no puede coger el 1 o el 2, ganamos

				//Si la suma es menor que 30, al menos no perdemos. 
				//En caso de que la busqueda no de resultado, usaremos esto.
				else if (total + i < META) menos_da_una_piedra = i;
			}
		}
	}
	//Si la busqueda tiene exito, devolvemos el resultado. 
	//En otro caso, jugamos aleatoriamente.
	if (ganamos)
		return ganamos;
	else if (menos_da_una_piedra)
		return menos_da_una_piedra;
	else
		return digitoAutomata(ultimo);
}

int botImposible(int turno, int ultimoDigito, int total)
{
	int digito;
	
	switch(turno){
	case 1:
		digito = 9;
		break;

	case 3:
		if      (ultimoDigito == 8) digito = 9;
		else if (ultimoDigito == 7) digito = 9;
		else if (ultimoDigito == 6) digito = 3;
		else if (ultimoDigito == 3) digito = 6;
		break;

	case 5:
		if (total = 29) digito = 1;
		else if (total = 28) digito = 2;
		else
		{
			if      (ultimoDigito == 1) digito = 3;
			else if (ultimoDigito == 2) digito = 5;
			else if (ultimoDigito == 3) digito = 9;
			else if (ultimoDigito == 4) digito = 6;
			else if (ultimoDigito == 5) digito = 6;
			else if (ultimoDigito == 6) digito = 5;
			else if (ultimoDigito == 9) digito = 3;
		}
		break;

	case 7:
		if      (ultimoDigito == 1) digito = 4;
		else if (ultimoDigito == 2) digito = 3;
		else if (ultimoDigito == 4) digito = 1;
		break;

	case 9:
		if      (ultimoDigito == 1) digito = 2;
		else if (ultimoDigito == 2) digito = 1;
		break;
	}

	return digito;
}