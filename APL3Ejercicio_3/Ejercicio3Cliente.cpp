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

using namespace std;

int main()
{
    int fd = shm_open("matrizMemotest", O_CREAT | O_RDWR, 0600); //fileDescriptor
    sem_t *esperaClient = sem_open("esperaCliente", O_CREAT,0600, 1);
    sem_t *bloqClient = sem_open("bloqueoCliente", O_CREAT,0600, 1);

    sem_wait(bloqClient);  //-->un solo cliente

    //signal(SIGINT, SIG_IGN) ignora ctrl+c

    ///--MEM COMP MAT ///
    ftruncate(fd, sizeof(int[4][4]));
    int(*ptMat)[4];
    ptMat = (int(*)[4])mmap(NULL, sizeof(int[4][4]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

     ///--MEM COMP FIL Y COL ///
    int fd2 = shm_open("filaYcolumna", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[2]));
    int * filCol;
    filCol = (int*)mmap(NULL, sizeof(int[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 1);
    close(fd);

    int k = 0,fil,col;

    ptMat[2][2]=5;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            cout << " " << ptMat[i][j];
        cout << endl;
    }

    sem_post(esperaClient);   ///Aviso al server
    
    cout<<"Ingrese primera fila:";
    cin>>fil;
    cout<<"Ingrese primera columna:";
    cin>>col;

    cout<<"Ingrese segunda fila:";
    cin>>fil;
    cout<<"Ingrese segunda columna:";
    cin>>col;

    munmap(ptMat,sizeof(int[4][4]));  ///Libera Mem
    shm_unlink("matrizMemotest");   ///Borra Archivo
    sem_close(bloqClient);
    sem_unlink("bloqueoClient");

    return EXIT_SUCCESS;
}