//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#############################################################################################################################################
#include "../Codigo_H/materias.h"
#include "../Codigo_H/parciales.h"
#include "../Codigo_H/otros.h"
#include "../Codigo_H/archivos.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
void Consultar_Y_Modificar_Parciales(FILE *Archivo_Parciales, FILE *Archivo_Estado_Carrera)
{

	STR_Parciales VAR_Parciales;

	int Seleccion_Menu;
	char BFR_Nota_1[3];
	char BFR_Nota_2[3];
	char BFR_Nota_R[3];

	Mostrar_Menu_Parciales();

	Seleccion_Menu = Ingresa_Menu_Valido(1, 9);
	while(Seleccion_Menu == -1)
	{

		Mostrar_Menu_Parciales();

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Error: Opcion Invalida! Intente denuevo:");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n--> ");
		Seleccion_Menu = Ingresa_Menu_Valido(1, 5);

	}

	switch(Seleccion_Menu)
	{

		case 1:

			system("cls");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN);
			printf("------------------------------------------------------------------------------------------------------------------\n");
			printf("%-8s| %-48s| %-15s| %-15s| %-19s|\n", " Codigo", "                    Nombre", "Nota Parcial 1", "Nota Parcial 2", "Nota Recuperatorio");
			printf("------------------------------------------------------------------------------------------------------------------\n");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			rewind(Archivo_Parciales);
			Error_Lectura_Escritura(fread(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Consultar_Y_Modificar_Parciales");
			while(!feof(Archivo_Parciales))
			{

				printf("  %-6d| %-48s|       ", VAR_Parciales.Codigo_De_Materia, VAR_Parciales.Nombre_De_Materia);

				if(VAR_Parciales.Nota_Parcial_1 >= 0 && VAR_Parciales.Nota_Parcial_1 <=3)
					setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

				else if(VAR_Parciales.Nota_Parcial_1 >= 4 && VAR_Parciales.Nota_Parcial_1 <=6)
					setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				else
					setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				printf("%-9s", VAR_Parciales.Nota_Parcial_1 == 0 ? " " : (sprintf(BFR_Nota_1, "%d", VAR_Parciales.Nota_Parcial_1), BFR_Nota_1));
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

				printf("|       ");

				if(VAR_Parciales.Nota_Parcial_2 >= 0 && VAR_Parciales.Nota_Parcial_2 <=3)
					setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

				else if(VAR_Parciales.Nota_Parcial_2 >= 4 && VAR_Parciales.Nota_Parcial_2 <=6)
					setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				else
					setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				printf("%-9s", VAR_Parciales.Nota_Parcial_2 == 0 ? " " : (sprintf(BFR_Nota_2, "%d", VAR_Parciales.Nota_Parcial_2), BFR_Nota_2));
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

				printf("|         ");

				if(VAR_Parciales.Nota_Recuperatorio >= 0 && VAR_Parciales.Nota_Recuperatorio <=3)
					setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

				else if(VAR_Parciales.Nota_Recuperatorio >= 4 && VAR_Parciales.Nota_Recuperatorio <=6)
					setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				else
					setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				printf("%-11s", VAR_Parciales.Nota_Recuperatorio == 0 ? " " : (sprintf(BFR_Nota_R, "%d", VAR_Parciales.Nota_Recuperatorio), BFR_Nota_R));
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

				printf("|\n");
				printf("------------------------------------------------------------------------------------------------------------------\n");
			
				Error_Lectura_Escritura(fread(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Consultar_Y_Modificar_Parciales");

			}

			printf("\n\n\n");
			system("pause");

		break;

		case 2:

			Modificar_Nota_Parciales(Archivo_Parciales, Archivo_Estado_Carrera, '1');

		break;

		case 3:

			Modificar_Nota_Parciales(Archivo_Parciales, Archivo_Estado_Carrera, '2');

		break;

		case 4:

			Modificar_Nota_Parciales(Archivo_Parciales, Archivo_Estado_Carrera, 'R');

		break;

	}

}
//#############################################################################################################################################
void Modificar_Nota_Parciales(FILE * Archivo_Parciales, FILE *Archivo_Estado_Carrera, char Dato_A_Modificar)
{

	STR_Parciales VAR_Parciales;

	int Codigo_De_Materia_Ingresado;
	int Valor_Ingresado;
	int Posicion_En_Bytes;
	int Materia_Disponible;

	printf("\n\nIngrese el codigo de materia que desea modificar:\n-------------------------------------------------\n--> ");
	Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();

	Posicion_En_Bytes = Buscar_Parciales(Archivo_Parciales, Codigo_De_Materia_Ingresado);
	while(Posicion_En_Bytes == -1)
	{

		Mostrar_Menu_Parciales();
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nLa materia ingresada no existe! Intente denuevo\n---------------------------------------------------------\n--> ");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();
		Posicion_En_Bytes = Buscar_Parciales(Archivo_Parciales, Codigo_De_Materia_Ingresado);

	}

	Materia_Disponible = Verificar_Disponibilidad(Archivo_Estado_Carrera, Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado));

	if(Materia_Disponible == -1)
	{

		Mostrar_Menu_Parciales();
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nLa materia ingresada esta no disponible! Intente con otra\n-----------------------------------------------\n--> ");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}
	else
	{

		printf("\nIngrese la nueva nota: ");
		Valor_Ingresado = Ingresa_Cantidad_Recursadas_Valida();

		while(Valor_Ingresado == -1)
		{

			printf("Error: Nota Invalida!\nIntente denuevo: ");
			Valor_Ingresado = Ingresa_Cantidad_Recursadas_Valida();

		}

		fseek(Archivo_Parciales, Posicion_En_Bytes - sizeof(STR_Parciales), SEEK_SET);
		Error_Lectura_Escritura(fread(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Modificar_Nota_Parciales");

		if(Dato_A_Modificar == '1')
			VAR_Parciales.Nota_Parcial_1 = Valor_Ingresado;

		else if(Dato_A_Modificar == '2')
			VAR_Parciales.Nota_Parcial_2 = Valor_Ingresado;

		else if(Dato_A_Modificar == 'R')
			VAR_Parciales.Nota_Recuperatorio = Valor_Ingresado;

		fseek(Archivo_Parciales, Posicion_En_Bytes - sizeof(STR_Parciales), SEEK_SET);
		Error_Lectura_Escritura(fwrite(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Modificar_Nota_Parciales");

		fflush(Archivo_Parciales);

	}
	
}
//#############################################################################################################################################
int Buscar_Parciales(FILE *Archivo_Parciales, int Codigo_De_Materia_Ingresado)
{

	
	STR_Parciales VAR_Parciales;

	rewind(Archivo_Parciales);

	Error_Lectura_Escritura(fread(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Buscar_Parcial");
	while(!feof(Archivo_Parciales))
	{

		if(VAR_Parciales.Codigo_De_Materia == Codigo_De_Materia_Ingresado)
			return ftell(Archivo_Parciales);

		Error_Lectura_Escritura(fread(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Buscar_Parcial");

	}

	return -1;

}