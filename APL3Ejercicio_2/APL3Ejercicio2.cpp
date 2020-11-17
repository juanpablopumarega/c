//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        2                                 
// Entrega N°:       1                                 
// Nombre Script:    APL3Ejercicio2.cpp             
// Grupo 2                                             
// Juarez Miguel                         DNI:38530113
// Grassi Jonatan                        DNI:40077893
// Lopez Pumarega Juan Pablo             DNI:34593023
// Miranda Andres                        DNI:32972232
// Salerti Natalia                       DNI:41559796
//------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <ctype.h>
#include <thread>
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <list>
#include <mutex>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

using namespace std;

mutex m1;

typedef struct 
{
	int cantMenor;
    string fileNameCantMenor;
	int cantMayor;
    string fileNameCantMayor;
	int bandera;
    string primerFile;
    string ultimoFile;
    int cantidadFiles;
}tMemoriaCompartida;

bool is_dir(string dir)
{
    DIR * directorio;
    if(directorio=opendir(dir.c_str())) {
        closedir(directorio);
        return true;
    }
    else {
        return false;
    }
}

bool is_file(string file)
{
    FILE * archivo;
    if(archivo=fopen(file.c_str(),"r")) {
        fclose(archivo);
        return true;
    }
    else {
        return false;
    }
}

int esVocal(char letra) {
    
    char letraEnMayuscula = (char) toupper(letra);
    return letraEnMayuscula == 'A' ||
           letraEnMayuscula == 'E' ||
           letraEnMayuscula == 'I' ||
           letraEnMayuscula == 'O' ||
           letraEnMayuscula == 'U';
}

void funcThread1(list<string> lista, char *dirEntrada, char *dirSalida, int nroThread, tMemoriaCompartida* memoriaCompartida)
{
    
    time_t start = time(0);
    time_t end;
    char* dt = ctime(&start);
    list<string>::iterator iterador;

    //Comenzamos a iterar la lista de archivos que analizará este thread.
    iterador=lista.begin();

    while( iterador != lista.end())  {

        string archivo = '/'+*iterador;
        string entrada = dirEntrada + archivo;
        string salida = dirSalida + archivo;
        ofstream MyWriteFile;
        int vocales=0,consonantes=0,otrochar=0,i=0;
        string cadena;
        ifstream MyReadFile(entrada);

        //Hago las operaciones sobre el file
        while(getline(MyReadFile,cadena)){
            while(cadena[i]){
                if (esVocal(cadena[i])) {
                    vocales++;
                }
                else {
                    if(!isalpha(cadena[i]) && cadena[i] != '\n' && cadena[i] != ' ' && cadena[i] != '\t') {
                        otrochar++;
                    }
                    else {
                        consonantes++;
                    }
                }
                i++;
            }
            i=0;
        }

        MyWriteFile.open(salida);
        MyWriteFile << "Hora de inicio: " << dt;
        MyWriteFile << "Numero de thread: "<< nroThread+1 <<endl;
        MyWriteFile << "Cant Vocales: " << vocales << endl;
        MyWriteFile << "Cant Consonantes: " << consonantes << endl;
        MyWriteFile << "Cant de otros caracteres: " << otrochar << endl;

        end = time(0);
        char* fin = ctime(&end);
        MyWriteFile << "Hora de finalización: " << fin << endl;

        //Entramos a la zona critica y guardamos los datos del archivo si cumple con enunciado
        m1.lock();

        int cantidadTotalDeCaracteres = vocales+consonantes+otrochar;

        //Si es la primera vez que entro inicializo los vectores
        if(memoriaCompartida->bandera==1){
            memoriaCompartida->cantMayor=cantidadTotalDeCaracteres;
            memoriaCompartida->cantMenor=cantidadTotalDeCaracteres;
            memoriaCompartida->fileNameCantMayor=entrada;
            memoriaCompartida->fileNameCantMenor=entrada;
            memoriaCompartida->bandera=0;
            memoriaCompartida->primerFile=entrada;
            memoriaCompartida->ultimoFile=entrada;
        }
        
        if(memoriaCompartida->cantMayor<cantidadTotalDeCaracteres){
            memoriaCompartida->cantMayor=cantidadTotalDeCaracteres;
            memoriaCompartida->fileNameCantMayor=entrada;
        }

        if(memoriaCompartida->cantMenor>cantidadTotalDeCaracteres){
            memoriaCompartida->cantMenor=cantidadTotalDeCaracteres;
            memoriaCompartida->fileNameCantMenor=entrada;
        }

        memoriaCompartida->cantidadFiles--;

        if(memoriaCompartida->cantidadFiles==0) {
            memoriaCompartida->ultimoFile=entrada;
        }

        m1.unlock();
        //Salimos de la zona critica

        iterador++;
    }
}

void ayuda(){
    cout << "-----------------------------------------------------------------------------------"           << endl;
    cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..."                                                   << endl;
    cout << "\t- Descripcion - Script para contar letras dentro de todos los archivosd en directorio con una paralelización especificada por parametro"                                                   << endl;
    cout << "\t- Nombre Script:     ./APL3Ejercicio2 "                                                      << endl;
    cout << "\t- Ejemplo de uso:    ./APL3Ejercicio2 3 './files/entrada' './files/salida'"                  << endl;
    cout << "\t- Parametros: "                  << endl;
    cout << "\t\t- N - [Required]     Numero entero mayor que cero que indique el paralelismo a realizarse. "  << endl;
    cout << "\t\t-                    En caso de que el paralelismo sea mayor al numero de files a analizar, quedará asignado como el nro de files" << endl;
    cout << "\t\t- Path entrada - [Required]     Path absoluto o relativo del directorio a analizar"  << endl;
    cout << "\t\t- Path salida  - [Required]     Path absoluto o relativo del directorio de salida"  << endl;
    cout << "\t- Fin de la ayuda... espero te sirva!"                                                       << endl;
    cout << "------------------------------------------------------------------------------------"          << endl;
}


int main(int argc, char *argv[])
{
    //Iniciamos la validación de Parametros
    if(argc!=4) {
        cout << "Cantidad de parametros invalida." << endl;
        cout << "Llamando a la ayuda..." << endl;
        ayuda();
        return EXIT_SUCCESS;
    } else {
        if(strcmp(argv[1],"-help")==0 || strcmp(argv[1],"-h")==0){
            ayuda();
            return EXIT_SUCCESS;
        } else {
            if(atoi(argv[1])<1) {
                cout << "Se espera un numero natural mayor a 0." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
            if(!is_dir(argv[2])) {
                cout << argv[2] <<": No es un directorio de entrada valido." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
            if(!is_dir(argv[3])) {
                cout << argv[3] <<": No es un directorio de salida valido." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
        }
    }
    //Fin de la validación de parametros.
    
    //Declaración de variables utilizadas.
    int paralelismo=atoi(argv[1]), n=0, r=0, resto=0;
    char * dirEntrada=argv[2];
    char * dirSalida=argv[3];
    struct dirent **namelist;
    int archivosXThread=0;
    list<string>::iterator iterador;
    std::thread myThreads[paralelismo];

    //funciones para memoria compartida
    int fd = shm_open("memoriaCompartida", O_CREAT | O_RDWR, 0600);

    ftruncate(fd, sizeof(tMemoriaCompartida));

    tMemoriaCompartida *memoriaCompartida = (tMemoriaCompartida *)mmap(NULL, 
                                            sizeof(tMemoriaCompartida), 
                                            PROT_READ | PROT_WRITE, 
                                            MAP_SHARED, 
                                            fd, 
                                            0);

    close(fd);

    memoriaCompartida->bandera=1;

    n = scandir(dirEntrada, &namelist, 0, alphasort);
    memoriaCompartida->cantidadFiles = n-2;

    if((n-2)<paralelismo) {
        paralelismo=(n-2);
        archivosXThread=1;
    } else {
        resto = (n-2)%paralelismo;
        archivosXThread=(n-2)/paralelismo;
    }

    //Itero por cantidad de threads
    for( int i = 0; i < paralelismo; i++ ) {
        
        list<string> lista;

        if(resto>0) {
            r=1;
            resto--;
        } else {
            r=0;
        }
        
        //Itero por cantidad de archivos existentes y evito el "." y ".."
        for ( int j = 0; j < archivosXThread + r; j++ ){
            if(n-- && strcmp(namelist[n]->d_name,".")!=0 && strcmp(namelist[n]->d_name,"..")!=0){
                lista.push_back(namelist[n]->d_name);
            }
            
        }

        //Imprimo La información adicional solicitada por pantalla
        cout << "Archivos Analizados por el Thread Numero: " << i+1 << endl;
        
        iterador=lista.begin();
        while( iterador != lista.end())  {
            cout << *iterador << endl;    
            iterador++;
        }
        cout << endl;
        
        //Genero el Thread y le paso los parametros necesarios
        myThreads[i] = std::thread(funcThread1, 
                                    lista, 
                                    dirEntrada, 
                                    dirSalida, 
                                    i, 
                                    memoriaCompartida);
    }

    //Itero para ir cerrando todos los joins
    for (int i=0; i < paralelismo ; i++){
        myThreads[i].join();
    }

    cout << "Menor cantidad de caracteres (" << memoriaCompartida->cantMenor << ")" << "File: " << memoriaCompartida->fileNameCantMenor << endl;
    cout << "Mayor cantidad de caracteres (" << memoriaCompartida->cantMayor << ")" << "File: " << memoriaCompartida->fileNameCantMayor << endl;
    cout << "Primer archivo finalizado: " << memoriaCompartida->primerFile << endl;
    cout << "Último archivo finalizado: " << memoriaCompartida->ultimoFile << endl;   

    //Libero la memoria tomada
    munmap(memoriaCompartida, sizeof(tMemoriaCompartida));
    shm_unlink("memoriaCompartida");

    return EXIT_SUCCESS;
}

//FIN