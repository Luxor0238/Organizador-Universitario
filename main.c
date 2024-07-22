//#############################################################################################################################################
//#############################################################################################################################################
//Librerias####################################################################################################################################
#include <stdio.h>	//Manejo de Entrada y Salida
#include <windows.h> //Para el manejo de la ventana de consola
//#############################################################################################################################################
#include "Codigo_H/archivos.h"
#include "Codigo_H/materias.h"
#include "Codigo_H/otros.h"
#include "Codigo_H/finales.h"
#include "Codigo_H/parciales.h"
#include "Codigo_H/trabajos_practicos.h"
//#############################################################################################################################################
//#############################################################################################################################################
//Funciones####################################################################################################################################
int main(void)
{

	FILE *Archivo_Materias_CSV; //Puntero a FILE para el archivo CSV
	FILE *Archivo_Materias; //Puntero a FILE para el archivo BIN
	FILE *Archivo_Estado_Carrera; //Puntero a FILE para el archivo del estado actual de la carrera
	FILE *Archivo_Parciales; //Puntero a FILE para el archivo de parciales
	FILE *Archivo_Trabajos_Practicos; //Puntero a FILE para el archivo de TPs

	Archivo_Materias_CSV = NULL;
	Archivo_Materias = NULL;
	Archivo_Estado_Carrera = NULL;
	Archivo_Parciales = NULL;
	Archivo_Trabajos_Practicos = NULL;

	STR_Trabajos VAR_Trabajos;
	STR_Fecha VAR_Fecha;
	int Fecha_Valida;

	char Nombre_Trabajo_Practico[50];

	int Seleccion_Menu;
	int Seleccion_Menu_Plan;
	int Seleccion_Menu_Trabajos;
	int Plan_De_Estudios_Cargado = 0;
	int Bandera_Plan_Cargado = 1;
	int Nota;

	int Codigo_De_Materia_Ingresado;
	int Codigo_De_Trabajo_Ingresado;
	int Posicion_En_Bytes;
	int Materia_Disponible;

	int Comfirmacion;
	int Es_Grupal;

	Maximizar_Ventana();

	do
	{

		if(Plan_De_Estudios_Cargado && Bandera_Plan_Cargado)
		{

			Archivo_Materias = Abrir_Archivo("Archivos/Materias.bin", "r", 1);
			Archivo_Estado_Carrera = Abrir_Archivo("Archivos/Estado_Carrera.bin", "r+b", 1);
			Archivo_Parciales = Abrir_Archivo("Archivos/Parciales.bin", "r+b", 1);
			Archivo_Trabajos_Practicos = Abrir_Archivo("Archivos/trabajos_practicos.bin", "r+b", 1);

			Mostrar_Menu_Plan_De_Estudios();

			setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("\n\nEl proseso se completo Correctamente!!\n\n");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			system("pause");

			Bandera_Plan_Cargado = 0;

		}
		else
		{

			Archivo_Materias = Abrir_Archivo("Archivos/Materias.bin", "r", 0);
			Archivo_Estado_Carrera = Abrir_Archivo("Archivos/Estado_Carrera.bin", "r+b", 0);
			Archivo_Parciales = Abrir_Archivo("Archivos/Parciales.bin", "r+b", 0);
			Archivo_Trabajos_Practicos = Abrir_Archivo("Archivos/trabajos_practicos.bin", "r+b", 0);

			if(Archivo_Materias != NULL && Archivo_Estado_Carrera != NULL && Archivo_Parciales != NULL && Archivo_Trabajos_Practicos != NULL)
			{

				Bandera_Plan_Cargado = 0;
				Plan_De_Estudios_Cargado = 1;
			}


		}

		Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado);
		Seleccion_Menu = Ingresa_Menu_Valido(1, 9);
		while(Seleccion_Menu == -1)
		{

			Mostrar_Menu_Gestion(Plan_De_Estudios_Cargado);

			setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Error: Opcion Invalida! Intente denuevo:");
			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("\n--> ");
			Seleccion_Menu = Ingresa_Menu_Valido(1, 9);

		}

		switch(Seleccion_Menu)
		{

			case 1:

				if(Plan_De_Estudios_Cargado)
				{

					Cambiar_Estado_Materias(Archivo_Estado_Carrera, Archivo_Materias, 32, Plan_De_Estudios_Cargado);
					Actualizar_Disponibilidad(Archivo_Estado_Carrera, Archivo_Materias);

				}
				else
					Error_Plan_De_Estudios();


			break;

			case 2:

				if(Plan_De_Estudios_Cargado)
					Mostrar_Materias(Archivo_Materias);
				
				else
					Error_Plan_De_Estudios();

			break;

			case 3:

				if(Plan_De_Estudios_Cargado)
					Mostrar_Estado_Carrera(Archivo_Estado_Carrera);

				else
					Error_Plan_De_Estudios();

			break;

			case 4:

				if(Plan_De_Estudios_Cargado)
				{

					Mostrar_Menu_Trabajos();

					Seleccion_Menu_Trabajos = Ingresa_Menu_Valido(1, 5);
					while(Seleccion_Menu_Trabajos == -1)
					{

						Mostrar_Menu_Trabajos();

						setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
						printf("Error: Opcion Invalida! Intente denuevo:");
						setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						printf("\n--> ");
						Seleccion_Menu_Trabajos = Ingresa_Menu_Valido(1, 5);

					}

					switch(Seleccion_Menu_Trabajos)
					{

						case 1:

							printf("\n\nIngrese el codigo de materia para el Trabajo Practico (0 para cancelar)\n-----------------------------------------------------------------------\n--> ");
							Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();

							if(Codigo_De_Materia_Ingresado != 0)
								Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);

							while(Posicion_En_Bytes == -1 && Codigo_De_Materia_Ingresado != 0)
							{

								if(Codigo_De_Materia_Ingresado == 0)
									break;

								Mostrar_Menu_Trabajos();
								setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
								printf("\n\n\nLa materia ingresada no existe! Intente denuevo (0 para cancelar)\n---------------------------------------------------------\n--> ");
								setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();
								Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);

							}

							if(Codigo_De_Materia_Ingresado != 0)
								Materia_Disponible = Verificar_Disponibilidad(Archivo_Estado_Carrera, Posicion_En_Bytes);

							while(Materia_Disponible == -1 && Codigo_De_Materia_Ingresado != 0)
							{

								Mostrar_Menu_Trabajos();
								setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
								printf("\n\n\nLa materia ingresada esta no disponible! Intente con otra (0 para cancelar)\n-----------------------------------------------\n--> ");
								setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								Codigo_De_Materia_Ingresado = Ingresa_Entero_Valido();

								if(Codigo_De_Materia_Ingresado == 0)
									break;

								Posicion_En_Bytes = Buscar_Materia_Estado_Carrera(Archivo_Estado_Carrera, Codigo_De_Materia_Ingresado);
								Materia_Disponible = Verificar_Disponibilidad(Archivo_Estado_Carrera, Posicion_En_Bytes);

							}
							
							if(Codigo_De_Materia_Ingresado != 0)
								Nuevo_Trabajo_Practico(Archivo_Trabajos_Practicos, Archivo_Materias, Codigo_De_Materia_Ingresado);

						break;

						case 2:
							
							Mostrar_Menu_Trabajos();
							printf("\n\nIngrese el codigo del trabajo que desea modificar: (0 para cancelar)\n-------------------------------------------------\n--> ");
							Codigo_De_Trabajo_Ingresado = Ingresa_Entero_Valido();

							if(Codigo_De_Materia_Ingresado != 0)
								Posicion_En_Bytes = Buscar_Trabajo_Practico(Archivo_Trabajos_Practicos, Codigo_De_Trabajo_Ingresado);

							while(Posicion_En_Bytes == -1 && Codigo_De_Materia_Ingresado != 0)
							{

								Mostrar_Menu_Trabajos();
								setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
								printf("\n\n\nEl trabajo ingresado no existe! Intente denuevo (0 para cancelar)\n---------------------------------------------------------\n--> ");
								setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								Codigo_De_Trabajo_Ingresado = Ingresa_Entero_Valido();

								if(Codigo_De_Materia_Ingresado == 0)
									break;

								Posicion_En_Bytes = Buscar_Trabajo_Practico(Archivo_Trabajos_Practicos, Codigo_De_Trabajo_Ingresado);

							}

							if(Codigo_De_Materia_Ingresado != 0)
							{

								rewind(Archivo_Trabajos_Practicos);
								fseek(Archivo_Trabajos_Practicos, Posicion_En_Bytes - sizeof(STR_Trabajos), SEEK_SET);
								Error_Lectura_Escritura(fread(&VAR_Trabajos, sizeof(STR_Trabajos), 1, Archivo_Trabajos_Practicos), "Main");

							}

							if(Codigo_De_Materia_Ingresado != 0)
							{

								Mostrar_Menu_Modificar_Trabajos();
								Seleccion_Menu = Ingresa_Menu_Valido(1, 7);
								while(Seleccion_Menu == -1)
								{

									Mostrar_Menu_Trabajos();
									Mostrar_Menu_Modificar_Trabajos();

									setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
									printf("Error: Opcion Invalida! Intente denuevo:");
									setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									printf("\n--> ");
									Seleccion_Menu = Ingresa_Menu_Valido(1, 7);

								}

							}
							else
								Seleccion_Menu = 0;

							switch(Seleccion_Menu)
							{

								case 1:

									printf("Ingrese el nombre del Trabajo Practico: ");
									fflush(stdin);
									fgets(Nombre_Trabajo_Practico, sizeof(Nombre_Trabajo_Practico), stdin);
									Leer_Texto(Nombre_Trabajo_Practico);
									strcpy(VAR_Trabajos.Nombre_De_Trabajo, Nombre_Trabajo_Practico);

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

								case 2:

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

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

								case 3:

									printf("\nEsta entregado? 1) Si\t2) No: ");
									Es_Grupal = Ingresa_Menu_Valido(1, 2);
									while(Es_Grupal < 1 || Es_Grupal > 2)
									{

										printf("Error: Opcion Invalida!\nIntente denuevo: ");
										Es_Grupal = Ingresa_Menu_Valido(1, 2);

									}
									if(Es_Grupal == 1)
										strcpy(VAR_Trabajos.Entregado, "Si");

									else
										strcpy(VAR_Trabajos.Entregado, "No");

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

								case 4:

									printf("\nEsta entregado? 1) Si\t2) No: ");
									Es_Grupal = Ingresa_Menu_Valido(1, 2);
									while(Es_Grupal < 1 || Es_Grupal > 2)
									{

										printf("Error: Opcion Invalida!\nIntente denuevo: ");
										Es_Grupal = Ingresa_Menu_Valido(1, 2);

									}
									if(Es_Grupal == 1)
										strcpy(VAR_Trabajos.Aprobado, "Si");

									else
										strcpy(VAR_Trabajos.Aprobado, "No");

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

								case 5:

									printf("\nIngrese la nueva nota: ");
									Nota = Ingresa_Cantidad_Recursadas_Valida();
									VAR_Trabajos.Nota = Nota;

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

								case 6:

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

									Modificar_Trabajo_Practico(Archivo_Trabajos_Practicos, VAR_Trabajos, Codigo_De_Trabajo_Ingresado);

								break;

							}

						break;

						case 3:

							Eleiminar_Trabajo_Practico(Archivo_Trabajos_Practicos);

						break;

						case 4:

							Mostrar_Trabajos(Archivo_Trabajos_Practicos);

						break;

					}

				}
				else
					Error_Plan_De_Estudios();

			break;

			case 5:

				if(Plan_De_Estudios_Cargado)
				{

					Consultar_Y_Modificar_Parciales(Archivo_Parciales, Archivo_Estado_Carrera);

				}
				else
					Error_Plan_De_Estudios();

			break;

			case 6:

				if(Plan_De_Estudios_Cargado)
				{

					Actualizar_Y_Mostrar_Finales(Archivo_Estado_Carrera);

				}
				else
					Error_Plan_De_Estudios();

			break;

			case 7:

				Mostrar_Menu_Plan_De_Estudios();
				Seleccion_Menu_Plan = Ingresa_Menu_Valido(1, 4);
				while(Seleccion_Menu_Plan == -1)
				{

					Mostrar_Menu_Plan_De_Estudios();

					setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
					printf("Error: Opcion Invalida! Intente denuevo:");
					setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					printf("\n--> ");
					Seleccion_Menu_Plan = Ingresa_Menu_Valido(1, 4);

				}

				switch(Seleccion_Menu_Plan)
				{

					case 1:

						setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						printf("\nIntroduzca el archivo CSV con las materias del plan de estudios en la carpeta /Archivos/Plan_de_Estudios/Plan.csv\n\n");
						setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						system("pause");

						Mostrar_Menu_Plan_De_Estudios();
						printf("\n\nCargando: Porfavor espere!\n\n");

						Archivo_Materias_CSV = Abrir_Archivo("Archivos/Plan_de_Estudios/Plan.csv", "r", 1);
						Archivo_Materias = Abrir_Archivo("Archivos/Materias.bin", "w+b", 1);
						Archivo_Estado_Carrera = Abrir_Archivo("Archivos/Estado_Carrera.bin", "w+b", 1);
						Archivo_Parciales = Abrir_Archivo("Archivos/Parciales.bin", "w+b", 1);
						Archivo_Trabajos_Practicos = Abrir_Archivo("Archivos/trabajos_practicos.bin", "w+b", 1);

						Pasar_Materias_CSV_a_BIN(Archivo_Materias_CSV, Archivo_Materias);
						Cargar_Archivo_Progreso_Desde_Cero(Archivo_Materias, Archivo_Estado_Carrera, Archivo_Parciales);

						Cerrar_Archivo(&Archivo_Materias);
						Cerrar_Archivo(&Archivo_Estado_Carrera);
						Cerrar_Archivo(&Archivo_Parciales);
						Cerrar_Archivo(&Archivo_Trabajos_Practicos);


						Plan_De_Estudios_Cargado = 1;

					break;

					case 2:

						if(Plan_De_Estudios_Cargado == 0)
						{

							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf("\nMueva los archivos ");
							setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf("Materias.bin");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf(", ");
							setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf("Estado_Carrera.bin");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf(", ");
							setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf("trabajos_practicos.bin");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf(" y ");
							setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf("Parciales.bin");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							printf(" que desee a Archivos/..\n\n");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							system("pause");

							Plan_De_Estudios_Cargado = 1;

						}

						else
						{

							Mostrar_Menu_Plan_De_Estudios();
							setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
							printf("\n\nSe ha producido un error!!\nPrimero cierre el plan actual\n\n");
							setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							system("pause");

						}

					break;

					case 3:

						Comfirmacion = Mostrar_Cuadro_De_Comfirmacion("Esta seguro de cerrar el plan en curso?");

						if(Comfirmacion == 1)
						{

							Mostrar_Menu_Plan_De_Estudios();
							printf("\n\nCargando: Porfavor espere!\n\n");

							if(Archivo_Materias == NULL || Archivo_Estado_Carrera == NULL || Archivo_Parciales == NULL || Archivo_Trabajos_Practicos == NULL)
							{

								Mostrar_Menu_Plan_De_Estudios();
								setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
								printf("\n\nSe ha producido un error!!\nNo hay ningun plan cargado\n\n");
								setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								system("pause");

							}
							else
							{

								Cerrar_Archivo(&Archivo_Materias);
								Cerrar_Archivo(&Archivo_Estado_Carrera);
								Cerrar_Archivo(&Archivo_Parciales);
								Cerrar_Archivo(&Archivo_Trabajos_Practicos);

								Plan_De_Estudios_Cargado = 0;
								Bandera_Plan_Cargado = 1;

								rename("Archivos/Materias.bin", "Archivos/Plan_Cerrado/Materias.bin");
								rename("Archivos/Estado_Carrera.bin", "Archivos/Plan_Cerrado/Estado_Carrera.bin");
								rename("Archivos/Parciales.bin", "Archivos/Plan_Cerrado/Parciales.bin");
								rename("Archivos/trabajos_practicos.bin", "Archivos/Plan_Cerrado/trabajos_practicos.bin");

								Mostrar_Menu_Plan_De_Estudios();

								setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
								printf("\n\nEl proseso se completo Correctamente!!\n\n");
								setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

								system("pause");

							}

						}

					break;

				}

			break;

		}

	}while(Seleccion_Menu != 8);

	Cerrar_Archivo(&Archivo_Materias);
	Cerrar_Archivo(&Archivo_Estado_Carrera);
	Cerrar_Archivo(&Archivo_Parciales);
	Cerrar_Archivo(&Archivo_Trabajos_Practicos);

	return 0;

}