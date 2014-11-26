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


void despedirse (tJugador ganador, string nombre);

//MENUS
int menu();
tDificultad seleccionar_dificultad();

//FUNCIONES DE JUEGO
tJugador pasaCalculadora(bool cheats);
tJugador quienEmpieza(tDificultad dificultad, bool cheats);

bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);

//FUNCIONES DE IA
int digitoAleatorio();
int digitoAutomata(int ultimo);
int botDificil(int total, int ultimo);
int botImposible(int ultimoDigito, int total);
bool minimax(int ultimoDigito, int total, bool maximizing);

//FUNCIONES DE JUGADOR

int digitoEntre(int a, int b);

int digitoPersona(int ultimo);

char mNumero(int ultimo, int n);
void mostrarCalculadora(int ultimo);

//FUNCIONES DE ARCHIVO
bool acerca();

string saludar();
bool actualizar_stats(tJugador ganador, string usuario);
void stats(string usuario);

void fcopy(string origen, string destino);

//FUNCIONES DE SISTEMA
void pause();

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

		if(opcion == 1)
		{
			ganador = pasaCalculadora(cheats);
			actualizar_stats(ganador, nombre);
			despedirse(ganador, nombre);

		}

		else if(opcion == 2) acerca();

		else if(opcion == 3) stats(nombre);

		else if (opcion == 4) 
		{
			cheats = true;
			cout << "Trampas activadas" << endl;
		}
	}
	while(opcion != 0);
	
	cout << "Hasta la proxima " << nombre << " (pulsa enter)";
	pause();

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
	cout << "3 - Estadisticas" << endl;
	cout << "0 - Salir" << endl;
	
	int seleccionar = digitoEntre(0,4);

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

//Actualiza las estadisticas
bool actualizar_stats(tJugador ganador, string usuario)
{
	bool ok;
	int ganadas, perdidas, abandonadas;
	string linea;
	ifstream stats;
	ofstream actualizar;
	
	stats.open("stats.txt");

	//Restauracion con el backup, si es necesaria
	if (!stats.is_open())
	{
		cout << "Error! stats.txt no existe. Buscando backup..."<<endl;
		stats.close();
		stats.open("backup.txt");
		if (stats.is_open())
		{
			cout << "backup encontrado. Restaurando stats"<<endl;
			fcopy("backup.txt", "stats.txt");
		}
		else
		{
			cout << "No pudo encontrarse el backup. Un nuevo archivo sera creado."<<endl;
		}
		stats.close();
		stats.open("stats.txt");
	}
	actualizar.open("backup.txt");

	if(stats.is_open())
	{
		//Copia de stats a backup, hasta la info del usuario
		do
		{
			getline(stats, linea);
			actualizar <<   linea;
		}
		while(linea != usuario);

		//Actualizacion de datos
		stats >> ganadas;
		stats >> perdidas;
		stats >> abandonadas;

		if     (ganador == Jugador)   ganadas += 1;
		else if(ganador == Automata) perdidas += 1;
		else if(ganador == Nadie) abandonadas += 1;

		actualizar << ganadas     << endl;
		actualizar << perdidas    << endl;
		actualizar << abandonadas << endl;
		actualizar << endl;
		
		//Copia el resto del archivo
		getline(stats, linea);
		while (!stats.eof())
		{
			getline(stats, linea);
			actualizar <<   linea;
		}

		ok = true;
	}
	else
	{
		ganadas =   (ganador == Jugador) ?1 :0;
		perdidas = (ganador == Automata) ?1 :0;
		abandonadas = (ganador == Nadie) ?1 :0;

		actualizar << usuario     << endl;
		actualizar << ganadas     << endl;
		actualizar << perdidas    << endl;
		actualizar << abandonadas << endl;
		actualizar << 		     endl;
		
		cout << "El archivo 'stats.txt' no se encontro, se ha creado un nuevo archivo" << endl;
		
		ok = false;
	}
	
	//Ahora copiamos la informacion actualizada en el archivo original
	fcopy("backup.txt", "stats.txt");
	
	stats.close();
	actualizar.close();

	return ok;
}

//Muestra las estadisticas de cada jugador
void stats(string nombre) 
{ 
	ifstream stats; 
	string line;
	int ganadas, perdidas, abandonadas; 
	 
	stats.open("stats.txt") 

	stats.getline(stats, line);
	stats <<      ganadas; 
	stats <<     perdidas; 
	stats <<  abandonadas; 
	stats <<         endl;
	
	while(line != nombre)
	{
		getline(stats, line);
	}
	
	cout <<  nombre 		 <<  ":" 			   << endl;
	cout << "Partidas jugadas: " 	 << (ganadas+perdidas+abandonadas) << endl; 
	cout << "	  ganadas: "     <<  ganadas    		   << endl; 
	cout << "	  perdidas: "    <<  perdidas    		   << endl; 
	cout << "	  abandonadas: " <<  abandonadas 		   << endl; 
	cout <<								      endl;
 
	stats.close(); 
}

//Conduce el desarrollo del juego y devuelve el ganador. 
//Si se abandona, devuelve Nadie.
tJugador pasaCalculadora(bool cheats)
{
	//Variables
	tJugador turno; tDificultad dificultad;

	int total = 0, ultimoDigito = 0;

	//Inicializar partida
	dificultad = seleccionar_dificultad();

	srand(time(NULL));//Semilla
	turno = quienEmpieza(dificultad, cheats);

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
				ultimoDigito = botImposible(ultimoDigito, total);
			}

			cout << "Elijo el numero " << ultimoDigito << endl;

			turno = Jugador;
		}
		total += ultimoDigito;
		cout << "Total = " << total << endl << endl;
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

//Permite al jugador poner en pausa el juego
void pause()
{
	system("pause");
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

tDificultad seleccionar_dificultad()
{
	cout << "Elige dificultad:" << endl;
	cout << "1 - Facil"         << endl;
	cout << "2 - Dificil"       << endl;
	cout << "3 - Imposible"     << endl;

	int opcion = digitoEntre(1,3);

	if        (opcion == 1)   return Facil;
	else if   (opcion == 2)   return Dificil;
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

int botImposible(int ultimoDigito, int total)
{
	//if (total == 0) return 9;
	for (int digito=1; digito<10; digito++)
	{
		if (digitoValido(ultimoDigito, digito))
		{
			//cout << "Empezando minimax para " << digito << endl;
			if (minimax(digito, total+digito, false))
				return digito;
			//cout << "Minimax acabado. Movimiento no optimo" << endl;
		}
	}
	return digitoAutomata(ultimoDigito);
}

//Lets start the backtracking party.
//Esta funcion devuelve true si el movimiento es optimo
bool minimax(int ultimoDigito, int total, bool maximizing)
{
	if (total >= META) {
		//cout << "Encontrado nodo terminal " << maximizing << endl;
		return maximizing;
	}
	if (maximizing)
	{
		bool bestValue = false;
		for (int i=1; i<10 && !bestValue; i++)
		{
			if (digitoValido(ultimoDigito, i))
			{
				//cout << "Maximizando" << endl;
				bestValue = minimax(i, total+i, false);
			}
		}
		//cout << "Nodo maximo analizado" << endl;
		return bestValue;
	}

	else /*if minimizing*/
	{
		bool worseValue = true;
		for (int i=1; i<10 && worseValue; i++)
		{
			if (digitoValido(ultimoDigito, i))
			{
				//cout << "Minimizando" << endl;
				worseValue = minimax(i, total+i, true);
			}
		}

		//cout << "Nodo minimo analizado" << endl;
		return worseValue;
	}
}
