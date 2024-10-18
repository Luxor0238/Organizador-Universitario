//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>	//Manejo de Entrada y Salida
#include <stdlib.h> //Manejo de comandos de sistema Windows
#include <string.h> //Manejo de Candenas de Texto
#include <windows.h> //Para el manejo de la ventana de consola
//#############################################################################################################################################
#include "../Codigo_H/materias.h"
#include "../Codigo_H/archivos.h"
#include "../Codigo_H/parciales.h"
#include "../Codigo_H/otros.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
void Pasar_Materias_CSV_a_BIN(FILE *Archivo_Materias_CSV, FILE *Archivo_Materias)
{
    STR_Materias VAR_Materias; // Variable en la que se guarda cada registro de las materias

    int Indice_Inicializar;

    char Materias_Correlativas[256]; // Aumenta el tamaño para manejar todas las correlativas
    char Linea_CSV[256]; // Cadena de texto en la que se guarda el registro actual del CSV

    char *Token;
    int index;

    fgets(Linea_CSV, sizeof(Linea_CSV), Archivo_Materias_CSV); // Descarta los títulos del CSV

    while (fgets(Linea_CSV, sizeof(Linea_CSV), Archivo_Materias_CSV)) // Mientras que haya datos...
    {
        // Leemos cada línea del CSV y almacenamos en VAR_Materias
        sscanf(Linea_CSV, "%d;%[^;];%[^\n]", &VAR_Materias.Codigo_De_Materia, VAR_Materias.Nombre_De_Materia, Materias_Correlativas);
        // Inicializamos el arreglo de materias correlativas en VAR_Materias
        for(Indice_Inicializar = 0; Indice_Inicializar <100; Indice_Inicializar ++)
			VAR_Materias.Materias_Correlativas[Indice_Inicializar] = 0;

        // Procesamos las materias correlativas usando strtok
        Token = strtok(Materias_Correlativas, " ");
        index = 0;

        while (Token != NULL && index < 100)
        {

            VAR_Materias.Materias_Correlativas[index++] = atoi(Token);
            Token = strtok(NULL, " ");

        }

        // Escribimos VAR_Materias en el archivo binario
        Error_Lectura_Escritura(fwrite(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Pasar_Materias_CSV_a_BIN");

    }

    Cerrar_Archivo(&Archivo_Materias_CSV, "Plan.csv");

}
//#############################################################################################################################################
void Cambiar_Estado_Materias(FILE *Archivo_Estado_Carrera, FILE *Archivo_Materias, int Notificaciones, int Plan_De_Estudios_Cargado, FILE *Archivo_Trabajos_Practicos)
{

	int Codigo_De_Materia_Ingresado;
	int Posicion_En_Bytes;
	int Seleccion_Menu;
	int Materia_Disponible;

	printf("\n\nIngrese el codigo de materia que desea modificar (0 para cancelar):\n-------------------------------------------------------------------\n--> ");
	Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();

	if(Codigo_De_Materia_Ingresado != 0)
		Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);

	while(Posicion_En_Bytes == -1 && Codigo_De_Materia_Ingresado != 0)
	{

		if(Codigo_De_Materia_Ingresado == 0)
			break;

		if(Plan_De_Estudios_Cargado)
			Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado, Archivo_Trabajos_Practicos);
		else
			Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado, NULL);

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nLa materia ingresada no existe! Intente denuevo (0 para cancelar)\n-----------------------------------------------------------------\n--> ");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();
		Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);

	}

	if(Codigo_De_Materia_Ingresado != 0)
		Materia_Disponible = Verificar_Disponibilidad(Archivo_Estado_Carrera, Posicion_En_Bytes);

	while(Materia_Disponible == -1 && Codigo_De_Materia_Ingresado != 0)
	{

		if(Plan_De_Estudios_Cargado)
			Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado, Archivo_Trabajos_Practicos);
		else
			Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado, NULL);
		
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\n\n\nLa materia ingresada esta no disponible! Intente con otra (0 para cancelar)\n---------------------------------------------------------------------------\n--> ");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();

		if(Codigo_De_Materia_Ingresado == 0)
			break;

		Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);
		Materia_Disponible = Verificar_Disponibilidad(Archivo_Estado_Carrera, Posicion_En_Bytes);

	}
	
	if(Materia_Disponible && Codigo_De_Materia_Ingresado != 0)
	{

		Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);
		Mostrar_Menu_Pricipal();

		Seleccion_Menu = Ingresa_Menu_Valido(1, 4);
		while(Seleccion_Menu == -1)
		{

			Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);
			Mostrar_Menu_Pricipal();

			setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Error: Opcion Invalida!\nIntente denuevo:\n-----------------------------------------\n--> ");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			Seleccion_Menu = Ingresa_Menu_Valido(1, 4);

		}

		switch(Seleccion_Menu)
		{


			case 1:

				Modificar_Condicion(Archivo_Estado_Carrera, Posicion_En_Bytes);

			break;

			case 2:

				Modificar_Recursadas(Archivo_Estado_Carrera, Posicion_En_Bytes);

			break;

			case 3:

				Modificar_Nota(Archivo_Estado_Carrera, Posicion_En_Bytes);

			break;

		}

	}

}
//#############################################################################################################################################
void Mostrar_Materias(FILE *Archivo_Materias)
{

	STR_Materias VAR_Materias;
	
	char buffer0[7], buffer1[7], buffer2[7], buffer3[7], buffer4[7];

	system("cls");
	printf("\n\nCargando: Porfavor espere!\n\n");

	rewind(Archivo_Materias);
	Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Mostrar_Materias");

	system("cls");
	setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("------------------------------------------------------------------------------------------------\n");
	printf("%-14s|%-49s|%-30s|\n", "Codigo", "Nombre", "       Correlativas      ");
	printf("------------------------------------------------------------------------------------------------\n");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	while(!feof(Archivo_Materias))
	{


    	printf("%-14d|%-49s|%-6s%-6s%-6s%-6s%-6s|\n", VAR_Materias.Codigo_De_Materia, VAR_Materias.Nombre_De_Materia, VAR_Materias.Materias_Correlativas[0] == 0 ? " " : (sprintf(buffer0, "%d", VAR_Materias.Materias_Correlativas[0]), buffer0), VAR_Materias.Materias_Correlativas[1] == 0 ? " " : (sprintf(buffer1, "%d", VAR_Materias.Materias_Correlativas[1]), buffer1), VAR_Materias.Materias_Correlativas[2] == 0 ? " " : (sprintf(buffer2, "%d", VAR_Materias.Materias_Correlativas[2]), buffer2), VAR_Materias.Materias_Correlativas[3] == 0 ? " " : (sprintf(buffer3, "%d", VAR_Materias.Materias_Correlativas[3]), buffer3), VAR_Materias.Materias_Correlativas[4] == 0 ? " " : (sprintf(buffer4, "%d", VAR_Materias.Materias_Correlativas[4]), buffer4));
		printf("------------------------------------------------------------------------------------------------\n");
		Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Mostrar_Materias");

	}

	printf("\n\n");
	system("pause");

}
//#############################################################################################################################################
void Cargar_Archivo_Progreso_Desde_Cero(FILE *Archivo_Materias, FILE *Archivo_Estado_Carrera, FILE *Archivo_Parciales)
{

	STR_Materias VAR_Materias;
	STR_Estado_Carrera VAR_Estado_Carrera;
	STR_Parciales VAR_Parciales;

	int i;
	int Suma_Correlativas = 0;

	rewind(Archivo_Materias);
	rewind(Archivo_Estado_Carrera);

	Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Cargar_Archivo_Progreso_Desde_Cero");
	while(!feof(Archivo_Materias))
	{

		VAR_Estado_Carrera.Codigo_De_Materia = VAR_Materias.Codigo_De_Materia;
		strcpy(VAR_Estado_Carrera.Nombre_De_Materia, VAR_Materias.Nombre_De_Materia);
		strcpy(VAR_Estado_Carrera.Condicion, "Por Cursar");
		VAR_Estado_Carrera.Cantidad_De_Recursadas = 0;

		for(i=0; i<100; i++)
			Suma_Correlativas += VAR_Materias.Materias_Correlativas[i];


		if(Suma_Correlativas == 0)
			strcpy(VAR_Estado_Carrera.Disponibilidad, "Disponible");
		else
			strcpy(VAR_Estado_Carrera.Disponibilidad, "No Disponible");

		VAR_Estado_Carrera.Nota = 0;

		VAR_Parciales.Codigo_De_Materia = VAR_Materias.Codigo_De_Materia;
		strcpy(VAR_Parciales.Nombre_De_Materia, VAR_Materias.Nombre_De_Materia);
		VAR_Parciales.Nota_Parcial_1 = 0;
		VAR_Parciales.Nota_Parcial_2 = 0;
		VAR_Parciales.Nota_Recuperatorio = 0;

		Error_Lectura_Escritura(fwrite(&VAR_Parciales, sizeof(STR_Parciales), 1, Archivo_Parciales), "Cargar_Archivo_Progreso_Desde_Cero");
		Error_Lectura_Escritura(fwrite(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Cargar_Archivo_Progreso_Desde_Cero");
		Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Cargar_Archivo_Progreso_Desde_Cero");

	}

}
//#############################################################################################################################################
void Mostrar_Estado_Carrera(FILE *Archivo_Estado_Carrera)
{

	STR_Estado_Carrera VAR_Estado_Carrera;
	int Materias_Promocionadas = 0;
	int Materias_Aprobadas = 0;
	int Materias_Cursadas = 0;
	int Materias_Reprobadas = 0;
	int Materias_En_Curso = 0;
	int Materias_Por_Cursar = 0;
	float Porcentaje_Carrera = 0;
	int Total_De_Materias = 0;

	char Buffer_Nota[3];

	system("cls");
	printf("\n\nCargando: Porfavor espere!\n\n");

	rewind(Archivo_Estado_Carrera);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Mostrar_Estado_Carrera");

	system("cls");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN);
	printf("----------------------------------------------------------------------------------------------------------------------------------\n");
	printf("%-8s| %-48s| %-15s| %-23s| %-20s| %-5s|\n", " Codigo", "                    Nombre","  Condicion", "Cantidad de recursadas", "  Disponibilidad", "Nota");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN);

	while(!feof(Archivo_Estado_Carrera))
	{

		if(strcmp(VAR_Estado_Carrera.Condicion, "Promocionada") == 0)
			Materias_Promocionadas ++;

		else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursada") == 0)
			Materias_Cursadas ++;

		else if(strcmp(VAR_Estado_Carrera.Condicion, "No Aprobada") == 0)
			Materias_Reprobadas ++;

		else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursando") == 0)
			Materias_En_Curso ++;

		else if(strcmp(VAR_Estado_Carrera.Condicion, "Por Cursar") == 0)
			Materias_Por_Cursar ++;

		else if(strcmp(VAR_Estado_Carrera.Condicion, "Aprobada") == 0)
			Materias_Aprobadas ++;
		
		if(strcmp(VAR_Estado_Carrera.Disponibilidad, "Disponible") == 0)
		{


			printf("----------------------------------------------------------------------------------------------------------------------------------\n");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("  %-6d| %-48s| ", VAR_Estado_Carrera.Codigo_De_Materia, VAR_Estado_Carrera.Nombre_De_Materia);

			if(strcmp(VAR_Estado_Carrera.Condicion, "No Aprobada") == 0)
				setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

			else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursada") == 0)
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			else if(strcmp(VAR_Estado_Carrera.Condicion, "Promocionada") == 0)
				setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursando") == 0)
				setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

			else if(strcmp(VAR_Estado_Carrera.Condicion, "Aprobada") == 0)
				setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			else
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			printf("%-15s", VAR_Estado_Carrera.Condicion);

			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("|           ");

			if(VAR_Estado_Carrera.Cantidad_De_Recursadas != 0)
				setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

			else
				setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			printf("%-13d", VAR_Estado_Carrera.Cantidad_De_Recursadas);

			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("| ");


			if(strcmp(VAR_Estado_Carrera.Disponibilidad, "Disponible") == 0)
				setColor(FOREGROUND_GREEN| FOREGROUND_INTENSITY);

			else
				setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

			printf("   %-17s", VAR_Estado_Carrera.Disponibilidad);

			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("|  ");

			if(VAR_Estado_Carrera.Nota >= 0 && VAR_Estado_Carrera.Nota <=3)
				setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

			else if(VAR_Estado_Carrera.Nota >= 4 && VAR_Estado_Carrera.Nota <=6)
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			else
				setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			printf("%-4s", VAR_Estado_Carrera.Nota == 0 ? " " : (sprintf(Buffer_Nota, "%d", VAR_Estado_Carrera.Nota), Buffer_Nota));
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("|\n");

		}

		Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Mostrar_Estado_Carrera");

	}
	printf("----------------------------------------------------------------------------------------------------------------------------------\n");

	printf("\n\n-----------------------------------------\n");

    setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%-33s", "Materias En Curso");

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("| ");

	setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf(" %-4d", Materias_En_Curso);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%c\n", '|');

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("-----------------------------------------\n");

	setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("%-33s", "Materias Promocionadas");

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("| ");

	setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf(" %-4d", Materias_Promocionadas);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%c\n", '|');

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("-----------------------------------------\n");

	setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%-33s", "Materias Cursadas");

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("| ");

	setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(" %-4d", Materias_Cursadas);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%c\n", '|');

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("-----------------------------------------\n");

	setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%-33s", "Materias Materias_Aprobadas");

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("| ");

	setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf(" %-4d", Materias_Aprobadas);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%c\n", '|');

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("-----------------------------------------\n");

	setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%-33s", "Materias Reprobadas");

    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("| ");

	setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(" %-4d", Materias_Reprobadas);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%c\n", '|');

	Total_De_Materias = Materias_Reprobadas + Materias_Cursadas + Materias_Promocionadas + Materias_En_Curso + Materias_Por_Cursar + Materias_Aprobadas;
	Porcentaje_Carrera = ((Materias_Aprobadas + Materias_Promocionadas + Materias_Cursadas) * 100) / Total_De_Materias;

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("------------- ----------------------------\n");
    printf("%-33s|  %-4d", "Materias Por Cursar", Materias_Por_Cursar);
    printf("%-2s\n", " |");
    printf("------------- ----------------------------\n");
    printf("%-33s|  %-4d", "Total de materias", Total_De_Materias);
    printf("%-2s\n", " |");
	printf("------------- ----------------------------\n");
    printf("%-33s| %.2f%-2c", "Porcentaje de carrera hecho", Porcentaje_Carrera, '%');
    printf("%s", "|");
	printf("\n-----------------------------------------\n");
	printf("\n\n");

	system("pause");

}
//#############################################################################################################################################
int Buscar_Materia_Estado_Carrera(FILE *Archivo_Estado_Carrera, int Codigo_De_Materia_Ingresado)
{

	STR_Estado_Carrera VAR_Estado_Carrera;

	rewind(Archivo_Estado_Carrera);

	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Buscar_Materia_Estado_Carrera");
	while(!feof(Archivo_Estado_Carrera))
	{

		if(VAR_Estado_Carrera.Codigo_De_Materia == Codigo_De_Materia_Ingresado)
			return ftell(Archivo_Estado_Carrera);

		Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Buscar_Materia_Estado_Carrera");

	}

	return -1;

}
//#############################################################################################################################################
int Buscar_Materia(FILE *Archivo_Materias, int Codigo_De_Materia_Ingresado)
{

	STR_Materias VAR_Materias;

	rewind(Archivo_Materias);

	Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Buscar_Materia");
	while(!feof(Archivo_Materias))
	{

		if(VAR_Materias.Codigo_De_Materia == Codigo_De_Materia_Ingresado)
			return ftell(Archivo_Materias);

		Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Buscar_Materia");

	}

	return -1;

}
//#############################################################################################################################################
void Mostrar_Materia_Seleccionada(FILE *Archivo_Estado_Carrera, int Posicion_En_Bytes)
{

	STR_Estado_Carrera VAR_Estado_Carrera;
	
	system("cls");

	rewind(Archivo_Estado_Carrera);
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Mostrar_Materia_Seleccionada");

	printf("%-10s%-50s%-18s%-25s%-6s\n", "Codigo", "Nombre","Condicion", "Cantidad de recursadas", "Nota");
	printf("%-10s%-50s%-18s%-25s%-6s\n\n", "------", "------","---------", "----------------------", "----");

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("%-10d%-50s", VAR_Estado_Carrera.Codigo_De_Materia, VAR_Estado_Carrera.Nombre_De_Materia);

	if(strcmp(VAR_Estado_Carrera.Condicion, "No Aprobada") == 0)
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursada") == 0)
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	else if(strcmp(VAR_Estado_Carrera.Condicion, "Promocionada") == 0)
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	else if(strcmp(VAR_Estado_Carrera.Condicion, "Cursando") == 0)
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	else if(strcmp(VAR_Estado_Carrera.Condicion, "Aprobada") == 0)
		setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	else
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	printf("%-18s", VAR_Estado_Carrera.Condicion);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("            ");

	if(VAR_Estado_Carrera.Cantidad_De_Recursadas != 0)
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	else
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	printf("%-13d", VAR_Estado_Carrera.Cantidad_De_Recursadas);

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("  ");

	if(VAR_Estado_Carrera.Nota >= 0 && VAR_Estado_Carrera.Nota <=3)
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	else if(VAR_Estado_Carrera.Nota >= 4 && VAR_Estado_Carrera.Nota <=6)
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	else
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	printf("%-6d", VAR_Estado_Carrera.Nota);
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("\n");

	/*
	printf("%-10d%-50s%-14s%-25d%-22s%-6d\n", VAR_Estado_Carrera.Codigo_De_Materia, VAR_Estado_Carrera.Nombre_De_Materia,VAR_Estado_Carrera.Condicion, VAR_Estado_Carrera.Cantidad_De_Recursadas, VAR_Estado_Carrera.Disponibilidad, VAR_Estado_Carrera.Nota);
*/
}
//#############################################################################################################################################
void Modificar_Condicion(FILE *Archivo_Estado_Carrera, int Posicion_En_Bytes)
{

	STR_Estado_Carrera VAR_Estado_Carrera;
	int Seleccion_Menu;

	rewind(Archivo_Estado_Carrera);
	
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Condicion");
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);

	Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);
	Mostrar_Menu_Condicion();
	Seleccion_Menu = Ingresa_Menu_Valido(1, 7);

	while(Seleccion_Menu == -1)
	{

		Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);
		Mostrar_Menu_Condicion();

		printf("Error: Opcion Invalida!\nIntente denuevo: ");
		Seleccion_Menu = Ingresa_Menu_Valido(1, 7);

	}

	switch(Seleccion_Menu)
	{

		case 1:

			strcpy(VAR_Estado_Carrera.Condicion, "No Aprobada");
			VAR_Estado_Carrera.Nota = 0;

		break;

		case 2:

			strcpy(VAR_Estado_Carrera.Condicion, "Cursada");

		break;

		case 3:

			strcpy(VAR_Estado_Carrera.Condicion, "Promocionada");

		break;

		case 4:

			strcpy(VAR_Estado_Carrera.Condicion, "Cursando");

		break;

		case 5:

			strcpy(VAR_Estado_Carrera.Condicion, "Por Cursar");

		break;

		case 6:

			strcpy(VAR_Estado_Carrera.Condicion, "Aprobada");

		break;

	}

	if(Seleccion_Menu >= 1 && Seleccion_Menu <= 6)
	{

		fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
		Error_Lectura_Escritura(fwrite(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Condicion");

	}

}
//#############################################################################################################################################
void Modificar_Recursadas(FILE *Archivo_Estado_Carrera, int Posicion_En_Bytes)
{

	STR_Estado_Carrera VAR_Estado_Carrera;

	int Cantidad;

	rewind(Archivo_Estado_Carrera);
	
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Condicion");
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);

	printf("\nIngrese la cantidad: ");
	Cantidad = Ingresa_Cantidad_Recursadas_Valida();

	while(Cantidad == -1)
	{

		Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);

		printf("Error: Cantidad Invalida!\nIntente denuevo: ");
		Cantidad = Ingresa_Cantidad_Recursadas_Valida();

	}

	VAR_Estado_Carrera.Cantidad_De_Recursadas = Cantidad;

	Error_Lectura_Escritura(fwrite(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Recursadas");
	
}
//#############################################################################################################################################
void Modificar_Nota(FILE *Archivo_Estado_Carrera, int Posicion_En_Bytes)
{

	STR_Estado_Carrera VAR_Estado_Carrera;

	int Nota;

	rewind(Archivo_Estado_Carrera);
	
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Condicion");
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);

	printf("\nIngrese la nueva nota: ");
	Nota = Ingresa_Cantidad_Recursadas_Valida();

	while(Nota == -1)
	{

		Mostrar_Materia_Seleccionada(Archivo_Estado_Carrera, Posicion_En_Bytes);

		printf("Error: Nota Invalida!\nIntente denuevo: ");
		Nota = Ingresa_Cantidad_Recursadas_Valida();

	}

	VAR_Estado_Carrera.Nota = Nota;

	Error_Lectura_Escritura(fwrite(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Modificar_Nota");
	
}
//#############################################################################################################################################
void Actualizar_Disponibilidad(FILE *Archivo_Estado_Carrera, FILE *Archivo_Materias)
{
    STR_Estado_Carrera VAR_Estado_Carrera;
    STR_Materias VAR_Materias;

    STR_Materias *VEC_Archivo_Materias = NULL;
    STR_Estado_Carrera *VEC_Estado_Carrera = NULL;

    int Cantidad_Materias_Estado = 0;
    int Cantidad_Materias_Materias = 0;
    int Indice = 0, Indice_Materias = 0;

    int Todas_Correlativas_Cumplidas = 0;
	int Correlativa = 0;
	int Encontrada = 0;
	int i = 0;
	int j = 0;

    // Leer materias y almacenarlas en un vector dinámico
    rewind(Archivo_Materias);
    Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Actualizar_Disponibilidad");
    while(!feof(Archivo_Materias))
    {

        Cantidad_Materias_Materias++;
        VEC_Archivo_Materias = realloc(VEC_Archivo_Materias, Cantidad_Materias_Materias * sizeof(STR_Materias));
        
        if(VEC_Archivo_Materias == NULL)
        {

            printf("Error: No se pudo realojar memoria para VEC_Archivo_Materias\n");
            exit(1);

        }

        VEC_Archivo_Materias[Cantidad_Materias_Materias - 1] = VAR_Materias;
        Error_Lectura_Escritura(fread(&VAR_Materias, sizeof(STR_Materias), 1, Archivo_Materias), "Actualizar_Disponibilidad");
    
	}
    // Leer estado de carrera y almacenarlas en un vector dinámico
    rewind(Archivo_Estado_Carrera);
    Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Actualizar_Disponibilidad");
    while(!feof(Archivo_Estado_Carrera))
    {

        Cantidad_Materias_Estado++;
        VEC_Estado_Carrera = realloc(VEC_Estado_Carrera, Cantidad_Materias_Estado * sizeof(STR_Estado_Carrera));
        
        if (VEC_Estado_Carrera == NULL)
        {

            printf("Error reallocating memory for VEC_Estado_Carrera\n");
            exit(1);

        }

        VEC_Estado_Carrera[Cantidad_Materias_Estado - 1] = VAR_Estado_Carrera;
        Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Actualizar_Disponibilidad");
    
    
    }

    // Actualizar disponibilidad en VEC_Estado_Carrera basado en las correlativas de VEC_Archivo_Materias
    for(Indice = 0; Indice < Cantidad_Materias_Estado; Indice++)
    {

        for(Indice_Materias = 0; Indice_Materias < Cantidad_Materias_Materias; Indice_Materias++)
        {
            
            if(VEC_Estado_Carrera[Indice].Codigo_De_Materia == VEC_Archivo_Materias[Indice_Materias].Codigo_De_Materia)
            {

                Todas_Correlativas_Cumplidas = 1;
                for(i = 0; i < 100; i++)
                {

                    Correlativa = VEC_Archivo_Materias[Indice_Materias].Materias_Correlativas[i];
                    if(Correlativa != 0)
                    {

                        Encontrada = 0;
                        for(j = 0; j < Cantidad_Materias_Estado; j++)
                        {

                            if(VEC_Estado_Carrera[j].Codigo_De_Materia == Correlativa && (strcmp(VEC_Estado_Carrera[j].Condicion, "Aprobada") == 0 || strcmp(VEC_Estado_Carrera[j].Condicion, "Promocionada") == 0 || strcmp(VEC_Estado_Carrera[j].Condicion, "Cursada") == 0))
                            {

                                Encontrada = 1;
                                break;

                            }

                        }
                        if(!Encontrada)
                        {

                            Todas_Correlativas_Cumplidas = 0;
                            break;

                        }

                    }

                }

                if(Todas_Correlativas_Cumplidas)
                	strcpy(VEC_Estado_Carrera[Indice].Disponibilidad, "Disponible");

                else
                    strcpy(VEC_Estado_Carrera[Indice].Disponibilidad, "No Disponible");

                break;

            }

        }

    }

    // Escribir los resultados actualizados de vuelta al archivo
    rewind(Archivo_Estado_Carrera);

    for(Indice = 0; Indice < Cantidad_Materias_Estado; Indice++)
    	fwrite(&VEC_Estado_Carrera[Indice], sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera);

    free(VEC_Archivo_Materias);
    free(VEC_Estado_Carrera);

    fflush(Archivo_Estado_Carrera);

}
//#############################################################################################################################################
int Verificar_Disponibilidad(FILE *Archivo_Estado_Carrera, int Posicion_En_Bytes)
{

	STR_Estado_Carrera VAR_Estado_Carrera;

	rewind(Archivo_Estado_Carrera);
	
	fseek(Archivo_Estado_Carrera, Posicion_En_Bytes - sizeof(STR_Estado_Carrera), SEEK_SET);
	Error_Lectura_Escritura(fread(&VAR_Estado_Carrera, sizeof(STR_Estado_Carrera), 1, Archivo_Estado_Carrera), "Buscar_Materia_Estado_Carrera");

	if(strcmp(VAR_Estado_Carrera.Disponibilidad, "Disponible") == 0)
		return 1;

	else
		return -1;

}