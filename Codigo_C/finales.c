//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>	//Manejo de Entrada y Salida
#include <stdlib.h> //Manejo de comandos de sistema Windows
#include <string.h> //Manejo de Candenas de Texto
#include <windows.h> //Para el manejo de la ventana de consola
//#############################################################################################################################################
#include "../Codigo_H/finales.h"
#include "../Codigo_H/archivos.h"
#include "../Codigo_H/materias.h"
#include "../Codigo_H/otros.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
void Actualizar_Y_Mostrar_Finales(FILE *Archivo_Estado_Carrera)
{

	STR_Estado_Carrera VAR_Estado_Carrerra;

	int Cantidad_Finales = 0;

	setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("\n\nCargando: Porfavor espere!\n\n");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	system("cls");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN);
	printf("-----------------------------------------------------------\n");
	printf("%-8s| %-48s|\n", " Codigo", "                    Nombre");

	rewind(Archivo_Estado_Carrera);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrerra, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Actualizar_Y_Mostrar_Finales");
	while(!feof(Archivo_Estado_Carrera))
	{

		if(strcmp(VAR_Estado_Carrerra.Condicion, "Cursada") == 0)
		{

			Cantidad_Finales ++;
			printf("-----------------------------------------------------------\n");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("  %-6d| %-48s", VAR_Estado_Carrerra.Codigo_De_Materia, VAR_Estado_Carrerra.Nombre_De_Materia);
			printf("|\n");

		}

		Error_Lectura_Escritura(fread(&VAR_Estado_Carrerra, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Actualizar_Y_Mostrar_Finales");

	}

	if(!Cantidad_Finales)
	{
		printf("-----------------------------------------------------------\n");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("        %-50s|\n", "No hay ninguna materia pendiente de final!");

	}

	printf("-----------------------------------------------------------\n\n");
	system("pause");

}