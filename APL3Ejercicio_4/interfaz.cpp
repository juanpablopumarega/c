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

    if(columna != 3){
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
    list<string> columnName;
    list<string> duplicated;

    for (columna = 0; std::getline(registro, dato, ' '); ++columna) {
        if(columna == 1 && dato != "COLLECTION")
            return false;

        if(columna>2) {
            
            if(columnName.size() > 0) {
                
                columnName.push_back(dato);
                
                //Reviso si existen campos duplicados.
                columnName.sort();
                duplicated = columnName;
                duplicated.unique();

                if(duplicated.size() != columnName.size()){
                    return false;
                }
                //Fin de la revisión de duplicados.

            } else {
                columnName.push_back(dato);
            }
        }
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

    return false;
}

void help()
{
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "Interfaz que se conectará a una Base de Datos del mismo servidor" << endl;
    cout << " - Se conectara al server a traves de FIFO." << endl;
    cout << " - Las acciones permitidas por el servidor son las siguientes:" << endl;
    cout << "\t - CREATE COLLECTION tablename campo1 campo2 [campo N]." << endl;
    cout << "\t\tCreara la tabla tablename con los campos indicados y el primero sera la PK" << endl;
    cout << "\t - DROP COLLECTION tablename" << endl;
    cout << "\t\tBorrara la tabla y el schema relacionado a la misma" << endl;
    cout << "\t - ADD IN tablename campo1=valor campo2=valor" << endl;
    cout << "\t\tAgregara un registro a la tabla siempre y cuando la PK no este duplicada o vacia" << endl;
    cout << "\t - FIND tablename columnName valorbuscado" << endl;
    cout << "\t\tDevolvera el registro de la tabla solicitado" << endl;
    cout << "\t - REMOVE tablename columnName valorAEliminar" << endl;
    cout << "\t\tBorrara un registro de la tabla segun columna y valor" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
}
    
int main(int argc, char *argv[]){
    
    if (argc != 1)
    {
        if (argc > 2) {
            cout << "La cantidad de parametros es incorrecta";
            return 1;
        }
        else if (!strcmp(argv[1],"--help") || !strcmp(argv[1], "-h")) {
            help();
            return 0;
        }
        else {
            cout << "El parametro ingresado es incorrecto";
            return 1;
        }
    }

    string accion;

    cout << "INGRESE COMANDO: ";
    getline(cin, accion);

    while(accion != "quit"){

        if(isValidSentence(accion)){

            //INICIANDO LA CONEXION DE FIFO
            char respuesta[1000];

            int fifoClienteServidor = open("/tmp/clienteServidor", 01);
            write(fifoClienteServidor,accion.c_str(),strlen(accion.c_str())+1);
            close(fifoClienteServidor);

            fifoClienteServidor = open("/tmp/clienteServidor", 00);
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