#Script que sirve para automatizar las compilaciones.
    #Una manera de compilar es primero crear el objeto: "g++ -c holamundo.cpp"
    #Luego creamos el ejecutable: "g++ -o holamundo.exe holamundo.o" (LINKEDICION)
    #Para hacerlo en un paso solo "g++ -o holamundo holamundo.cpp"
#Creamos las etiquetas que se compilaran a futuro.
all: interfaz servidor

interfaz:
	rm -f interfaz
	g++ -o interfaz interfaz.cpp

servidor:
	rm -f gdc
	g++ -o gdc gdc.cpp

clear:
	rm -f interfaz
	rm -f gdc