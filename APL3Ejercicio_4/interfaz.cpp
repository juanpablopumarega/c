//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        4                                 
// Entrega N°:       1                                 
// Nombre Script:    interfaz.cpp                 
// Ejemplo de uso:                              
// Grupo 2                                             
// Juarez Miguel                         DNI:38530113
// Grassi Jonatan                        DNI:40077893
// Lopez Pumarega Juan Pablo             DNI:34593023
// Miranda Andres                        DNI:32972232
// Salerti Natalia                       DNI:41559796
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

bool isValidADD(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;

    int contador = 0;
    size_t found = 0;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {

        if(columna == 1 && dato != "IN") {
            return false; //si el ADD no esta seguido del IN
        }

       if(columna == 2){
            found=dato.find("=");
            
            if (found != string::npos) {
                return false; //en el nombre de la tabla tenemos un =
            }
       }

       if(columna > 2) {
            found=dato.find("=");

            if (found != string::npos) {
                contador++;                
            } else {
                return false; //no tenemos clave valor.
            }
       }

    }

    if(contador < 2){
        return false; //menor a 2 campos
    }

    return true;
}

bool isValidREMOVEorFIND(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;
    int columna = 0;

    for (columna = 0; std::getline(registro, dato, ' '); ++columna) {}

    if(columna != 4){
        cout << columna << endl;
        return false;
    }

    return true;
}

bool isValidDROP(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;
    int columna = 0;

    for (columna = 0; std::getline(registro, dato, ' '); ++columna) {
        if(columna == 1 && dato != "COLLECTION")
            return false;
    }

    if(columna != 3){
        return false;
    }

    return true;
}

bool isValidCREATE(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;
    int columna = 0;

    for (columna = 0; std::getline(registro, dato, ' '); ++columna) {
        if(columna == 1 && dato != "COLLECTION")
            return false;
    }

    if(columna < 5){
        return false;
    }

    return true;
}

bool isValidSentence(string sentencia){

    std::stringstream registro(sentencia);
    std::string dato;

    std::getline(registro, dato, ' ');

    if(dato == "ADD") { return isValidADD(sentencia); }
    if(dato == "REMOVE") { return isValidREMOVEorFIND(sentencia); }
    if(dato == "DROP") { return isValidDROP(sentencia); }
    if(dato == "CREATE") { return isValidCREATE(sentencia); }
    if(dato == "FIND") { return isValidREMOVEorFIND(sentencia); }

    return true;
}
    
int main(int argc, char *argv[]){
    
    string accion;

    cout << "INGRESE COMANDO: ";
    getline(cin, accion);

    while(accion != "quit"){

        if(isValidSentence(accion)){

            //INICIANDO LA CONEXION DE FIFO
            char respuesta[1000];

            int fifoClienteServidor = open("/tmp/clienteServidor", O_WRONLY);
            write(fifoClienteServidor,accion.c_str(),strlen(accion.c_str())+1);
            close(fifoClienteServidor);

            fifoClienteServidor = open("/tmp/clienteServidor", O_RDONLY);
            read(fifoClienteServidor,respuesta,sizeof(respuesta));
            close(fifoClienteServidor);

            cout << "Mensaje recibido del SERVER: " << respuesta << endl;
            //FIN DE LA CONEXION

        } else {
            cout << "Sintaxis incorrecta" << endl;
        }

        cout << "INGRESE COMANDO: ";
        getline(cin, accion);
    }

    return EXIT_SUCCESS;
}