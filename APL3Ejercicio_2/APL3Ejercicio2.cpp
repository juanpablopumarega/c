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

void funcThread1()
{
    time_t start = time(0);
    time_t end;
    char* dt = ctime(&start);
    int vocales=0,consonantes=0,otrochar=0,i=0;
    string cadena;
    ifstream MyReadFile("/home/andres/Documentos/GitHub/c/APL3Ejercicio_2/files/ejemplo.txt");
    
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
    
    cout << "Hora de inicio: " << dt << endl;
    cout << "Numero de thread: "<< getpid() <<endl;
    cout <<"vocales: " << vocales << endl;
    cout <<"consonantes: " << consonantes << endl;
    cout <<"otra cosa: " << otrochar << endl;
    sleep(5);
    end = time(0);
    char* fin = ctime(&end);
    cout << "Hora de fin: " << fin << endl;
    
}



int main()
{
    cout << "Proceso padre: " << getpid() << endl;
    thread th1(funcThread1);    
    th1.join();
    return EXIT_SUCCESS;
}
