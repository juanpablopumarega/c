#include <iostream>
#include <stdlib.h>
#include <unistd.h>
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
using namespace std;

void borrarCaracter(char *cad, char caracter);

void reiniciarJuego(int mat[][4]);

void handlerSigSigusr1(int sig);

static int SIGUSR1_reciv = 0;

int main()
{

    sem_t *esperaClient = sem_open("esperaCliente", O_CREAT, 0600, 1);
    sem_t *blockServer = sem_open("bloqueoServer", O_CREAT, 0600, 1);
    sem_t *esperaIngreso = sem_open("esperaIngreso", O_CREAT, 0600, 1);

    sem_wait(blockServer); //permite que se ejecute 1 solo server

    signal(SIGUSR1, handlerSigSigusr1); //Señal de corte
    ///--MEM COMP MAT ///
    int fd = shm_open("matrizMemotest", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[4][4]));
    char(*mat)[4];
    mat = (char(*)[4])mmap(NULL, sizeof(char[4][4]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 1);
    close(fd);

    ///--MEM COMP FIL Y COL /// filCol[0]->fil | filCol[1]->col
    int fd2 = shm_open("filaYcolumna", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[2]));
    int *filCol;
    filCol = (int *)mmap(NULL, sizeof(int[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 1);
    close(fd);

    //signal(SIGINT, SIG_IGN) ignora ctrl+c

    sem_wait(esperaClient); ///Espera cliente

    while (!SIGUSR1_reciv)
    {
        while (!SIGUSR1_reciv)
        {
            
        }
    }

    //----Liberacion de recursos
    munmap(mat, sizeof(int[4][4]));
    shm_unlink("matrizMemotest");
    munmap(filCol, sizeof(int[2]));
    shm_unlink("filaYcolumna");
    sem_close(esperaClient);
    sem_unlink("esperaCliente");
    sem_close(blockServer);
    sem_unlink("bloqueoServer");
    sem_close(esperaIngreso);
    sem_unlink("esperaIngreso");

    return EXIT_SUCCESS;
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

void reiniciarJuego(int mat[][4])
{
    int completoPar, completoMath = 0, cantLetras = 26;
    char vecLetras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", letra;
    int numRandF, numRandCol;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            mat[i][j] = '-';
    }
    srand(time(NULL));
    while (completoMath < 8)
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