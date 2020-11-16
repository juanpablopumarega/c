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
#include <string.h>
#include <semaphore.h>
//#include <thread>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <signal.h>

#define FIL 4
#define COL 4
#define PARFIN 8
using namespace std;

void mostrarTablero(char mat[][COL]);

void help();

void inicializarMat(char mat[][COL]);

bool huboCoincidencia(int fil1, int col1, int *filCol, char mat[][COL]);

void taparCasillas(int fil1, int col1, int *filCol, char mat[][COL]);

void iniciarJuego(char mat[][COL]);

void borrarCaracter(char *cad, char caracter);

void reiniciarJuego(int mat[][COL]);

void handlerSigSigusr1(int sig);

void actualizarTablero(char tablero[][COL], char mat[][COL], int fil, int col);

void* funcionDentroDelHilo(void* pidJuego);

void crearHilo ();

static int SIGUSR1_reciv = 0;

static int finJuego = 0;

int main(int argc, char const *argv[])
{
    if (argc != 1)
    {
        if (argc > 2)
        {
            cout << "La cantidad de parametros es incorrecta";
            return 1;
        }
        else if (!strcmp(argv[1],"--help") || !strcmp(argv[1], "-h"))
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

    sem_t *esperaClient = sem_open("esperaCliente", O_CREAT, 0600, 0);
    sem_t *blockServer = sem_open("bloqueoServer", O_CREAT, 0600, 1);
    sem_t *esperaIng = sem_open("esperaIngPar1", O_CREAT, 0600, 0);
    sem_t *semTablero = sem_open("escribTab", O_CREAT, 0600, 0);
    sem_t *semMuetra = sem_open("muestraTab", O_CREAT, 0600, 0);
    sem_t *semFinJuego = sem_open("finJuego", O_CREAT, 0600, 0);
    //sem_t *esperaIngPar2 = sem_open("esperaIngPar2", O_CREAT, 0600, 0);

    sem_wait(blockServer); //permite que se ejecute 1 solo server

    signal(SIGUSR1, handlerSigSigusr1); //Señal de corte

    ///--TABLERO ///
    int fil1, col1;
    int fd = shm_open("tablero", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(char[FIL][COL]));
    char(*tablero)[COL];
    tablero = (char(*)[COL])mmap(NULL, sizeof(char[FIL][COL]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    inicializarMat(tablero);

    char mat[FIL][COL];

    ///--MEM COMP FIL Y COL /// filCol[0]->fil1 | filCol[1]->col1
    int fd2 = shm_open("filaYcolumna", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[2]));
    int *filCol = (int *)mmap(NULL, sizeof(int[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd);

    signal(SIGINT, SIG_IGN); //ignora ctrl+c
    cout << "esperando jugador" << endl;
    
    /// Creo hilo para controlar señal de corte
    //crearHilo ();
    
    while (!SIGUSR1_reciv)
    {
        inicializarMat(tablero);
        iniciarJuego(mat);
        mostrarTablero(mat);
        finJuego = 0;
        sem_post(semTablero);
        sem_wait(esperaClient); ///Espera cliente
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
            sem_wait(semMuetra);
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
    }

    //----Liberacion de recursos
    //munmap(mat, sizeof(int[4][4]));
    //shm_unlink("matrizMemotest");

    munmap(filCol, sizeof(int[2]));
    shm_unlink("filaYcolumna");
    munmap(tablero, sizeof(int[FIL][COL]));
    shm_unlink("tablero");

    sem_close(semFinJuego);
    shm_unlink("finJuego");

    sem_close(esperaClient);
    sem_unlink("esperaCliente");
    sem_close(blockServer);
    sem_unlink("bloqueoServer");
    sem_close(esperaIng);
    sem_unlink("esperaIngPar1");
    sem_close(semTablero);
    sem_unlink("escribTab");
    sem_close(semMuetra);
    sem_unlink("muestraTab");

    return EXIT_SUCCESS;
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
        SIGUSR1_reciv = 1;
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
    cout << "  1 2 3 4 " << endl;
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
    cout << "Nuestro script replica el clasico juego memotest" << endl;
    cout << "Debera en un tablero de 4x4 indicar las letras que estan repetidas" << endl;
    cout << "Solo un jugador podra jugar a la vez" << endl;
    cout << "Al finalizar la partida se le mostrara el tiempo que tardo en realizarlo" << endl;
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

void *funcionDentroDelHilo(void* pidJuego) {
    signal(SIGUSR1, handlerSigSigusr1); //Señal de corte

    pid_t* pid = (pid_t*) pidJuego;

    while(true){
        while (!SIGUSR1_reciv) {
            if(finJuego){
            kill( -*pid, SIGKILL);
            kill( -getgid(), SIGKILL);
            }
            
        }
    }
}

void crearHilo () {
    
    // id del hilo
    pthread_t hilo; 
    pid_t pidJuego = getgid();

    //
    pthread_create(&hilo, NULL, &funcionDentroDelHilo, &pidJuego);

    pthread_join(hilo, NULL); 
    pthread_exit(NULL); 

}