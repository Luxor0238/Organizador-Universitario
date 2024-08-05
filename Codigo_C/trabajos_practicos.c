//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
//#############################################################################################################################################
#include "../Codigo_H/trabajos_practicos.h"
#include "../Codigo_H/archivos.h"
#include "../Codigo_H/otros.h"
#include "../Codigo_H/materias.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
void Nuevo_Trabajo_Practico(FILE *Archivo_Trabajos_Practicos, FILE *Archivo_Materias, int Codigo_De_Materia_Ingresado)
{

	STR_Trabajos VAR_Trabajos;
	STR_Materias VAR_Materias;
	STR_Fecha VAR_Fecha;

	char Nombre_Trabajo_Practico[50];
	int Es_Grupal;

	int Fecha_Valida;
	int Numero_Aleatorio;

	srand(time(NULL));

	Numero_Aleatorio = rand() % 1000;
	rewind(Archivo_Trabajos_Practicos);
	Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Nuevo_Trabajo_Practico");
	while(!feof(Archivo_Trabajos_Practicos))
	{

		if(VAR_Trabajos.Codigo_De_Trabajo == Numero_Aleatorio)
		{

			Numero_Aleatorio = rand() % 1000;
			rewind(Archivo_Trabajos_Practicos);

		}

		Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Nuevo_Trabajo_Practico");

	}
	VAR_Trabajos.Codigo_De_Trabajo = Numero_Aleatorio;

	VAR_Trabajos.Codigo_De_Materia = Codigo_De_Materia_Ingresado;

	rewind(Archivo_Materias);
	Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Nuevo_Trabajo_Practico");
	while(!feof(Archivo_Materias))
	{

		if(VAR_Materias.Codigo_De_Materia == Codigo_De_Materia_Ingresado)
			strcpy(VAR_Trabajos.Nombre_De_Materia, VAR_Materias.Nombre_De_Materia);

		Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Nuevo_Trabajo_Practico");

	}

	printf("Ingrese el nombre del Trabajo Practico: ");
	fflush(stdin);
	fgets(Nombre_Trabajo_Practico, sizeof(Nombre_Trabajo_Practico), stdin);
	Leer_Texto(Nombre_Trabajo_Practico);
	strcpy(VAR_Trabajos.Nombre_De_Trabajo, Nombre_Trabajo_Practico);

	printf("\nEs grupal? 1) Si\t2) No: ");
	Es_Grupal = Ingresa_Menu_Valido(1, 2);
	while(Es_Grupal < 1 || Es_Grupal > 2)
	{

		printf("Error: Opcion Invalida!\nIntente denuevo: ");
		Es_Grupal = Ingresa_Menu_Valido(1, 2);

	}
	if(Es_Grupal == 1)
		strcpy(VAR_Trabajos.Es_Grupal, "Si");

	else
		strcpy(VAR_Trabajos.Es_Grupal, "No");

	strcpy(VAR_Trabajos.Entregado, "No");
	strcpy(VAR_Trabajos.Aprobado, "No");
	VAR_Trabajos.Nota = 0;

	printf("\nIngrese la fecha de entrega:");
	printf("\nAnio: ");
	VAR_Fecha.Anio = Ingresa_Cantidad_Recursadas_Valida();
	printf("\nMes: ");
	VAR_Fecha.Mes = Ingresa_Cantidad_Recursadas_Valida();
	printf("\nDia: ");
	VAR_Fecha.Dia = Ingresa_Cantidad_Recursadas_Valida();
	printf("\nHora: ");
	VAR_Fecha.Hora = Ingresa_Cantidad_Recursadas_Valida();
	printf("\nMinutos: ");
	VAR_Fecha.Minutos = Ingresa_Cantidad_Recursadas_Valida();
	Fecha_Valida = Ingresa_Fecha_Valida(VAR_Fecha);

	while(Fecha_Valida == -1)
	{

		printf("\nFecha Invalida! Intente denuevo:");
		printf("\nAnio: ");
		VAR_Fecha.Anio = Ingresa_Cantidad_Recursadas_Valida();
		printf("\nMes: ");
		VAR_Fecha.Mes = Ingresa_Cantidad_Recursadas_Valida();
		printf("\nDia: ");
		VAR_Fecha.Dia = Ingresa_Cantidad_Recursadas_Valida();
		printf("\nHora: ");
		VAR_Fecha.Hora = Ingresa_Cantidad_Recursadas_Valida();
		printf("\nMinutos: ");
		VAR_Fecha.Minutos = Ingresa_Cantidad_Recursadas_Valida();
		Fecha_Valida = Ingresa_Fecha_Valida(VAR_Fecha);

	}

	VAR_Trabajos.Fecha_De_Entrega.Dia = VAR_Fecha.Dia;
	VAR_Trabajos.Fecha_De_Entrega.Mes = VAR_Fecha.Mes;
	VAR_Trabajos.Fecha_De_Entrega.Anio = VAR_Fecha.Anio;
	VAR_Trabajos.Fecha_De_Entrega.Hora = VAR_Fecha.Hora;
	VAR_Trabajos.Fecha_De_Entrega.Minutos = VAR_Fecha.Minutos;
	strcpy(VAR_Trabajos.Borrado, "No");

	printf("\n\nCargando: Porfavor espere!\n\n");

	Error_Lectura_Escritura(fwrite(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Nuevo_Trabajo_Practico");
	fflush(Archivo_Trabajos_Practicos);

}
//#############################################################################################################################################
void Mostrar_Trabajos(FILE *Archivo_Trabajos_Practicos)
{

	STR_Trabajos VAR_Trabajos;

	int Cantidad_Trabajos = 0;

	char buffer_TP[3];
	
	system("cls");
	setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf(" %-18s| %-18s| %-26s| %-26s| %-7s| %-10s| %-9s| %-5s|%-18s|\n", "Codigo de Trabajo", "Codigo de Materia", "    Nombre de Materia", "    Nombre de Trabajo", "Grupal", "Entregado", "Aprobado", "Nota", " Fecha de Entrega");
	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	rewind(Archivo_Trabajos_Practicos);
	Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Mostrar_Trabajos");
	while(!feof(Archivo_Trabajos_Practicos))
	{

    	if(strcmp(VAR_Trabajos.Borrado, "No") == 0)
    	{
	    
	    	printf("        %-11d|        %-11d| %-26s| %-26s|   %-5s|    %-7s|    %-6s|  %-4s| %-2d/%-2d/%-2d %-2d:%-3d|\n", VAR_Trabajos.Codigo_De_Trabajo, VAR_Trabajos.Codigo_De_Materia, VAR_Trabajos.Nombre_De_Materia, VAR_Trabajos.Nombre_De_Trabajo, VAR_Trabajos.Es_Grupal, VAR_Trabajos.Entregado, VAR_Trabajos.Aprobado, VAR_Trabajos.Nota == 0 ? "--" : (sprintf(buffer_TP, "%d", VAR_Trabajos.Nota), buffer_TP), VAR_Trabajos.Fecha_De_Entrega.Dia, VAR_Trabajos.Fecha_De_Entrega.Mes, VAR_Trabajos.Fecha_De_Entrega.Anio, VAR_Trabajos.Fecha_De_Entrega.Hora, VAR_Trabajos.Fecha_De_Entrega.Minutos);
	    	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		}

		if(strcpy(VAR_Trabajos.Borrado, "SI") == 0)
			Cantidad_Trabajos ++;

		Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Mostrar_Trabajos");

	}

	if(!Cantidad_Trabajos)
	{

		setColor(FOREGROUND_RED | FOREGROUND_GREEN);
		printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		printf("|                                                %-104s|\n", "No tienes ningun trabajo practico cargado!!");
		printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}

	printf("\n\n");
	system("pause");

}
//#############################################################################################################################################
void Eleiminar_Trabajo_Practico(FILE *Archivo_Trabajos_Practicos)
{

	STR_Trabajos VAR_Trabajos;

	int Codigo_De_Trabajo_Ingresado;
	int Posicion_En_Bytes;
	
	Mostrar_Menu_Trabajos();
	printf("\n\nIngrese el codigo del trabajo que desea eliminar:\n-------------------------------------------------\n--> ");
	Codigo_De_Trabajo_Ingresado = Ingresa_Entero_Valido();

	Posicion_En_Bytes = Buscar_Trabajo_Practico(Archivo_Trabajos_Practicos, Codigo_De_Trabajo_Ingresado);

	while(Posicion_En_Bytes == -1)
	{

		Mostrar_Menu_Trabajos();
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nEl trabajo ingresado no existe! Intente denuevo\n---------------------------------------------------------\n--> ");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Codigo_De_Trabajo_Ingresado = Ingresa_Entero_Valido();
		Posicion_En_Bytes = Buscar_Trabajo_Practico(Archivo_Trabajos_Practicos, Codigo_De_Trabajo_Ingresado);

	}

	printf("\n\nCargando: Porfavor espere!\n\n");
	
	fseek(Archivo_Trabajos_Practicos, Posicion_En_Bytes - sizeof(STR_Trabajos), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Eleiminar_Trabajo_Practico");

	strcpy(VAR_Trabajos.Borrado, "Si");

	fseek(Archivo_Trabajos_Practicos, Posicion_En_Bytes - sizeof(STR_Trabajos), SEEK_SET);
	Error_Lectura_Escritura(fwrite(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Eleiminar_Trabajo_Practico");

	fflush(Archivo_Trabajos_Practicos);

	Mostrar_Menu_Trabajos();
	printf("\n\nBorrado con exito!\n\n");
	system("pause");

}
//#############################################################################################################################################
int Buscar_Trabajo_Practico(FILE *Archivo_Trabajos_Practicos, int Codigo_De_Trabajo_Ingresado)
{

	STR_Trabajos VAR_Trabajos;

	rewind(Archivo_Trabajos_Practicos);
	Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Buscar_Trabajo_Practico");
	while(!feof(Archivo_Trabajos_Practicos))
	{

		if(VAR_Trabajos.Codigo_De_Trabajo == Codigo_De_Trabajo_Ingresado && strcmp(VAR_Trabajos.Borrado, "No") == 0)
			return ftell(Archivo_Trabajos_Practicos);

		Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Buscar_Trabajo_Practico");

	}

	return -1;

}
//#############################################################################################################################################
void Modificar_Trabajo_Practico(FILE *Archivo_Trabajos_Practicos, STR_Trabajos VAR_Trabajos_Ingresada, int Codigo_De_Trabajo_Ingresado)
{

	STR_Trabajos VAR_Trabajos_Read;

	int Fin_De_Lectura = 0;
	int Posicion_En_Bytes;

	rewind(Archivo_Trabajos_Practicos);
	Error_Lectura_Escritura(fread(&VAR_Trabajos_Read, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Modificar_Trabajo_Practico");
	while(!feof(Archivo_Trabajos_Practicos) && !Fin_De_Lectura)
	{

		if(VAR_Trabajos_Read.Codigo_De_Trabajo == Codigo_De_Trabajo_Ingresado)
		{

			Posicion_En_Bytes = ftell(Archivo_Trabajos_Practicos);
			Fin_De_Lectura = 1;

		}

		Error_Lectura_Escritura(fread(&VAR_Trabajos_Read, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Modificar_Trabajo_Practico");

	}

	fseek(Archivo_Trabajos_Practicos, Posicion_En_Bytes - sizeof(STR_Trabajos), SEEK_SET);
	Error_Lectura_Escritura(fwrite(&VAR_Trabajos_Ingresada, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Modificar_Trabajo_Practico");
	fflush(Archivo_Trabajos_Practicos);

	printf("\n\n\n        %-11d|        %-11d| %-26s| %-26s|   %-5s|    %-7s|    %-6s|  %-4d| %-2d/%-2d/%-2d %-2d:%-3d|\n\n\n\n", VAR_Trabajos_Ingresada.Codigo_De_Trabajo, VAR_Trabajos_Ingresada.Codigo_De_Materia, VAR_Trabajos_Ingresada.Nombre_De_Materia, VAR_Trabajos_Ingresada.Nombre_De_Trabajo, VAR_Trabajos_Ingresada.Es_Grupal, VAR_Trabajos_Ingresada.Entregado, VAR_Trabajos_Ingresada.Aprobado, VAR_Trabajos_Ingresada.Nota, VAR_Trabajos_Ingresada.Fecha_De_Entrega.Dia, VAR_Trabajos_Ingresada.Fecha_De_Entrega.Mes, VAR_Trabajos_Ingresada.Fecha_De_Entrega.Anio, VAR_Trabajos_Ingresada.Fecha_De_Entrega.Hora, VAR_Trabajos_Ingresada.Fecha_De_Entrega.Minutos);
	system("pause");

}