#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

void imprimirNivel(int nivel, int nodo, int arbol[], int miPid)
{
    cout << "Nivel: " << nivel << " Nodo: " << nodo;
    cout << " Predecesores: ";

    for (int i = 1; i < nivel; i++)
    {
        cout << " " << arbol[i - 1];
    }

    cout << " MiPid es: " << miPid << endl;
}

void ayuda()
{
    cout << "-------------------------------------------------------" << endl;
    cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..." << endl;
    cout << "\t- Nombre Script:     ./APL3Ejercicio1    " << endl;
    cout << "\t- Ejemplo de uso:    ./APL1Ejercicio1.cpp 3" << endl;
    cout << "\t- N - [Optional]     Numero entero entre 1 y 5 que indicará el nivel del arbol a generar" << endl;
    cout << "\t- Fin de la ayuda... espero te sirva!" << endl;
    cout << "-------------------------------------------------------" << endl;
}

int main(int argc, char *argv[])
{
    int nivel;
    //Iniciamos la validación de Parametros
    if (argc > 2)
    {
        cout << "Cantidad de parametros invalida." << endl;
        cout << "Llamando a la ayuda..." << endl;
        ayuda();
        return EXIT_SUCCESS;
    }
    else
    {
        if (argc == 2)
        {
            if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)
            {
                ayuda();
                return EXIT_SUCCESS;
            }
            else
            {
                if (atoi(argv[1]) < 1 || atoi(argv[1]) > 5)
                {
                    cout << "Se espera un numero natural menos o igual a 5." << endl;
                    cout << "Llamando a la ayuda..." << endl;
                    ayuda();
                    return EXIT_SUCCESS;
                }
                else
                    nivel = atoi(argv[1]);
            }
        }
        else
            nivel = 5;
    }
    //Fin de la validación de parametros.

    cout << "Inicio del proceso" << endl;
    cout << "Nivel: 1 Nodo: 1 MiPid: " << getpid() << endl;

    int arbolgenialogico[nivel];
    arbolgenialogico[0] = getpid();

    if (nivel == 1)
    {
        sleep(15);
        return EXIT_SUCCESS;
    }

    if (fork() > 0)
    {

        if (fork() > 0)
        {

            if (fork() > 0)
            {
                sleep(15);
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
            else
            {
                imprimirNivel(2, 4, arbolgenialogico, getpid());
            }
        }
        else
        {
            imprimirNivel(2, 3, arbolgenialogico, getpid());
            arbolgenialogico[1] = getpid();

            if (nivel == 2)
            {
                return EXIT_SUCCESS;
            }

            if (fork() > 0)
            {
                sleep(15);
                wait(NULL);
            }
            else
            {
                imprimirNivel(3, 7, arbolgenialogico, getpid());
                arbolgenialogico[2] = getpid();

                if (nivel == 3)
                {
                    return EXIT_SUCCESS;
                }

                if (fork() > 0)
                {
                    sleep(15);
                    wait(NULL);
                }
                else
                {
                    imprimirNivel(4, 9, arbolgenialogico, getpid());
                    arbolgenialogico[3] = getpid();

                    if (nivel == 4)
                    {
                        return EXIT_SUCCESS;
                    }

                    if (fork() > 0)
                    {
                        sleep(15);
                        wait(NULL);
                    }
                    else
                    {
                        imprimirNivel(5, 10, arbolgenialogico, getpid());
                    }
                }
            }
        }
    }
    else
    {
        imprimirNivel(2, 2, arbolgenialogico, getpid());
        arbolgenialogico[1] = getpid();

        if (nivel == 2)
        {
            return EXIT_SUCCESS;
        }

        if (fork() > 0)
        {

            if (fork() > 0)
            {
                sleep(15);
                wait(NULL);
                wait(NULL);
            }
            else
            {
                imprimirNivel(3, 6, arbolgenialogico, getpid());
                arbolgenialogico[2] = getpid();

                if (nivel == 3)
                {
                    return EXIT_SUCCESS;
                }

                if (fork() > 0)
                {
                    sleep(15);
                    wait(NULL);
                }
                else
                {
                    imprimirNivel(4, 8, arbolgenialogico, getpid());
                }
            }
        }
        else
        {
            imprimirNivel(3, 5, arbolgenialogico, getpid());
        }
    }
    return EXIT_SUCCESS;
}

//Fin
