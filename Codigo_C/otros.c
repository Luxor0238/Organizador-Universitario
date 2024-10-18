//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>	//Manejo de Entrada y Salida
#include <windows.h> //Para el manejo de la ventana de consola
#include <ctype.h> //Para el manejo de comprobaciones de cadenas
#include <time.h> //Para el manejo de fechas
#include <stdlib.h> //Manejo de comandos de sistema Windows
//#############################################################################################################################################
#include "../Codigo_H/otros.h"
#include "../Codigo_H/trabajos_practicos.h"
#include "../Codigo_H/archivos.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
void Mostrar_Menu_Gestion(int Plan_De_Estudios_Cargado, FILE *Archivo_Trabajos_Practicos)
{

	time_t Fecha_Actual = time(NULL);
	struct tm STR_Fecha_F = *localtime(&Fecha_Actual);

	STR_Trabajos VAR_Trabajos;

	int Dias_Para_Entregar;
	int Codigo_De_Trabajo;

	int Registros = 0;

	system("cls");
	printf("Organizador Multicarrera v0.8.3\n\n%02d/%02d/%04d %02d:%02d\n\n", STR_Fecha_F.tm_mday, STR_Fecha_F.tm_mon + 1, STR_Fecha_F.tm_year + 1900, STR_Fecha_F.tm_hour, STR_Fecha_F.tm_min);

	if(!Plan_De_Estudios_Cargado)
	{

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("No hay ningun plan de estudios cargado!!\n\n");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}

	if(Plan_De_Estudios_Cargado)
	{

		printf("----------------------Notificaciones(0)--------------------------\n\n");

		Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Mostrar_Menu_Gestion");
		while(!feof(Archivo_Trabajos_Practicos))
		{

			if(strcmp(VAR_Trabajos.Borrado, "No") == 0)
			{

				Codigo_De_Trabajo = VAR_Trabajos.Codigo_De_Trabajo;

				struct tm STR_F_Entrega = {0, 0, 0, VAR_Trabajos.Fecha_De_Entrega.Dia, VAR_Trabajos.Fecha_De_Entrega.Mes - 1, VAR_Trabajos.Fecha_De_Entrega.Anio - 1900};
				time_t Fecha_Entrega = mktime(&STR_F_Entrega);
				time_t Fecha_Actual = mktime(&STR_Fecha_F);
				Dias_Para_Entregar = difftime(Fecha_Entrega, Fecha_Actual) / (60 * 60 * 24);
				
				if(Dias_Para_Entregar > 0)
				{

					if(Dias_Para_Entregar > 20)
					{

						setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf("     Tienes %d dia/s para entregar el trabajo practico ID: %d\n", Dias_Para_Entregar, Codigo_De_Trabajo);
						setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

					}
					else if(Dias_Para_Entregar >=10 && Dias_Para_Entregar <=20)
					{

						setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf("     Tienes ");
						setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
						printf("%d", Dias_Para_Entregar);
						setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf(" dia/s para entregar el trabajo practico ID: %d\n", Codigo_De_Trabajo);
						setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

					}
					else
					{

						setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf("     Tienes ");
						setColor(FOREGROUND_RED);
						printf("%d", Dias_Para_Entregar);
						setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf(" dia/s para entregar el trabajo practico ID: %d\n", Codigo_De_Trabajo);
						setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

					}

				}

				else if(Dias_Para_Entregar == 0)
				{
					setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
					printf("          Hoy es la entrega del trabajo practico ID: %d\n", Codigo_De_Trabajo);
					setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

				}

				else
				{
					setColor(FOREGROUND_RED);
					printf("        El trabajo practico ID: %d vencio hace %d dia/s\n", Codigo_De_Trabajo, -Dias_Para_Entregar);
					setColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

				}


			}

			Registros ++;

			Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Mostrar_Menu_Gestion");

		}

		if(Registros == 0)
			printf("                     Nada que ver por aqui!!\n");

		printf("\n------------------------------------------------------------------\n\n");

	}

	printf("Opciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Cambiar el estado de una materia\n2) Ver todas las materias del plan\n3) Ver el estado actual de la carrera\n4) Gestionar Trabajos Practicos\n5) Gestionar Parciales\n6) Consultar Finales Pendientes\n7) Configuracion de Plan de Estudios\n8) Salir del Programa\n");

	printf("\n--> ");

}
//#############################################################################################################################################
void Leer_Texto(char Texto_Ingresado[])
{

	int i;
	int Cantidad_De_Caracteres = strlen(Texto_Ingresado);

	for(i=0; i<Cantidad_De_Caracteres; i++)
	{

		if(Texto_Ingresado[i] == '\n')
			Texto_Ingresado[i] = '\0';

	}

}
//#############################################################################################################################################
int Ingresa_Fecha_Valida(STR_Fecha VAR_Fecha)
{

	if(VAR_Fecha.Anio >= 1582)
		if(VAR_Fecha.Mes>=1 && VAR_Fecha.Mes<=12)
			if(VAR_Fecha.Dia>1 && VAR_Fecha.Dia<=Cantidad_Dias_Mes(VAR_Fecha.Mes, VAR_Fecha.Anio))
				return 1;

	return -1;

}

int Cantidad_Dias_Mes(int Mes, int Anio)
{

	int Cantidad_Dias;

	if(Mes == 4 || Mes == 6 || Mes == 9 || Mes == 11)
		Cantidad_Dias = 30;

	else
		if(Mes == 2)
			Cantidad_Dias = 28 + Es_Bisiesto(Anio);

		else
			Cantidad_Dias = 31;

	return Cantidad_Dias;

}

int Es_Bisiesto(int Anio)
{

	if((Anio % 4 == 0 && Anio % 100 !=0) || Anio % 400 == 0)
		return 1;

	else
		return 0;

}
//#############################################################################################################################################
void A(void)
{

	printf("\n\n\n######################################################################################################################################\n######################################################################################################################################\n\n\n\n");
	system("pause");

}
//#############################################################################################################################################
void Maximizar_Ventana(void)
{

	HWND hWnd = GetConsoleWindow();

    if (hWnd != NULL)
    	ShowWindow(hWnd, SW_MAXIMIZE);

}
//#############################################################################################################################################
void setColor(int color) {
    // Obtener el handle de la consola estándar de salida
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Establecer el color del texto
    SetConsoleTextAttribute(hConsole, color);
}
//#############################################################################################################################################
void Mostrar_Menu_Pricipal(void)
{

	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Modificar Condicion\n2) Modificar Cantidad Recursadas\n3) Modificar Nota\n4) Atras\n");
	printf("\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Trabajos(void)
{

	system("cls");
	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Nuevo Trabajo Practico\n2) Modificar un Trabajo Practico\n3) Eliminar un Trabajo Practico\n4) Ver un Trabajo Practico\n5) Atras\n");
	printf("\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Modificar_Trabajos(void)
{

	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Cambiar Nombre\n2) Cambiar Es Grupal\n3) Cambiar Entregado\n4) Cambiar Aprobado\n5) Cambiar Nota\n6) Cambiar Fecha de Entrega\n7) Atras\n");
	printf("\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Parciales(void)
{

	system("cls");
	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Mostrar Parciales\n2) Modificar Nota 1er Parcial\n3) Modificar Nota 2do Parcial\n4) Modificar Nota Recuperatorio\n5) Atras");
	printf("\n\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Condicion(void)
{

	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) No Aprobada\n2) Cursada\n3) Promocionada\n4) Cursando\n5) No Cursada\n6) Aprobada\n7) Volver al inicio\n");
	printf("\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Plan_De_Estudios(void)
{

	system("cls");
	printf("\n\nOpciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Cargar un nuevo plan\n2) Cargar plan en curso\n3) Cerrar plan en curso\n4) Volver al inicio\n");
	printf("\n--> ");

}
//#############################################################################################################################################
void Mostrar_Menu_Finales(void)
{

	printf("Opciones Disponibles:\n");
	printf("---------------------\n");
	printf("1) Modificar Intentos restantes\n2) Modificar Llamados Restantes\n3) Modificar Proxima Fecha\n4) Volver al inicio\n");
	printf("\n--> ");

}
//#############################################################################################################################################
int Ingresa_Cantidad_Recursadas_Valida(void)
{

	int Cantidad;

	Cantidad = Ingresa_Entero_Valido();

	if(Cantidad >=0)
		return Cantidad;

	else
		return -1;

}
//#############################################################################################################################################
int Ingresa_Entero_Valido()
{

	char Codigo_Ingresado[256];
	int Caracter = 0;
	int Es_Digito;
	int Dato_Invalido = 1;
	int i = 0;

	do
	{

		fflush(stdin);
		if(fgets(Codigo_Ingresado, sizeof(Codigo_Ingresado), stdin) == NULL)
		{

			setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Ocurrio un error inesperado al procesar la entrada!\n");
			perror("Error");
			exit(3);

		}

		Caracter = strlen(Codigo_Ingresado);

		if(Codigo_Ingresado[Caracter - 1] == '\n')
		{

			Codigo_Ingresado[Caracter - 1] = '\0';
			Caracter--;
		}

		Es_Digito = isdigit(Codigo_Ingresado[0]);
		while(Es_Digito && i<Caracter)
		{

			Es_Digito = isdigit(Codigo_Ingresado[i]);
			i++;
		}

		if(Es_Digito)
			Dato_Invalido = 0;

		else
		{

			setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Error: Dato Invalido! Intente denuevo: ");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		}

	}while(Dato_Invalido);

	fflush(stdin);
	return atoi(Codigo_Ingresado);

}
//#############################################################################################################################################
int Ingresa_Menu_Valido(int Min, int Max)
{

	int Menu;

	Menu = Ingresa_Entero_Valido();

	if(Menu >=Min && Menu <=Max)
		return Menu;

	else
		return -1;

}
//#############################################################################################################################################
void Error_Plan_De_Estudios(void)
{

	setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("\nError: No hay ningun Plan de Estudios configurado\n");
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	system("pause");

}
//#############################################################################################################################################
int Mostrar_Cuadro_De_Comfirmacion(char Mensaje[])
{

	int Opcion;

	printf("----------------------------------------------\n");
	printf("|      Desea continuar la operacion?         |\n");
	printf("|--------------------------------------------|\n");
	printf("|                                            |\n");
	printf("|%-44s|\n", Mensaje);
	printf("|                                            |\n");
	printf("|--------------------------------------------|\n");
	printf("|     |  1) Si   |        |   2) NO   |      |\n");
	printf("----------------------------------------------\n");
	printf("\n--> ");

	Opcion = Ingresa_Menu_Valido(1, 2);

	while(Opcion == -1)
	{

		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Error: Opcion Invalida! Intente denuevo:");
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n--> ");
		Opcion = Ingresa_Menu_Valido(1, 2);

	}

	return Opcion;

}
