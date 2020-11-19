#include <iostream>
#include <ios>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <vector>
#include <ctime>

#define FIL 4
#define COL 4
#define PARFIN 8
#define BUFFER 10000

using namespace std;

void help();

///No ingreso la casilla desde el cliente es solo para una validacion
void ingresarCasillas(int *filcol, char mat[][COL]);

void hayJugadorActivo();

void mostrarTablero(char mat[][COL]);

bool validacionIng(char *ing, int tamMat, string msj);

bool validacionCasilla(char mat[][COL], int fil, int col);

int main(int argc, char const *argv[])
{
    int valor;

    if (argc != 1)
    {
        if (argc > 2)
        {
            cout << "La cantidad de parametros es incorrecta"<<endl;
            return 1;
        }
        else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
        {
            help();
            return 0;
        }
        else
        {
            cout << "El parametro ingresado es incorrecto"<<endl;
            return 1;
        }
    }

    sem_t *bloqClient = sem_open("bloqueoCliente", O_CREAT, 0600, 1);
    sem_getvalue(bloqClient, &valor);
    if (!valor)
    {
        hayJugadorActivo();
        return 0;
    }

    sem_wait(bloqClient); //-->un solo cliente

    signal(SIGINT, SIG_IGN);
    sem_t *esperaClient = sem_open("esperaCliente", O_CREAT, 0600, 0);
    sem_t *esperaIng = sem_open("esperaIngPar1", O_CREAT, 0600, 0);
    sem_t *semTablero = sem_open("escribTab", O_CREAT, 0600, 0);
    sem_t *semMuetra = sem_open("muestraTab", O_CREAT, 0600, 0);
    sem_t *semFinJuego = sem_open("finDeJuego", O_CREAT, 0600, 0);

    //signal(SIGINT, SIG_IGN) ignora ctrl+c

    ///--MEM COMP MAT ///
    int fd = shm_open("tablero", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(char[FIL][COL]));
    char(*tablero)[COL];
    tablero = (char(*)[COL])mmap(NULL, sizeof(char[FIL][COL]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    ///--MEM COMP FIL Y COL ///
    int fd2 = shm_open("filaYcolumna", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[2]));
    int *filCol = (int *)mmap(NULL, sizeof(int[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd);

    sem_post(esperaClient); ///Aviso al server
    sem_wait(semTablero);

    int finJuego;
    sem_getvalue(semFinJuego, &finJuego);

    time_t tiempoInicio, tiempoFin;
    time(&tiempoInicio);

    while (!finJuego)
    {
        mostrarTablero(tablero);
        ingresarCasillas(filCol, tablero);
        sem_post(esperaIng);
        sem_wait(semTablero);
        mostrarTablero(tablero);
        ingresarCasillas(filCol, tablero);
        sem_post(esperaIng);
        sem_wait(semTablero);
        mostrarTablero(tablero);
        sleep(1);
        system("clear");  ///opcional desaparece lo anterior
        sem_post(semMuetra); ///para que se pueda ver ambas casillas
        sem_wait(semTablero);
        sem_getvalue(semFinJuego, &finJuego);
    }
    sem_wait(semFinJuego);
    sem_post(bloqClient);
    time(&tiempoFin);
    time_t segTotal = tiempoFin - tiempoInicio;
    time_t segundos = segTotal % 60;
    time_t minutos = segTotal / 60;

    cout << "juego finalizado. Tiempo de juego: " << minutos << "m " << segundos << "s" << endl;

    sem_close(bloqClient);
    sem_unlink("bloqueoCliente");

    return EXIT_SUCCESS;
}

void hayJugadorActivo()
{
    cout << "Error,otro jugador se encuentra en la sala" << endl;
    cout << "Intente mas tarde" << endl;
}

void ingresarCasillas(int *filcol, char tablero[][COL])
{
    do
    {
        char str[BUFFER];
        do
        {
            cout << "Ingrese fila: ";
            cin.getline(str, BUFFER);
        } while (!validacionIng(str, FIL, "fila"));
        *filcol = str[0] - '0';
        do
        {
            cout << "Ingrese columna: ";
            cin.getline(str, BUFFER);

        } while (!validacionIng(str, COL, "columna"));
        *(filcol + 1) = str[0] - '0';
    } while (!validacionCasilla(tablero, *filcol, *(filcol + 1)));
}

bool validacionCasilla(char tablero[][COL], int fil, int col)
{
    if (tablero[fil - 1][col - 1] != '-')
    {
        cout << "Error,La casilla ingresada ya se encuentra desbloqueada" << endl;
        cout << "Ingrese nuevamente" << endl;
        return false;
    }
    return true;
}

bool validacionIng(char *ing, int tamMat, string msj)
{
    if (strlen(ing) != 1)
    {
        cout << "Error,Ingreso invalido" << endl;
        return false;
    }
    if (*ing < 48 || *ing > 57)
    {
        cout << "Error,debe ingresar un numero entero positivo" << endl;
        return false;
    }

    if ((*ing - '0') > tamMat || (*ing - '0') <= 0)
    {
        cout << "Error,La " << msj << " ingresada es incorrecta" << endl;
        return false;
    }

    return true;
}

void mostrarTablero(char tablero[][COL])
{
    cout << "  ";
    for (size_t i = 0; i < COL; i++)
    {
        cout << i + 1 << " ";
    }
    cout << endl;
    for (size_t i = 0; i < FIL; i++)
    {
        cout << i + 1;
        for (size_t j = 0; j < COL; j++)
            cout << " " << tablero[i][j];
        cout << endl;
    }
}

void help()
{
    cout << "AYUDA" << endl;
    cout << "Nuestro script replica el clasico juego memotest" << endl;
    cout << "Debera en un tablero de 4x4 indicar las letras que estan repetidas" << endl;
    cout << "Solo un jugador podra jugar a la vez" << endl;
    cout << "Al finalizar la partida se le mostrara el tiempo que tardo en realizarlo" << endl;
    cout << "SINTAXIS" << endl;
    cout << "./Ejercicio3Cliente.cpp" << endl;
    cout << "./Ejercicio3Cliente.cpp --help" << endl;
    cout << "./Ejercicio3Cliente.cpp -h" << endl;
}