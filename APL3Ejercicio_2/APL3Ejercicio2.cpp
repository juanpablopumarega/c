//------------------------------------------------------------------------------------------------------------------
// APL:              3                                 
// Ejercicio:        2                                 
// Entrega N°:       1                                 
// Nombre Script:    APL3Ejercicio2.cpp                 
// Ejemplo de uso:   ./APL1Ejercicio2.cpp 3             
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
#include <ctype.h>
#include <thread>
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <list>

using namespace std;

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

void funcThread1(list<string> lista, char * dirEntrada, char * dirSalida, int nroThread)
{
    
    time_t start = time(0);
    time_t end;
    char* dt = ctime(&start);
    list<string>::iterator iterador;

    cout << "comienzo a iterar la lista dentro " << endl;
    iterador=lista.begin();

    while( iterador != lista.end())  {

        string entrada = dirEntrada + *iterador;
        string salida = dirSalida + *iterador;
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
                    if(!isalpha(cadena[i])) {
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
        MyWriteFile << "Numero de thread: "<< nroThread <<endl;
        MyWriteFile << "vocales: " << vocales << endl;
        MyWriteFile << "consonantes: " << consonantes << endl;
        MyWriteFile << "otra cosa: " << otrochar << endl;
        end = time(0);
        char* fin = ctime(&end);
        MyWriteFile << "Hora de fin: " << fin << endl;

        iterador++;
    }

    cout << "fin de la iteración de la lista" << endl;
    //BORRAR ANTES DE ENTREGAR ES SOLO DE REFERENCIA

}

void ayuda(){
    cout << "-------------------------------------------------------"                                       << endl;
    cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..."                                                   << endl;
    cout << "\t- Nombre Script:     ./APL3Ejercicio1.sh "                                                   << endl;
    cout << "\t- Ejemplo de uso:    ./APL1Ejercicio2.cpp 3 '/home/jp/c/APL3Ejercicio_2/files/entrada' '/home/jp/c/APL3Ejercicio_2/files/salida'"                                                 << endl;
    cout << "\t- N - [Required]     Numero entero entre 1 y 5 que indicará el nivel del arbol a generar"    << endl;
    cout << "\t- Fin de la ayuda... espero te sirva!"                                                       << endl;
    cout << "-------------------------------------------------------"                                       << endl;
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
            if(atoi(argv[1])<1 || atoi(argv[1])>20) {
                cout << "Se espera un numero natural menos o igual a 5." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
        }
    }
    //Fin de la validación de parametros.


    int paralelismo=atoi(argv[1]), n=0, i=0, x=0, y=0, cantidadFiles=0, r=0;
    char * dirEntrada=argv[2];
    char * dirSalida=argv[3];
    struct dirent **namelist;
    int archivosXThread=0;
    list<string>::iterator iterador;
    thread th1;

    cantidadFiles = n = scandir(dirEntrada, &namelist, 0, alphasort);
    int resto = (n-2)%paralelismo;
    cout << "resto: " << resto << endl;
    archivosXThread=(n-2)/paralelismo;

    cout << "La cantidad de archivos es: " << n << endl << "El nivel de paralelismo es: " << paralelismo << endl << "Archivos por Thread: " << archivosXThread << endl;
    cout << "Proceso padre: " << getpid() << endl;

    char *archivoXthread1[paralelismo][archivosXThread];
    int h=0;

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

        //BORRAR ANTES DE ENTREGAR ES SOLO DE REFERENCIA
        cout << "comienzo a iterar la lista" << endl;
        iterador=lista.begin();
        while( iterador != lista.end())  {
            cout << *iterador << endl;    
            iterador++;
        }
        cout << "fin de la iteración de la lista" << endl;
        //BORRAR ANTES DE ENTREGAR ES SOLO DE REFERENCIA

        thread th1(funcThread1, lista, dirEntrada, dirSalida, i+1);
        th1.join();

    }

   //for( int i = 0; i < paralelismo; i++ ) {
   //    th1.join();
   //}


    return EXIT_SUCCESS;
}
