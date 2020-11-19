#include <iostream>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string>
#include <signal.h>
#include <ios>


#define FIL 4
#define COL 4
#define PARFIN 8
using namespace std;

void mostrarTablero(char mat[][COL]);

void help();

void inicializarMat(char mat[][COL]);

void liberarRecursos();

bool huboCoincidencia(int fil1, int col1, int *filCol, char mat[][COL]);

void taparCasillas(int fil1, int col1, int *filCol, char mat[][COL]);

void iniciarJuego(char mat[][COL]);

void borrarCaracter(char *cad, char caracter);

void reiniciarJuego(int mat[][COL]);

void handlerSigSigusr1(int sig);

void actualizarTablero(char tablero[][COL], char mat[][COL], int fil, int col);

static int SIGUSR1_reciv = 0;

sem_t *esperaClient;
sem_t *blockServer;
sem_t *esperaIng;
sem_t *semTablero;
sem_t *semMuestra;
sem_t *semFinJuego;
char(*tablero)[COL];
int *filCol; 

static int clienteJugando = 0;

static int finJuego = 0;

int main(int argc, char const *argv[])
{   
    int valor;

    if (argc != 1)
    {
        if (argc > 2)
        {
            cout << "La cantidad de parametros es incorrecta";
            return 1;
        }
        else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
        {
            help();
            return 0;
        }
        else
        {
            cout << "El parametro ingresado es incorrecto";
            return 1;
        }
    }

    blockServer = sem_open("bloqueoServer", O_CREAT, 0600, 1);
    sem_getvalue(blockServer, &valor);
    if (!valor)
    {
        cout << "Error,El servidor ya ha sido iniciado" << endl;
        return 0;
    }

    sem_wait(blockServer);

    esperaClient = sem_open("esperaCliente", O_CREAT, 0600, 0);
    esperaIng = sem_open("esperaIngPar1", O_CREAT, 0600, 0);
    semTablero = sem_open("escribTab", O_CREAT, 0600, 0);
    semMuestra = sem_open("muestraTab", O_CREAT, 0600, 0);
    semFinJuego = sem_open("finDeJuego", O_CREAT, 0600, 0);

    signal(SIGUSR1, handlerSigSigusr1); //Señal de corte

    ///--TABLERO ///
    int fil1, col1;
    int fd = shm_open("tablero", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(char[FIL][COL]));
    tablero = (char(*)[COL])mmap(NULL, sizeof(char[FIL][COL]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    inicializarMat(tablero);

    char mat[FIL][COL];

    ///--MEM COMP FIL Y COL /// filCol[0]->fil1 | filCol[1]->col1
    int fd2 = shm_open("filaYcolumna", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[2]));
    filCol = (int *)mmap(NULL, sizeof(int[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd);

    signal(SIGINT, SIG_IGN); //ignora ctrl+c

    while (!SIGUSR1_reciv)
    {
        cout << "Esperando jugador..." << endl;
        cout << "Tablero destapado" << endl;
        inicializarMat(tablero);
        iniciarJuego(mat);
        mostrarTablero(mat);
        finJuego = 0;
        sem_post(semTablero);
        sem_wait(esperaClient); ///Espera cliente
        clienteJugando=1;
        while (finJuego != PARFIN)
        {
            sem_wait(esperaIng);
            fil1 = filCol[0] - 1;
            col1 = filCol[1] - 1;
            actualizarTablero(tablero, mat, filCol[0] - 1, filCol[1] - 1);
            sem_post(semTablero);
            sem_wait(esperaIng);
            actualizarTablero(tablero, mat, filCol[0] - 1, filCol[1] - 1);
            sem_post(semTablero);
            sem_wait(semMuestra);
            if (!huboCoincidencia(fil1, col1, filCol, mat))
                taparCasillas(fil1, col1, filCol, tablero);
            else
            {
                finJuego++;
                if (finJuego == PARFIN)
                    sem_post(semFinJuego);
            }
            sem_post(semTablero);
        }
        clienteJugando=0;
    }
    sem_post(blockServer);
    liberarRecursos();

    return EXIT_SUCCESS;
}

void liberarRecursos()
{
    munmap(filCol, sizeof(int[2]));
    shm_unlink("filaYcolumna");
    munmap(tablero, sizeof(int[FIL][COL]));
    shm_unlink("tablero");

    sem_close(semFinJuego);
    sem_unlink("finDeJuego");
    sem_close(esperaClient);
    sem_unlink("esperaCliente");
    sem_close(blockServer);
    sem_unlink("bloqueoServer");
    sem_close(esperaIng);
    sem_unlink("esperaIngPar1");
    sem_close(semTablero);
    sem_unlink("escribTab");
    sem_close(semMuestra);
    sem_unlink("muestraTab");
}

void taparCasillas(int fil1, int col1, int *filCol, char mat[][COL])
{
    mat[fil1][col1] = '-';
    mat[(*filCol - 1)][(*(filCol + 1) - 1)] = '-';
}

bool huboCoincidencia(int fil1, int col1, int *filCol, char mat[][COL])
{
    if (mat[fil1][col1] == mat[(*filCol - 1)][(*(filCol + 1) - 1)])
        return true;
}

void actualizarTablero(char tablero[][COL], char mat[][COL], int fil, int col)
{
    tablero[fil][col] = mat[fil][col];
}

void handlerSigSigusr1(int sig)
{

    if (SIGUSR1 == sig)
    {
        if (clienteJugando == 0)
        {
            cout << "Se recibio señal(" << sig << ")se interrumpira el proceso" << endl;
            liberarRecursos();
            exit(sig);
        }
        else
        {
            cout << "Se recibio señal(" << sig << ")se interrumpira el proceso" << endl;
            cout << "una vez finalizada la partida" << endl;
            SIGUSR1_reciv = 1;
        }
    }
}

void borrarCaracter(char *cad, char caracter)
{
    char *pe;
    while (*cad != caracter)
        cad++;
    pe = cad + 1;
    while (*pe)
    {
        *cad = *pe;
        cad++;
        pe++;
    }
    *cad = '\0';
}

void inicializarMat(char mat[][COL])
{
    for (size_t i = 0; i < FIL; i++)
    {
        for (size_t j = 0; j < COL; j++)
            mat[i][j] = '-';
    }
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
    cout << "./Ejercicio3Servidor.cpp" << endl;
    cout << "./Ejercicio3Servidor.cpp --help" << endl;
    cout << "./Ejercicio3Servidor.cpp -h" << endl;
}

void iniciarJuego(char mat[][COL])
{
    int completoPar, completoMath = 0, cantLetras = 26;
    char vecLetras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", letra;
    int numRandF, numRandCol;
    inicializarMat(mat);
    srand(time(NULL));
    while (completoMath < PARFIN)
    {
        completoPar = 0;
        letra = vecLetras[random() % cantLetras];
        borrarCaracter(vecLetras, letra);
        while (completoPar < 2)
        {
            numRandF = random() % 5;
            numRandCol = random() % 5;
            if (mat[numRandF][numRandCol] == '-')
            {
                mat[numRandF][numRandCol] = letra;
                completoPar++;
            }
        }
        cantLetras--;
        completoMath++;
    }
}
