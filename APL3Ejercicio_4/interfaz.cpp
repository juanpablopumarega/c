//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        1                                 
// Entrega N°:       1                                 
// Nombre Script:    APL3Ejercicio4.cpp                 
// Ejemplo de uso:                              
// Grupo 2                                             
// Lopez Pumarega Juan Pablo             DNI:34593023  
// Miranda Andres                        DNI:32972232  
//------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]){
    
    string accion;

    cout << "INGRESE COMANDO: ";
    cin >> accion;

    while(accion != "quit"){

        //INICIANDO LA CONEXION DE FIFO
        char contenido[] = "hola que hace";
        int fifoClienteServidor = open("./fifo/clienteServidor", 01);
        write(fifoClienteServidor,contenido,strlen(contenido));
        close(fifoClienteServidor);
        
        fifoClienteServidor = open("./fifo/clienteServidor", 00);
        read(fifoClienteServidor,contenido,sizeof(contenido));
        cout << contenido << endl;
        close(fifoClienteServidor);
        //FIN DE LA CONEXION

        cout << "INGRESE COMANDO: ";
        cin >> accion;

    }
    
    



    return EXIT_SUCCESS;
}