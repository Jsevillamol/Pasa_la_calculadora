/*-------------------------------
PASA LA CALCULADORA
Autores: 
Jaime Sevilla Molina
Victor Gonzalez del Hierro
Fecha
2014/11
Version: 6.0
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
	Automata,
	Jugador1,
	Jugador2
};

typedef enum tDificultad
{
	Facil,
	Dificil,
	Imposible
};

typedef enum tModo
{
	Salir,
	Single,
	Double
};

//FUNCIONES
void despedirse (tJugador ganador, string usuario);
void despedirseDoble(tJugador ganador, string usuario1, string usuario2, tJugador turno);

void des_activar_cheats(bool &cheats);

//FUNCIONES DE JUEGO
tJugador pasaCalculadora(bool cheats);
tJugador pasaCalculadora2(bool cheats, string usuario1, string usuario2, tJugador &turno);
tJugador quienEmpieza(tDificultad dificultad, bool cheats);
tJugador quienEmpiezaDoble(bool cheats, string usuario1, string usuario2);

bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);

//MENUS
int menu();
tDificultad seleccionar_dificultad();
int seleccionar_modo_de_juego();

//FUNCIONES DE IA
int digitoAleatorio();
int digitoAutomata(int ultimo);
int botDificil(int total, int ultimo);
int botImposible(int ultimoDigito, int total);
bool minimax(int ultimoDigito, int total, bool maximizing);

//FUNCIONES DE JUGADOR

int digitoEntre(int a, int b);

int digitoPersona();
int digitoPersona(int ultimo);

char mNumero(int ultimo, int n);
void mostrarCalculadora();
void mostrarCalculadora(int ultimo);

//FUNCIONES DE ARCHIVO
bool mostrar(string archivo);

void registrar_nueva_ejecucion();
string iniciar_sesion1();
string iniciar_sesion2(string usuario1);
string usuario_valido();
void cambio_sesion(string &usuario1, string &usuario2);
bool actualizar_stats(tJugador ganador, string usuario);
void actualizar_stats_doble (tJugador ganador, tJugador turno, string usuario1, string usuario2);
void stats(string usuario1);
void stats2(string usuario1, string usuario2);

void fcopy(string origen, string destino);
bool restore_from_backup();

string reset(string usuario1);
void reset2(string &usuario1, string &usuario2);
void hard_reset();
void soft_reset(string usuario1);
void delete_user(string usuario);

//FUNCIONES DE SISTEMA
void pause();

int main()
{
	int opcion, modo;
	bool cheats = false;
	tJugador ganador, turno;
	string usuario1, usuario2;
	
	cout << "Bienvenido a Pasa la calculadora!" << endl;
	
	usuario1 = iniciar_sesion1();
	registrar_nueva_ejecucion();

	//Bucle Menu
	do{
		modo = seleccionar_modo_de_juego();
		if(modo == Single)
		{
			do
			{
				cout << usuario1 << " VS BOT" << endl;

				opcion = menu();
	
				if(opcion == 1)
				{
					ganador = pasaCalculadora(cheats);
					actualizar_stats(ganador, usuario1);
					despedirse(ganador, usuario1);
				}
	
				else if(opcion == 2) 
				{
					if(!mostrar("acerca.txt")) 
						cout << "Error, el archivo 'acerca.txt' no existe" << endl;
				}
	
				else if(opcion == 3) stats(usuario1);
	
				else if (opcion == 4)
				{
					cout << "Hasta la proxima, " << usuario1 << endl;
					usuario1 = iniciar_sesion1();
				}
				else if(opcion == 5) usuario1 = reset(usuario1);
	
				else if(opcion == 6)
					des_activar_cheats(cheats);
			}
			while(opcion != 0);
		}

		else if(modo == Double)
		{
			usuario2 = iniciar_sesion2(usuario1);
			do
			{
				cout << usuario1 << " VS " << usuario2 << endl;
				
				opcion = menu();
	
				if(opcion == 1)
				{
					ganador = pasaCalculadora2(cheats, usuario1, usuario2, turno);
					actualizar_stats_doble(ganador, turno, usuario1, usuario2);
					despedirseDoble(ganador, usuario1, usuario2, turno);
				}
	
				else if(opcion == 2) 
				{
					if(!mostrar("acerca.txt")) 
						cout << "Error, el archivo 'acerca.txt' no existe" << endl;
				}
	
				else if(opcion == 3) stats2(usuario1, usuario2);
	
				else if(opcion == 4) cambio_sesion(usuario1, usuario2);
				
				else if(opcion == 5) reset2(usuario1, usuario2);
	
				else if(opcion == 6)
					des_activar_cheats(cheats);
			}
			while(opcion != 0);

			cout << setfill('-') << setw(79) << '-'        << endl
			     << "Hasta la proxima " << usuario2 << "." << endl
			     << setfill(' ');
			pause();
		}
	}
	while(modo != Salir);
	cout << setfill('-') << setw(79) << '-'        << endl
	     << "Hasta la proxima " << usuario1 << "." << endl
	     << setfill(' ');
	pause();
}

//FUNCIONES
//Se despide del jugador, la despedida varia segun gane el jugador, 
//el automata o ninguno de ellos (el jugador abandone)
void despedirse(tJugador ganador, string usuario)
{
	if (ganador == Nadie){
		cout << "Abandonas, "  << usuario << "? Ohhh..."     << endl << endl;
	}
	else if (ganador == Jugador){
		cout << "Enhorabuena " << usuario << ", has ganado!" << endl << endl;
	}
	else /*if (ganador == Automata)*/{
		cout << "Lo siento "   << usuario << ", he ganado"   << endl << endl;
	}
}

//Se despide del jugador, la despedida varia segun gane el jugador 1, 
//el jugador 2 o ninguno de ellos (el uno de los abandone)
void despedirseDoble(tJugador ganador, string usuario1, string usuario2, tJugador turno)
{
	if (ganador == Nadie)
	{
		if (turno == Jugador1)
		{
			cout << "Abandonas " << usuario2 << "? Ohhh..." << endl << endl;
		}
		else /*if (turno == Jugador2)*/ 
		{
			cout << "Abandonas " << usuario1 << "? Ohhh..." << endl << endl;
		}
	}
	else if (ganador == Jugador1)
	{
		cout << "Enhorabuena " << usuario1 << ", has ganado!"  << endl << endl;
	}
	else /*if (ganador == Jugador2)*/
	{
		cout << "Enhorabuena " << usuario2 << ", has ganado!"  << endl << endl;
	}
}

void des_activar_cheats(bool &cheats)
{
	cheats = !cheats;

	cout << setfill('-') << setw(79) << '-' << endl << setfill(' ');

	if(cheats)
	{
		cout << "Trampas activadas" << endl << endl;
	}
	else /*if(!cheats)*/
	{
		cout << "Trampas desactivadas" << endl << endl;
	}
}
//FUNCIONES DE JUEGO
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

	//Primer turno
	if (turno == Jugador)
	{
		ultimoDigito = digitoPersona();
		turno = Automata;
	}
	else /*if (turno == Automata)*/
	{
		if (dificultad == Facil)
		{
			ultimoDigito = digitoAleatorio();
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

	//Bucle de juego
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
	
	if (ultimoDigito == 0) return Nadie; 
	//Si el jugador abandona, no gana nadie

	return turno;
}
tJugador pasaCalculadora2(bool cheats, string usuario1, string usuario2, tJugador &turno)
{
	int total = 0, ultimoDigito = 0;

	srand(time(NULL));//Semilla
	
	turno = quienEmpiezaDoble(cheats, usuario1, usuario2);
	
	//Primer turno
	if (turno == Jugador1)
	{
		cout << "Te toca, " << usuario1 << ":" << endl;
		ultimoDigito = digitoPersona();
		turno = Jugador2;
	}
	else /*if (turno == Jugador2)*/
	{
		cout << "Te toca, " << usuario2 << ":" << endl;
		ultimoDigito = digitoPersona();
		turno = Jugador1;
	}
	total += ultimoDigito;
	cout << "Total = " << total << endl << endl;

	//Bucle de juego
	while ((total < META) && (ultimoDigito != 0))
	{
		//Turno jugador1
		if (turno == Jugador1)
		{
			cout << "Te toca, " << usuario1 << ":" << endl;
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Jugador2;
		}
		//Turno jugador2
		else /*if (turno == Jugador2)*/
		{
			cout << "Te toca, " << usuario2 << ":" << endl;
			ultimoDigito = digitoPersona(ultimoDigito);
			turno = Jugador1;
		}
		total += ultimoDigito;
		cout << "Total = " << total << endl << endl;
	}
	
	
	if (ultimoDigito == 0) return Nadie; 
	else return turno;
	//Si un jugador abandona, no gana nadie

}
//Decide aleatoriamente quien empieza la partida, si el automata o el jugador
tJugador quienEmpieza(tDificultad dificultad, bool cheats)
{
	if ((rand() % 2 && dificultad != Imposible) || cheats)
	{
		cout << setfill('-') << setw(79) << '-' << endl
		     << "Tu empiezas"            << endl
		     << setfill(' ');
		return Jugador;
	}
	else
	{
		cout << setfill('-') << setw(79) << '-' << endl
		     << "Empiezo yo"             << endl
	             << setfill(' ');
	             
		return Automata;
	}
}

//Decide aleatoriamente quien empieza la partida, si el jugador 1 o el jugador 2
tJugador quienEmpiezaDoble(bool cheats, string usuario1, string usuario2)
{
	if ((rand() % 2 || cheats))
	{
		cout << setfill('-') << setw(79) << '-' << endl
		     << "Empieza " << usuario1          << endl
		     << setfill(' ');
		return Jugador1;
	}
	else
	{
		cout << setfill('-') << setw(79) << '-' << endl
		     << "Empieza "   << usuario2        << endl
	         << setfill(' ');             
		return Jugador2;
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

//MENUS
//Proporciona al jugador la posibilidad de jugar, ver las instrucciones o las estadisticas del juego, 
//cambiar de usuario, o salir.
int menu()
{
	cout << setfill('-') << setw(79) << '-'       << endl
	     << "1 - Jugar"                           << endl
	     << "2 - Acerca de"                       << endl
	     << "3 - Estadisticas"                    << endl
	     << "4 - Iniciar sesion con otro usuario" << endl
	     << "5 - Resetear estadisticas"           << endl
	     << "0 - Cambiar modo de juego / Salir"   << endl
	     << setfill(' ');
	
	int seleccionar = digitoEntre(0,6);

	return seleccionar;
}

//Permite al jugador seleccionar la dificultad del juego
tDificultad seleccionar_dificultad()
{
	cout << setfill('-') << setw(79) << '-' << endl
	     << "Elige dificultad:"             << endl
	     << "1 - Facil"                     << endl
	     << "2 - Dificil"                   << endl
	     << "3 - Imposible"                 << endl
	     << setfill(' ');

	int opcion = digitoEntre(1,3);

	if        (opcion == 1)   return Facil;
	else if   (opcion == 2)   return Dificil;
	else /*if (opcion == 3)*/ return Imposible;
}

//Permite selecionar al jugador si quiere jugar 
//contra el automata o contra otro jugador
int seleccionar_modo_de_juego()
{
	cout << setfill('-') << setw(79) << '-' << endl
	     << "Selecciona el modo de juego:"   << endl
	     << "1 - Un jugador"                << endl
	     << "2 - Dos jugadores"             << endl
	     << "0 - Salir"                     << endl
	     << setfill(' ');
	
	int modo = digitoEntre(0,2);

	return modo;
}

//FUNCIONES DE IA
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
	int movimiento=-1;
	if (total == 0) movimiento=9;
	else
	{
		//Buscamos un movimiento optimo
		for (int digito=1; digito<10 && movimiento == -1; digito++)
		{
			if (digitoValido(ultimoDigito, digito))
			{
				//cout << "Empezando minimax para " << digito << endl;
				if (minimax(digito, total+digito, false))
					movimiento = digito;
				//cout << "Minimax acabado. Movimiento no optimo" << endl;
			}
		}
	}
	//Si no encontramos un movimiento optimo, devolvemos uno de botDificil
	if (movimiento == -1) movimiento = botDificil(total, ultimoDigito);

	return movimiento;
}

//Lets start the backtracking party.
//Esta funcion devuelve true si el movimiento es optimo
bool minimax(int ultimoDigito, int total, bool maximizing)
{
	if (total >= META) 
	{
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

//FUNCIONES DE JUGADOR
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

int digitoPersona()
{
	mostrarCalculadora();

	cout << "Escoge un numero (0 para abandonar)" << endl;

	int digito = digitoEntre(0,9);

	cout << "Has elegido el " << digito << endl;

	return digito;
}

//Pide un digito al jugador mostrando el teclado. Solo devolvera un valor 
//que cumpla las reglas del juego o 0. Para un valor no valido, mostrara un error.
int digitoPersona(int ultimo)
{
	int digito = -1; //-1 es mi error flag
	
	mostrarCalculadora(ultimo);

	cout << "Escoge un numero (0 para abandonar)" << endl;

	do
	{
		digito = digitoEntre(0,9);
		if (!digitoValido(ultimo, digito) && digito !=0)
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
		return char(n+int('0'));
	else
		return ' ';
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

//FUNCIONES DE ARCHIVO
//Muestra el archivo "acerca.txt" siempre que este no contenga errores
bool mostrar(string archivo)
{
	bool ok;
	ifstream acerca;
	char c;
	
	cout << setfill('-') << setw(79) << '-' << endl;
	cout << setfill(' ');
	
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
	}
	return ok;
}

void registrar_nueva_ejecucion()
{
	int ejecuciones; char c;

	ifstream stats("stats.txt", ios::in);
	ofstream backup("backup.txt", ios::out);
	//Las comprobaciones de backup ya se han hecho en iniciar_sesion()

	stats >> ejecuciones;
	backup << ejecuciones+1;

	stats.get(c);
	while (!stats.eof())
	{
		backup.put(c);
		stats.get(c);
	}

	stats.close();
	backup.close();

	fcopy("backup.txt", "stats.txt");
}

//Devuelve el nombre de usuario, y crea su perfil 
//en las estadisticas si no existe
string iniciar_sesion1()
{
	//Conseguimos el nombre de usuario
	string usuario1;
	cout << setfill('-') << setw(79) << '-' << endl
	     << "Como te llamas? "
	     << setfill(' ');
	usuario1 = usuario_valido();

	string line;
	ofstream backup;
	//Abrimos el archivo stats
	ifstream stats;
	stats.open("stats.txt");

	//Restauracion desde backup, si es necesaria
	if (!stats.good())
	{
		cout << "\"stats.txt\" no existe. Buscando backup." << endl;
		stats.close();
		restore_from_backup();
		stats.open("stats.txt");
	}
	if (stats.good())
	{
		//Busqueda de la info del usuario
		getline(stats,line);
		while (line!=usuario1 && !stats.eof())
		{
			getline(stats,line);
		}
		//Si el usuario no existe, creamos un nuevo perfil de usuario.
		if (stats.eof())
		{
			cout << "Usuario no encontrado. Se creara un nuevo perfil." << endl;
			
			backup.open("backup.txt",ios::app);
				backup << usuario1 << endl
				       << 0        << endl  //Ganadas
				       << 0        << endl  //Perdidas
				       << 0        << endl  //Abandonadas
				                   << endl;
			backup.close();
		}
		else
		{
			cout << "Bienvenido de nuevo " << usuario1 << endl;
		}
	}
	else
		//Si el archivo stats no existe y no hay backup, creamos un nuevo archivo 
		//y agregamos el perfil del nuevo usuario
	{
		cout << "Un nuevo archivo sera creado." << endl;

		backup.open("backup.txt");
			backup << 0        << endl << endl //Ejecuciones
			       << usuario1 << endl         //Usuario
			       << 0        << endl         //Ganadas
			       << 0        << endl         //Perdidas
			       << 0        << endl         //Abandonadas
			                   << endl;
		backup.close();
	}
	stats.close();

	fcopy("backup.txt", "stats.txt");

return usuario1;
}

//Devuelve el nombre de usuario, y crea su perfil 
//en las estadisticas si no existe
string iniciar_sesion2(string usuario1)
{
	//Conseguimos el nombre de usuario
	string usuario2;
	cout << setfill('-') << setw(79) << '-' << endl
	     << "Contra quien vas a jugar?"     << endl
	     << setfill(' ');
	usuario2 = usuario_valido();
	
	while(usuario2 == usuario1)
	{
		cout << "Error, no puedes jugar contra ti mismo" << endl;
		usuario2 = usuario_valido();
	}

	string line;
	ofstream backup;
	//Abrimos el archivo stats
	ifstream stats;
	stats.open("stats.txt");

	//Restauracion desde backup, si es necesaria
	if (!stats.good())
	{
		cout << "\"stats.txt\" no existe. Buscando backup." << endl;
		stats.close();
		restore_from_backup();
		stats.open("stats.txt");
	}
	if (stats.good())
	{
		//Busqueda de la info del usuario
		getline(stats,line);
		while (line!=usuario2 && !stats.eof())
		{
			getline(stats,line);
		}
		//Si el usuario no existe, creamos un nuevo perfil de usuario.
		if (stats.eof())
		{
			cout << "Usuario no encontrado. Se creara un nuevo perfil." << endl;
			
			backup.open("backup.txt",ios::app);
				backup << usuario2 << endl
				       << 0        << endl  //Ganadas
				       << 0        << endl  //Perdidas
				       << 0        << endl  //Abandonadas
				                   << endl;
			backup.close();
		}
		else
		{
			cout << "Bienvenido de nuevo " << usuario2 << endl;
		}
	}
	else
		//Si el archivo stats no existe y no hay backup, creamos un nuevo archivo 
		//y agregamos el perfil del nuevo usuario
	{
		cout << "Un nuevo archivo sera creado." << endl;

		backup.open("backup.txt");
			backup << 0        << endl << endl //Ejecuciones
			       << usuario2 << endl         //Usuario
			       << 0        << endl         //Ganadas
			       << 0        << endl         //Perdidas
			       << 0        << endl         //Abandonadas
			                   << endl;
		backup.close();
	}
	stats.close();

	fcopy("backup.txt", "stats.txt");

return usuario2;
}

//Evita que el nombre de usuario empiece por un numero, para evitar 
//errores a la hora de buscar a ese usuario en el archivo stats.txt
string usuario_valido()
{
	string nombre;
	
	cin >> nombre;
	cin.clear();
	
	while(isdigit(nombre[0]))
	{
		cout << "Error, el nombre de usuario no puede empezar por un digito" << endl;
		cin >> nombre;
		cin.clear();
	}
	return nombre;
}

//En el modo 2P permite seleccionar que jugador quiere cambiar su usuario
void cambio_sesion(string &usuario1, string &usuario2)
{
	cout << setfill('-') << setw(79) << '-'              << endl
	     << "Que jugador quiere cambiar sesion?" << endl << endl
	     << "1- " << usuario1                            << endl
             << "2- " << usuario2                            << endl
	     << "3- Ambos"                                   << endl
	     << "0- Volver al menu"                          << endl
	     << setfill(' ');

	int sesion = digitoEntre(0,3);

	if (sesion == 1)
	{
		cout << "Hasta la proxima, " << usuario1 << endl;
		usuario1 = iniciar_sesion2(usuario2);
	}
	else if (sesion == 2)
	{
		cout << "Hasta la proxima, " << usuario2 << endl;
		usuario2 = iniciar_sesion2(usuario1);
	}
	else if (sesion == 3)
	{
		cout << "Hasta la proxima, " << usuario1 << endl
		     << "Hasta la proxima, " << usuario2 << endl;
		usuario1 = iniciar_sesion1();
		usuario2 = iniciar_sesion2(usuario1);
	}
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
	if (!stats.good())
	{
		cout << "Error! stats.txt no existe. Buscando backup..."<<endl;
		stats.close();

		restore_from_backup();

		stats.open("stats.txt");
	}

	actualizar.open("backup.txt");

	if(stats.good())
	{
		//Copia de stats a backup, hasta la info del usuario
		do
		{
			getline(stats, linea);
			actualizar << linea << endl;
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
		actualizar << abandonadas;

		char c;
		stats.get(c);
		while (!stats.eof())
		{
			actualizar.put(c);
			stats.get(c);
		}

		ok = true;
	}
	else
	{
		ganadas =   (ganador == Jugador) ?1 :0;
		perdidas = (ganador == Automata) ?1 :0;
		abandonadas = (ganador == Nadie) ?1 :0;

		actualizar << 1   << endl << endl //Ejecuciones
			   << usuario     << endl
		           << ganadas     << endl
		           << perdidas    << endl
		           << abandonadas << endl
		           << 	     	     endl;
		
		cout << "El archivo 'stats.txt' no se encontro, se ha creado un nuevo archivo" << endl;
		
		ok = false;
	}
	stats.close();
	actualizar.close();

	//Ahora copiamos la informacion actualizada en el archivo original
	fcopy("backup.txt", "stats.txt");
	
return ok;
}

void actualizar_stats_doble(tJugador ganador, tJugador turno, string usuario1, string usuario2)
{
	if (ganador == Jugador1)
	{
		actualizar_stats(Jugador, usuario1);
		actualizar_stats(Automata, usuario2);
	}
	else if (ganador == Jugador2)
	{
		actualizar_stats(Automata, usuario1);
		actualizar_stats(Jugador, usuario2);
	}
	else /*if ganador == Nadie*/
	{
		if (turno == Jugador1)
		{
			actualizar_stats(Jugador, usuario1);
			actualizar_stats(Nadie, usuario2);
		}
		else /* if (turno == Jugador2)*/
		{
			actualizar_stats(Nadie, usuario1);
			actualizar_stats(Jugador, usuario2);
		}
	}
}

//Muestra las estadisticas del jugador actual
void stats(string usuario) 
{ 
	ifstream stats; 
	string line;
	int ganadas, perdidas, abandonadas, ejecuciones; 
	 
	stats.open("stats.txt"); 

	stats >> ejecuciones;
	
	getline(stats, line);
	
	while(line != usuario)
	{
		getline(stats, line);
	}
	
	stats >>     ganadas;
	stats >>    perdidas;
	stats >> abandonadas;
	
	cout << setfill('-')   << setw(79)                  << '-'                            << endl;
	cout << "Numero de ejecuciones del programa: "      <<  ejecuciones                   << endl;
	cout << endl;
	cout << "Partidas de " << usuario << ": "           << (ganadas+perdidas+abandonadas) << endl; 
	cout << right
	     << setfill(' ') << setw(22) << "ganadas: "     <<  ganadas                       << endl 
	     << setfill(' ') << setw(22) << "perdidas: "    <<  perdidas                      << endl
	     << setfill(' ') << setw(22) << "abandonadas: " <<  abandonadas                   << endl
	     << endl;
	     
	stats.close();
}

void stats2(string usuario1, string usuario2) 
{ 
	cout << setfill('-') << setw(79) << '-' << endl
	     << "1 - " << usuario1               << endl
	     << "2 - " << usuario2               << endl
	     << "0 - Volver al menu"             << endl
	     << setfill(' ');

	int muestra = digitoEntre(0,2);
	
	if     (muestra == 1) stats(usuario1);
	else if(muestra == 2) stats(usuario2);
}

//Copia el contenido de un archivo en otro 
//por si fuera necesario restaurar un archivo
void fcopy(string origen, string destino)
{
	ifstream paso1;
	ofstream paso2;
	char 	  word;
	
	paso1.open(origen);
	paso2.open(destino);

	paso1.get(word);

	while(!paso1.eof())
	{
		paso2.put(word);
		paso1.get(word);
	}

	paso1.close();
	paso2.close();
}

//Copia backup.txt en stats.txt. Si backup no existe devuelve false
bool restore_from_backup()
{
	ifstream backup; bool ok;

	backup.open("backup.txt");
	if (backup.good())
	{
		backup.close();
		fcopy("backup.txt", "stats.txt");
		ok = true;
	}
	else
	{
		backup.close();
		cout << "El backup no ha sido encontrado" << endl;
		ok = false;
	}
return ok;
}

string reset(string usuario)
{
	cout << setfill('-') << setw(79) << '-'              << endl
	     << "1 - Borrar estadisticas del jugador actual" << endl
		 << "2 - Borrar perfil del jugador actual"       << endl
	     << "3 - Borrar todas las estadisticas"          << endl
	     << "0 - Volver al menu"                         << endl;
	cout << setfill(' ');
	
	int opcion = digitoEntre(0,3);

	if (opcion == 1) 
		soft_reset(usuario);
	else if(opcion == 2)
	{
		delete_user(usuario);
		usuario = iniciar_sesion1();
	}
	else if (opcion == 3)
	{ 
		hard_reset();
		usuario = iniciar_sesion1();
	}
return usuario;
}

void reset2(string &usuario1, string &usuario2)
{
	cout << setfill('-') << setw(79) << '-'         << endl
	     << "1 - Borrar estadisticas de un jugador" << endl
		 << "2 - Borrar perfil de un jugador"       << endl
	     << "3 - Borrar todas las estadisticas"     << endl
	     << "0 - Volver al menu"                    << endl;
	cout << setfill(' ');
	
	int opcion = digitoEntre(0,3);

	if (opcion == 1) 
	{
		cout << setfill('-') << setw(79) << '-' << endl
			 << "1 - Borrar estadisticas de " << usuario1 << endl
			 << "2 - Borrar estadisticas de " << usuario2 << endl
			 << "0 - Salir"                               << endl
			 << setfill(' ');

		int option = digitoEntre(0,2);
	
		if     (option == 1) soft_reset(usuario1);
		else if(option == 2) soft_reset(usuario2);
	}
	else if(opcion == 2)
	{
		cout << setfill('-') << setw(79) << '-' << endl
			 << "1 - Borrar perfil de " << usuario1 << endl
			 << "2 - Borrar perfil de " << usuario2 << endl
			 << "0 - Salir"                               << endl
			 << setfill(' ');

		int option = digitoEntre(0,2);
	
		if     (option == 1) 
		{
			delete_user(usuario1);
			usuario1 = iniciar_sesion2(usuario2);
		}
		else if(option == 2) 
		{
			delete_user(usuario2);
			usuario2 = iniciar_sesion2(usuario1);
		}
	}
	else if (opcion == 3)
	{ 
		hard_reset();
		usuario1 = iniciar_sesion1();
		usuario2 = iniciar_sesion2(usuario1);
	}
}

void hard_reset()
{
	remove("backup.txt");
	remove("stats.txt");
}

void soft_reset(string usuario)
{
	int i;
	string linea;
	ifstream stats;
	ofstream actualizar;
	
	stats.open("stats.txt");

	//Restauracion con el backup, si es necesaria
	if (!stats.good())
	{
		cout << "Error! stats.txt no existe. Buscando backup..."<<endl;
		stats.close();

		restore_from_backup();

		stats.open("stats.txt");
	}

	actualizar.open("backup.txt");

	if(stats.good())
	{
		//Copia de stats a backup, hasta la info del usuario
		do
		{
			getline(stats, linea);
			actualizar << linea << endl;
		}
		while(linea != usuario);

		//Ignoramos los datos anteriores...
		stats >> i;
		stats >> i;
		stats >> i;
		 //...y los sutituimos por 0
		actualizar << 0 << endl; //Ganadas
		actualizar << 0 << endl; //Perdidas
		actualizar << 0;         //Abandonadas

		char c;
		stats.get(c);
		while (!stats.eof())
		{
			actualizar.put(c);
			stats.get(c);
		}
	}
	else
	{

		actualizar << 1   << endl << endl //Ejecuciones
			       << usuario     << endl
		           << 0 << endl
		           << 0 << endl
		           << 0 << endl
		           <<endl;
		
		cout << "El archivo 'stats.txt' no se encontro, se ha creado un nuevo archivo" << endl;
	}

	stats.close();
	actualizar.close();

	//Ahora copiamos la informacion actualizada en el archivo original
	fcopy("backup.txt", "stats.txt");
}

void delete_user(string usuario)
{
	string linea;
	ifstream stats;
	ofstream actualizar;
	
	stats.open("stats.txt");

	//Restauracion con el backup, si es necesaria
	if (!stats.good())
	{
		cout << "Error! stats.txt no existe. Buscando backup..."<<endl;
		stats.close();

		restore_from_backup();

		stats.open("stats.txt");
	}

	actualizar.open("backup.txt");

	if(stats.good())
	{
		//Copia de stats a backup, hasta la info del usuario
		getline(stats,linea);
		while(linea != usuario)
		{
			actualizar << linea << endl;
			getline(stats, linea);
		}

		//Ignoramos los datos anteriores
		for (int i=0; i<4;i++)
			stats.ignore(1000, '\n');

		//Copiamos el resto del archivo
		char c;
		stats.get(c);
		while (!stats.eof())
		{
			actualizar.put(c);
			stats.get(c);
		}
	}
	else
	{

		actualizar << 1   << endl << endl //Ejecuciones
		           <<endl;
		
		cout << "El archivo 'stats.txt' no se encontro, se ha creado un nuevo archivo" << endl;
	}

	stats.close();
	actualizar.close();

	//Ahora copiamos la informacion actualizada en el archivo original
	fcopy("backup.txt", "stats.txt");
}

//FUNCIONES DE SISTEMA
//Permite al jugador poner en pausa el juego
void pause()
{
	system("pause");
}
