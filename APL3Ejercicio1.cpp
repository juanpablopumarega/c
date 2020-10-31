#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(){

    cout << "Inicio del proceso" << endl;

    int arbolgenialogico [5];
    arbolgenialogico[0] = getpid();

    int pid2 = fork();
    

    if(pid2 > 0) {

            int pid3 = fork();

            if(pid3 > 0) {

                waitpid(pid3, NULL, 0);
                int pid4 = fork();

                if(pid4 > 0) {
                    waitpid(pid4, NULL, 0);
                } 
                else {
                    cout << "NIVEL 2 - NODO 4 - PADRE: " << &arbolgenialogico << " - MIO: " << getpid() << endl;
                }
            } 
            else {
                cout << "NIVEL 2 - NODO 3 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;

                int pid7 = fork();

                if(pid7 > 0) {
                    waitpid(pid7, NULL, 0);
                } 
                else {
                    cout << "NIVEL 3 - NODO 7 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;

                    int pid9 = fork();

                    if(pid9 > 0) {
                        waitpid(pid9, NULL, 0);
                    } 
                    else {
                        cout << "NIVEL 4 - NODO 9 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;

                        int pid10 = fork();

                        if(pid10 > 0) {
                            waitpid(pid10, NULL, 0);
                        } 
                        else {
                            cout << "NIVEL 5 - NODO 10 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;
                        }
                    }
                }
            }

        waitpid(pid2, NULL, 0);
    } 
    else {
            cout << "NIVEL 2 - NODO 2 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;

                int pid5 = fork();

                if(pid5 > 0) {
                    
                    waitpid(pid5, NULL, 0);
                    int pid6 = fork();

                    if(pid6 > 0) {
                        waitpid(pid6, NULL, 0);
                    } 
                    else {
                        cout << "NIVEL 3 - NODO 6 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;

                        int pid8 = fork();

                        if(pid8 > 0) {
                            waitpid(pid8, NULL, 0);
                        } 
                        else {
                            cout << "NIVEL 4 - NODO 8 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;
                        }
                    }

                } 
                else {
                    cout << "NIVEL 3 - NODO 5 - PADRE: " << getppid() << " - MIO: " << getpid() << endl;
                }
        }

    return EXIT_SUCCESS;

}
