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
#include <fcntl.h>
#include <string.h>
#include <string>
using namespace std;

void borrarCaracter(char*cad,char caracter)
{   
    char*pe;
    while(*cad!=caracter)
        cad++;
    pe=cad+1;
    while(*pe)
    {
        *cad=*pe;
        cad++;
        pe++;
    }
    *cad='\0';
}


int main()
{
    int completoPar,completoMath=0,cantLetras=26;
    char vecLetras[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ",letra;
    int numRandF,numRandCol;

    int fd = shm_open("matrizMemotest", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[4][4]));
    char(*mat)[4];
    mat = (char(*)[4])mmap(NULL, sizeof(char[4][4]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            mat[i][j] = '-';
    } 
    srand(time(NULL));   
    while (completoMath < 8)
    {   
        completoPar=0;
        letra = vecLetras[random() % cantLetras];
        borrarCaracter(vecLetras,letra);       
        while(completoPar < 2)
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
    

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            cout << " " << mat[i][j];
        cout << endl;
    }

    //munmap(ptMat,sizeof(int[4][4]));
    //shm_unlink("matrizMemotest");
    return EXIT_SUCCESS;
}