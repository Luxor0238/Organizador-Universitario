//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>	//Manejo de Entrada y Salida
#include <stdlib.h> //Manejo de comandos de sistema Windows
#include <windows.h> //Para el manejo de la ventana de consola
//#############################################################################################################################################
#include "../Codigo_H/otros.h"
#include "../Codigo_H/archivos.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
FILE *Abrir_Archivo(char Ruta[], char Modo[], int Salir)
{

	FILE *Archivo; //Puntero a FILE para abrir un archivo

	Archivo = fopen(Ruta, Modo); //Abre un archivo

	if(Archivo == NULL && Salir) //Si no se pudo abrir el archivo
	{

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\nNo se pudo abrir el archivo: %s\n", Ruta); //Indica que archivo no se pudo abrir
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		perror("Error"); //Indica el porque no se pudo abrir
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		exit(1);

	}

	return Archivo;

}
//#############################################################################################################################################
void Cerrar_Archivo(FILE **Archivo)
{

	if(*Archivo != NULL)
	{

		fclose(*Archivo);
		*Archivo = NULL;

	}

}
//#############################################################################################################################################
void Error_Lectura_Escritura(int Error, char Funcion[])
{

	if(Error < 0)
	{

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nSe produjo un error al intentar leer o escribir en el buffer!\n");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("El error se ocaciono en la funcion: %s\n\n\n", Funcion);
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		exit(2);

	}

}