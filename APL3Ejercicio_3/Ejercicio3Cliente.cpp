#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
//#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <vector>

#define FIL 2
#define COL 2
#define PARFIN 2

using namespace std;

void help();

void ingresarCasillas(int *filcol);

void mostrarTablero(char mat[][COL]);

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
    sem_t *bloqClient = sem_open("bloqueoCliente", O_CREAT, 0600, 1);
    sem_t *esperaIng = sem_open("esperaIngPar1", O_CREAT, 0600, 0);
    sem_t *semTablero = sem_open("escribTab", O_CREAT, 0600, 0);
    sem_t *semMuetra = sem_open("muestraTab", O_CREAT, 0600, 0);
    sem_t *semFinJuego = sem_open("finJuego", O_CREAT, 0600, 0);

    sem_wait(bloqClient); //-->un solo cliente

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

    while (!finJuego)
    {   
        mostrarTablero(tablero);
        ingresarCasillas(filCol);
        sem_post(esperaIng);
        sem_wait(semTablero);
        mostrarTablero(tablero);
        ingresarCasillas(filCol);
        sem_post(esperaIng);
        sem_wait(semTablero);
        mostrarTablero(tablero);
        sleep(1);
        sem_post(semMuetra);                   ///para que se pueda ver ambas casillas
        sem_wait(semTablero);
        sem_getvalue(semFinJuego, &finJuego);
    }
    sem_wait(semFinJuego);
    cout<<"juego finalizado";
   

    //munmap(tablero,sizeof(int[4][4]));  ///Libera Mem
    //shm_unlink("matrizMemotest");   ///Borra Archivo
    sem_close(bloqClient);
    sem_unlink("bloqueoCliente");

    return EXIT_SUCCESS;
}

void ingresarCasillas(int *filcol)
{
    cout << "Ingrese fila:";
    cin >> *filcol;
    cout << "Ingrese columna:";
    cin >> *(filcol + 1);
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