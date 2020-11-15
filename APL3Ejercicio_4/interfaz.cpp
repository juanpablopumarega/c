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

using namespace std;

void ayuda(){
    //cout << "-------------------------------------------------------" << endl;
    //cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..." << endl;
    //cout << "\t- Nombre Script:     ./APL3Ejercicio1.sh " << endl;
    //cout << "\t- Ejemplo de uso:    ./APL1Ejercicio2.cpp 3" << endl;
    //cout << "\t- N - [Required]     Numero entero entre 1 y 5 que indicará el nivel del arbol a generar" << endl;
    //cout << "\t- Fin de la ayuda... espero te sirva!" << endl;
    //cout << "-------------------------------------------------------" << endl;
}

string buscarEnTabla(const char * tableName, int nroColumna, string datoABuscar) {
    
    ifstream lectura;
    lectura.open(tableName,ios::in);

    for (std::string linea; std::getline(lectura, linea); )
    {
        std::stringstream registro(linea);
        std::string dato;

        for (int columna = 0; std::getline(registro, dato, ';'); ++columna)
        {
            if(columna == nroColumna && dato == datoABuscar) {
                return linea;
            }
        }
    }

    return "ERROR: Valor " + datoABuscar + " no encontrado en la tabla";
}

int obtenerNroColumna(string tableName, string nombreCampo) {
        
        ifstream MyReadFile(tableName);
        string cadena;
        int n=0;

        while(getline(MyReadFile,cadena)){
          if(cadena == nombreCampo) {
              return n;
          }
          n++;
        }

        return -1;
}

string find(string tableName, string columnName, string value) {

    string schema = "./schemas/" + tableName + ".schema";
    //string tabla = "./tablas/" + tableName + ".dat";
    int nroColumna = obtenerNroColumna(schema, columnName);

    if(nroColumna < 0)
        return "ERROR: Columna " + columnName + " no encontrada en la tabla";

    return buscarEnTabla("./tablas/producto.dat", nroColumna, value);

}



int main(int argc, char *argv[]){

    //int nroColumna = obtenerNroColumna("./schemas/producto.schema", "precio");
    //string resultado = buscarEnTabla("./tablas/producto.dat",nroColumna,"400");

    string resultado = find("producto","nombre_producto2","producto4");
    cout << resultado << endl;

    return EXIT_SUCCESS;
}

//Fin
