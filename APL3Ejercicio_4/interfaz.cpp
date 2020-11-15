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

string buscarEnTabla(string tableName, int nroColumna, string datoABuscar) {
    
    ifstream lectura;
    lectura.open(tableName,ios::in);

    if(lectura.fail()){
        return "ERROR: No se pudo acceder a la tabla " + (string)tableName + ". Intentelo nuevamente";
    }

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

    lectura.close();

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
    string tabla = "./tablas/" + tableName + ".dat";
    int nroColumna = obtenerNroColumna(schema, columnName);

    if(nroColumna < 0)
        return "ERROR: Columna " + columnName + " no encontrada en la tabla";

    return buscarEnTabla(tabla, nroColumna, value);

}

//https://www.youtube.com/watch?v=CcP2_7yubV8
string eliminarRegistroPorclave(string tableName, int nroColumna, string datoABuscar) {
    
    ifstream lectura;
    lectura.open(tableName,ios::in);

    string tablaTemporal = tableName + ".tmp";

    ofstream temporal;
    temporal.open(tablaTemporal.c_str(),ios::out);

    if(lectura.fail()){
        return "ERROR: No se pudo acceder a la tabla " + tableName + ". Intentelo nuevamente";
    }

    for (std::string linea; std::getline(lectura, linea); )
    {
        std::stringstream registro(linea);
        std::string dato;

        int flagDeEscritura = 1;

        for (int columna = 0; std::getline(registro, dato, ';'); ++columna)
        {
            if(columna == nroColumna && dato == datoABuscar) {
               flagDeEscritura=0;
            }
        }

        if(flagDeEscritura) {
            temporal<<linea<<endl;
        }
            
    }

    lectura.close();
    temporal.close();

    remove(tableName.c_str());
    rename(tablaTemporal.c_str(),tableName.c_str());

    return "Registro eliminado de la tabla correctamente..";
}

string remove(string tableName, string columnName, string value) {

    string schema = "./schemas/" + tableName + ".schema";
    string tabla = "./tablas/" + tableName + ".dat";

    int nroColumna = obtenerNroColumna(schema, columnName);

    if(nroColumna < 0)
        return "ERROR: Columna " + columnName + " no encontrada en la tabla";

    return eliminarRegistroPorclave(tabla, nroColumna, value);

}

string drop(string tableName) {

    string schema = "./schemas/" + tableName + ".schema";
    string tabla = "./tablas/" + tableName + ".dat";

    if(!remove(tabla.c_str()) && !remove(schema.c_str())) {
        return "La tabla " + tabla + " removida exitosamente.";
    } else {
        return "La tabla " + tabla + " NO se ha podido remover exitosamente.";
    }

}


int main(int argc, char *argv[]){

    cout << find("producto","nombre_producto","producto3") << endl;
    cout << "Removiendo registro..." << endl;
    cout << remove("producto","nombre_producto","producto3") << endl;
    cout << find("producto","nombre_producto","producto3") << endl;
    //cout << "DROP de la tabla.." << endl;
    //cout << drop("producto") << endl;

    return EXIT_SUCCESS;
}

//Fin
