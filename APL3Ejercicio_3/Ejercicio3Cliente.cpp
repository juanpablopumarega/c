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
#include <vector>

using namespace std;

int main()
{
    int fd = shm_open("matrizMemotest", O_CREAT | O_RDWR, 0600); //fileDescriptor
    ftruncate(fd, sizeof(int[4][4]));
    int(*ptMat)[4];
    ptMat = (int(*)[4])mmap(NULL, sizeof(int[4][4]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    int k = 0;

    ptMat[2][2]=5;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
            cout << " " << ptMat[i][j];
        cout << endl;
    }

    //munmap(ptMat,sizeof(int[4][4]));  ///Libera Mem
    //shm_unlink("matrizMemotest");   ///Borra Archivo

    return 1;
}