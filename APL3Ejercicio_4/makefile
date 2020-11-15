#Script que sirve para automatizar las compilaciones.
    #Una manera de compilar es primero crear el objeto: "g++ -c holamundo.cpp"
    #Luego creamos el ejecutable: "g++ -o holamundo.exe holamundo.o" (LINKEDICION)
    #Para hacerlo en un paso solo "g++ -o holamundo holamundo.cpp"
#Creamos las etiquetas que se compilaran a futuro.
all: interfaz servidor

interfaz:
	g++ -o interfaz interfaz.cpp;

servidor:
	g++ -o gdc gdc.cpp;