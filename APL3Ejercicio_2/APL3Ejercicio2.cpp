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
    ofstream salida;
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
    salida.open("./files/salida/ejemplo.txt");
    salida << "Hora de inicio: " << dt;
    salida << "Numero de thread: "<< getpid() <<endl;
    salida <<"vocales: " << vocales << endl;
    salida <<"consonantes: " << consonantes << endl;
    salida <<"otra cosa: " << otrochar << endl;
    sleep(5);
    end = time(0);
    char* fin = ctime(&end);
    salida << "Hora de fin: " << fin << endl;
}

void ayuda(){
    cout << "-------------------------------------------------------" << endl;
    cout << "\t- Ayuda del Script APL3Ejercicio1.cpp ..." << endl;
    cout << "\t- Nombre Script:     ./APL3Ejercicio1.sh " << endl;
    cout << "\t- Ejemplo de uso:    ./APL1Ejercicio2.cpp 3" << endl;
    cout << "\t- N - [Required]     Numero entero entre 1 y 5 que indicará el nivel del arbol a generar" << endl;
    cout << "\t- Fin de la ayuda... espero te sirva!" << endl;
    cout << "-------------------------------------------------------" << endl;
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
            if(atoi(argv[1])<1 || atoi(argv[1])>5) {
                cout << "Se espera un numero natural menos o igual a 5." << endl;
                cout << "Llamando a la ayuda..." << endl;
                ayuda();
                return EXIT_SUCCESS;
            }
        }
    }
    //Fin de la validación de parametros.


    int paralelismo=atoi(argv[1]), n=0;
    const char * dirEntrada=argv[2];
    const char * dirSalida=argv[3];
    struct dirent **namelist;
    int archivosXThread=0;
    
    n = scandir(dirEntrada, &namelist, 0, alphasort);
    archivosXThread=n/paralelismo;
    cout << "La cantidad de archivos es: " << n << endl << "El nivel de paralelismo es: " << paralelismo << endl << "Archivos por Thread: " << archivosXThread << endl;
    cout << "Proceso padre: " << getpid() << endl;
    if(n < 0)
        perror("scandir");
    else {
        while(n--) {
        
            printf("%s\n", namelist[n]->d_name);
            
            free(namelist[n]);
        }
        
        free(namelist);
    }
    thread th1(funcThread1,dirEntrada,dirSalida);    
    th1.join();
    thread th2(funcThread1);    
    th2.join();
    return EXIT_SUCCESS;
}
