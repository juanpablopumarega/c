//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        1                                 
// Entrega N°:       1                                 
// Nombre Script:    APL3Ejercicio1.cpp                 
// Ejemplo de uso:   ./APL3Ejercicio1.cpp 3             
// Grupo 2                                             
// Lopez Pumarega Juan Pablo             DNI:34593023  
// Miranda Andres                        DNI:32972232  
//------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

void imprimirNivel(int nivel, int nodo, int arbol[], int miPid){
    cout << "Nivel: " << nivel << " Nodo: " << nodo;
    cout << " Predecesores: ";

    for(int i=1;i<nivel;i++){
        cout << " " << arbol[i-1];
    }

    cout << " MiPid es: " << miPid << endl;
}

void ayuda(){
    cout << "-------------------------------------------------------" << endl;
    cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..." << endl;
    cout << "\t- Nombre Script:     ./APL3Ejercicio1.sh " << endl;
    cout << "\t- Ejemplo de uso:    ./APL1Ejercicio2.cpp 3" << endl;
    cout << "\t- N - [Required]     Numero entero entre 1 y 5 que indicará el nivel del arbol a generar" << endl;
    cout << "\t- Fin de la ayuda... espero te sirva!" << endl;
    cout << "-------------------------------------------------------" << endl;
}


int main(int argc, char *argv[]){

    //Iniciamos la validación de Parametros
    if(argc!=2) {
        cout << "Cantidad de parametros invalida." << endl;
        cout << "Llamando a la ayuda..." << endl;
        ayuda();
        return EXIT_SUCCESS;
    } else {
        if(strcmp(argv[1],"-help")==0 || strcmp(argv[1],"-h")==0){
            ayuda();
            return EXIT_SUCCESS;
        } else {
            if(atoi(argv[1])<1 || atoi(argv[1])>5) {
                cout << "Se espera un numero natural menos o igual a 5." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
        }
    }
    //Fin de la validación de parametros.

    int nivel = atoi(argv[1]);

    cout << "Inicio del proceso" << endl;
    cout << "Nivel: 1 Nodo: 1 MiPid: " << getpid() << endl;

    int arbolgenialogico [nivel];
    arbolgenialogico[0] = getpid();

    if(nivel==1) {
        return EXIT_SUCCESS;
    }

    int pid2 = fork();
    
    if(pid2 > 0) {

            int pid3 = fork();

            if(pid3 > 0) {

                waitpid(pid3, NULL, 0);
                int pid4 = fork();

                if(pid4 > 0) {
                    waitpid(pid4, NULL, 0);
                } else {
                    imprimirNivel(2,4,arbolgenialogico,getpid());
                }
            } else {
                imprimirNivel(2,3,arbolgenialogico,getpid());
                arbolgenialogico[1] = getpid();

                if(nivel==2) {
                    return EXIT_SUCCESS;
                }

                int pid7 = fork();

                if(pid7 > 0) {
                    waitpid(pid7, NULL, 0);
                } else {
                    imprimirNivel(3,7,arbolgenialogico,getpid());
                    arbolgenialogico[2] = getpid();
                    
                    if(nivel==3) {
                        return EXIT_SUCCESS;
                    }
                    
                    int pid9 = fork();

                    if(pid9 > 0) {
                        waitpid(pid9, NULL, 0);
                    } else {
                        imprimirNivel(4,9,arbolgenialogico,getpid());
                        arbolgenialogico[3] = getpid();

                        if(nivel==4) {
                            return EXIT_SUCCESS;
                        }

                        int pid10 = fork();

                        if(pid10 > 0) {
                            waitpid(pid10, NULL, 0);
                        } 
                        else {
                            imprimirNivel(5,10,arbolgenialogico,getpid());
                        }
                    }
                }
            }

        waitpid(pid2, NULL, 0);
    } else {
            imprimirNivel(2,2,arbolgenialogico,getpid());
            arbolgenialogico[1] = getpid();

            if(nivel==2) {
                return EXIT_SUCCESS;
            }

            int pid5 = fork();
            if(pid5 > 0) {
                
                waitpid(pid5, NULL, 0);
                int pid6 = fork();

                if(pid6 > 0) {
                    waitpid(pid6, NULL, 0);
                } else {
                    imprimirNivel(3,6,arbolgenialogico,getpid());
                    arbolgenialogico[2] = getpid();

                    if(nivel==3) {
                        return EXIT_SUCCESS;
                    }

                    int pid8 = fork();
                    if(pid8 > 0) {
                        waitpid(pid8, NULL, 0);
                    } else {
                        imprimirNivel(4,8,arbolgenialogico,getpid());
                    }
                }
            } else {
                imprimirNivel(3,5,arbolgenialogico,getpid());
            }
        }
    return EXIT_SUCCESS;
}

//Fin
