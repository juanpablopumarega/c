//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        4                                 
// Entrega N°:       1                                 
// Nombre Script:    gdc.cpp                 
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
#include <signal.h>

using namespace std;

typedef struct {
	string nombreCampo;
    string valor;
} claveValor;

void ayuda(){
    //
}

void signal_handler(int signal_num) { 
   cout << "Se recibió la señal (" << signal_num << "). Se interrumpe el proceso. \n"; 
   unlink("./fifo/clienteServidor");
   exit(signal_num);   
} 

string buscarEnTabla(string tableName, int nroColumna, string datoABuscar) {
    
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

    if(nroColumna == -1)
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
            if(iteradorDeSchema->valor == ""){
                return "ERROR: El campo " + iteradorDeSchema->nombreCampo + " (PK) no debe estar vacio.";
            }

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

string ejecutarADD(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;

    size_t found = 0;

    string tableName;
    list<claveValor> listaRegistro;
    claveValor hashmap;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {

        if(columna == 2) {
            tableName = dato; 
        }

       if(columna > 2) {
            found=dato.find("=");

            if (found != string::npos) {
                hashmap.nombreCampo=dato.substr(0, found);
                hashmap.valor=dato.substr(found+1);
                listaRegistro.push_back(hashmap);
            }
       }

    }

    return add(tableName, listaRegistro);
}

string ejecutarREMOVE(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;

    string tableName;
    string columnName;
    string value;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {

        if(columna == 1) {
            tableName = dato; 
        }

        if(columna == 2) {
            columnName = dato; 
        }

        if(columna == 3) {
            value = dato; 
        }

    }

    return remove(tableName, columnName, value);
}

string ejecutarDROP(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;
    string tableName;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {
        if(columna == 2) {
            tableName = dato; 
        }
    }

    return drop(tableName);
}

string ejecutarFIND(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;

    string tableName;
    string columnName;
    string value;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {

        if(columna == 1) {
            tableName = dato; 
        }

        if(columna == 2) {
            columnName = dato; 
        }

        if(columna == 3) {
            value = dato; 
        }

    }

    return find(tableName, columnName, value);
}

string ejecutarCREATE(string sentencia) {

    std::stringstream registro(sentencia);
    std::string dato;

    size_t found = 0;

    string tableName;
    list<string> campos;

    for (int columna = 0; std::getline(registro, dato, ' '); ++columna) {

        if(columna == 2) {
            tableName = dato; 
        }

       if(columna > 2) {
            campos.push_back(dato);  
       }

    }

    return create(tableName, campos);
}

string realizarAccion(string accion) {

    std::stringstream registro(accion);
    std::string dato;

    std::getline(registro, dato, ' ');

    if(dato == "ADD") { return ejecutarADD(accion); }
    if(dato == "REMOVE") { return ejecutarREMOVE(accion); }
    if(dato == "DROP") { return ejecutarDROP(accion); }
    if(dato == "CREATE") { return ejecutarCREATE(accion); }
    if(dato == "FIND") { return ejecutarFIND(accion); }

    return "ERROR: Ha ocurrido un error inesperado, por favor intentelo mas tarde nuevamente";
}

void help()
{
    cout << "------------------------------------------------------------------------" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "SERVIDOR de la Base de datos creada en c++" << endl;
    cout << " - Admitira un Cliente que se conectara a traves de FIFO." << endl;
    cout << " - Por cada creacion de tabla se creara un archivo .dat en ./tablas" << endl;
    cout << " - Por cada creacion de tabla se creara un archivo .schema en ./schemas con la estructura de la tabla deseada" << endl;
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
    cout << "------------------------------------------------------------------------" << endl;
    cout << "------------------------------------------------------------------------" << endl;
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

    signal(SIGUSR1,signal_handler);

    mkfifo("/tmp/clienteServidor",0666);
    
    while (1) {
        char contenido[1024];

        int fifoClienteServidor = open("/tmp/clienteServidor", O_RDONLY);
        read(fifoClienteServidor,contenido,sizeof(contenido));
        close(fifoClienteServidor);
        //cout << "Mensaje recibido del CLIENTE: " << contenido << endl;

        string respuesta = realizarAccion(string(contenido));

        fifoClienteServidor = open("/tmp/clienteServidor", O_WRONLY);
        write(fifoClienteServidor,respuesta.c_str(),strlen(respuesta.c_str())+1);
        close(fifoClienteServidor);
    }
    
    return EXIT_SUCCESS;
}

//Fin
