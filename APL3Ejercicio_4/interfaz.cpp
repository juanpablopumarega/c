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

using namespace std;


typedef struct {
	string nombreCampo;
    string valor;
} claveValor;

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

bool validarCampos(string schema, list<claveValor> registro) {

        ifstream MyReadFile(schema);
        string cadena;
        claveValor estructura;
        list<claveValor> schemaDeTabla;
        list<claveValor>::iterator iteradorDeSchema;
        list<claveValor>::iterator iteradorDeRegistro;
        

        while(getline(MyReadFile,cadena)){
          estructura.nombreCampo=cadena;
          schemaDeTabla.push_back(estructura);
        }

        iteradorDeRegistro=registro.begin();

        while( iteradorDeRegistro != registro.end()) {
        
        int flagCampoInexistente=0; 

            iteradorDeSchema=schemaDeTabla.begin();
        
            while( iteradorDeSchema != schemaDeTabla.end()) {
                
                if(iteradorDeRegistro->nombreCampo == iteradorDeSchema->nombreCampo) {
                    flagCampoInexistente=1;
                }
        
                iteradorDeSchema++;
            }

        if(!flagCampoInexistente){
            return false;
        }

            iteradorDeRegistro++;
        }

    return true;

}

list<claveValor> ordenarRegistro(string schema, list<claveValor> registro) {

        ifstream MyReadFile(schema);
        string cadena;
        claveValor estructura;
        list<claveValor> schemaDeTabla;
        list<claveValor>::iterator iteradorDeSchema;
        list<claveValor>::iterator iteradorDeRegistro;
        int flagCampoInexistente=0;

        while(getline(MyReadFile,cadena)){
          estructura.nombreCampo=cadena;
          schemaDeTabla.push_back(estructura);
        }

        iteradorDeSchema=schemaDeTabla.begin();
        
        while( iteradorDeSchema != schemaDeTabla.end()) {
        
            iteradorDeRegistro=registro.begin();
        
            while( iteradorDeRegistro != registro.end()) {
                
                if(iteradorDeRegistro->nombreCampo == iteradorDeSchema->nombreCampo) {
                    iteradorDeSchema->valor = iteradorDeRegistro->valor;
                }
        
                iteradorDeRegistro++;
            }
        
            iteradorDeSchema++;
        }

    return schemaDeTabla;

}

string agregarRegistroATabla(string tableName, string registroAInsertar, string primaryKey) {
    
    ifstream lectura;
    lectura.open(tableName,ios::in);

    if(lectura.fail()){
        return "ERROR: No se pudo acceder a la tabla " + tableName + ". Intentelo nuevamente";
    }

    for (std::string linea; std::getline(lectura, linea); )
    {
        std::stringstream registro(linea);
        std::string dato;

        for (int columna = 0; std::getline(registro, dato, ';'); ++columna)
        {
            if(columna == 0 && dato == primaryKey) {
                lectura.close();
                return "ERROR: Primary Key duplicada: " + primaryKey + ", no se pudo insertar.";
            }
        }  
    }

    lectura.close();

    ofstream escritura;
    escritura.open(tableName.c_str(),ios::app);
    escritura<<registroAInsertar<<endl;
    escritura.close();

    return "Registro insertado correctamente";
}


string add(string tableName, list<claveValor> registro) {

    string schema = "./schemas/" + tableName + ".schema";
    string tabla = "./tablas/" + tableName + ".dat";
    list<claveValor>::iterator iteradorDeSchema;

    if(!validarCampos(schema, registro)) {
        return "ERROR: Campos invalidos, por favor revisar";
    }

    list<claveValor> listaOrdenada = ordenarRegistro(schema, registro);

    iteradorDeSchema=listaOrdenada.begin();
        
    int n=1;
    int flag = 1;
    string clavePrimaria;

    string registroAInsertar;

    while( iteradorDeSchema != listaOrdenada.end())  {
        registroAInsertar  += iteradorDeSchema->valor;

        if(flag) {
            clavePrimaria = iteradorDeSchema->valor;
            flag = 0;
        }

        if(listaOrdenada.size() != n) {
            registroAInsertar  += ";";
        }

        iteradorDeSchema++;
        n++;
    }

    return agregarRegistroATabla(tabla,registroAInsertar,clavePrimaria);

}



string create(string tableName, list<string> campos) {
    string schema = "./schemas/" + tableName + ".schema";
    string tabla = "./tablas/" + tableName + ".dat";
    ofstream flSchema;
    ofstream flTable;
    list<string>::iterator iterador;
    
    if (ifstream(schema)) {
        return "ERROR: Coleccion existente";
    }
    
    flSchema.open(schema.c_str(),ios::out);
    iterador=campos.begin();

    while( iterador != campos.end())  {
        flSchema << *iterador <<endl;
        iterador++;
    }

    flSchema.close();
    flTable.open(tabla.c_str(),ios::out);
    flTable.close();

    return "Se creo la tabla "+ tableName;
}


int main(int argc, char *argv[]){

      
    //CREACION DE LA TABLA PERSONA
    list<string> tabla;
    tabla.push_back("id");
    tabla.push_back("nombre");
    tabla.push_back("apellido");
    cout << create("persona",tabla) << endl;

    //INSERTAR REGISTRO 1
    claveValor hashmap;
    list<claveValor> registro;
    hashmap.nombreCampo="nombre";
    hashmap.valor="pepe";
    registro.push_back(hashmap);
    hashmap.nombreCampo="id";
    hashmap.valor="5";
    registro.push_back(hashmap);
    hashmap.nombreCampo="apellido";
    hashmap.valor="pig";
    registro.push_back(hashmap);    
    cout << add("persona",registro) << endl;
    
    //INSERTAR REGISTRO 2
    list<claveValor> registro2;
    hashmap.nombreCampo="nombre";
    hashmap.valor="pepa";
    registro2.push_back(hashmap);
    hashmap.nombreCampo="id";
    hashmap.valor="6";
    registro2.push_back(hashmap);
    hashmap.nombreCampo="apellido";
    hashmap.valor="pig";
    registro2.push_back(hashmap);    
    cout << add("persona",registro2) << endl;

    //REGISTRO DUPLICADO
    cout << add("persona",registro2) << endl;

    sleep(30);

    //FIND DE REGISTRO
    cout << find("persona","id","5") << endl;

    //FIND DE REGISTRO QUE NO EXISTE EL VALOR
    cout << find("persona","id","10") << endl;

    //FIND DE REGISTRO QUE NO EXISTE EL CAMPO
    cout << find("persona","otro","10") << endl;

    //REMOVE REGISTRO POR CAMPO IGUAL
    cout << remove("persona","apellido","pig") << endl;

    sleep(30);

    //DROP DE TABLA PERSONA
    cout << drop("persona") << endl;
    
    return EXIT_SUCCESS;
}

//Fin
