/*
* Atividade: Avalia??o Semana 7 - CEL080 - 2021.3 - TURMA A
* Autor: David Nery Henriques Knop
* Matr?cula: 201669038A
* Data: 13/12/2021
*/

/* Includes Padr?o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Vari?veis de Controle*/
#define TEMP_SAMPLE_READING_INTERVAL 1000 // ms
FILE *fp;
char filename[] = "src\\AV7\\temp_AV7.txt";
char temperatureSampleStr[] = "";
double temperatureSampleValue;
int STRSIZE = 9; /* Como especificar esse valor?*/

/* Prot?tipo das fun?oes*/
void vTaskReadSensor_av7(void* pvParameters);
void vTaskDisplay_av7(void* pvParameters);
int	openFileToRead(char* filename, const char* readMode);

/* Handle para o Sem?foro Bin?rio */
xSemaphoreHandle xBinarySemaphore;

void main_av7(void)
{
	/*Cria o Sem?foro Bin?rio*/
	vSemaphoreCreateBinary(xBinarySemaphore);

	/* Testa se o sem?foro foi criado corretamente */
	if (xBinarySemaphore != NULL)
	{
		/* Executando a leitura inicial do arquivo de amostras de temperatura
		** > Nome do arquivo presente na variavel e modo de somente leitura. */
		if (openFileToRead(filename, "r")) return;
		printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
		printf_colored("> O arquivo de amostras de temperatura foi aberto corretamente!\r\n", COLOR_YELLOW);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_YELLOW);

		/* Realiza um take no sem?foro para garantir que ele ficar? indispon?vel */
		xSemaphoreTake(xBinarySemaphore, 0);

		/*Cria a Task Handler*/
		xTaskCreate(vTaskDisplay_av7, "TaskDisplay", 1000, NULL, 2, NULL);
		printf_colored("---------------------------------------------------------------\r\n", COLOR_GREEN);
		printf_colored(">> Task \"Display\" Criada!\r\n", COLOR_GREEN);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_GREEN);

		/*Cria a Task Peri?dica*/
		xTaskCreate(vTaskReadSensor_av7, "TaskReadSensor", 1000, NULL, 1, NULL);
		printf_colored("---------------------------------------------------------------\r\n", COLOR_CYAN);
		printf_colored(">> Task Peri?dica \"Read Sensor\" Criada!\r\n", COLOR_CYAN);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_CYAN);

		/*Inicia o Escalonador*/
		printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
		printf_colored(">> Escalonador Iniciado!\r\n", COLOR_YELLOW);
		printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
		printf_colored("===============================================================\r\n", COLOR_RED);
		vTaskStartScheduler();

		while (1);
	}
}

// Task para exibi??o da amostra lida pela task de leitura de temperatura
void vTaskDisplay_av7(void* pvParameters)
{
	for (;;)
	{
		//printf_colored("Task Display tenta adquirir o sem?foro...\n", COLOR_GREEN);
		// portMAX_DELAY indica bloqueio infinito at? obter o sem?foro
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		printf_colored("Task Display est? processando a amostra obtida!\n", COLOR_GREEN);
		printf("> Temperatura lida: %.02f ?C\n", temperatureSampleValue);
	}
}

// Task para leitura de amostras de temperatura presentes no arquivo de dados
void vTaskReadSensor_av7(void* pvParameters)
{
	for (;;)
	{
		printf_colored("\nTask Peri?dica Read Sensor est? executando...\n", COLOR_CYAN);

		if (fgets(temperatureSampleStr, STRSIZE, fp) == NULL)
		{
			printf_colored("\n------------------------------------------------------\r\n", COLOR_RED);
			printf_colored(">> Todas as amostras de temperatura foram processadas!\r\n", COLOR_RED);
			printf_colored("------------------------------------------------------\r\n\n", COLOR_RED);

			fclose(fp);
			vTaskDelete(NULL); /* O que acontece aqui?*/
		}
		/*Convers?o da amostra lida de string para double*/
		/*As leituras variam de 0 - 1024, ou, em ?C, 0 - 100?C
			Logo, fazemos uma regra de tr?s para convers?o
				1024				   = 100?C
				temperatureSampleValue = x?C
				x = (100*temperatureSampleValue)/1024
		*/
		temperatureSampleValue = (atof(temperatureSampleStr)*100)/1024;
			
		/*Imp?e um intervalo entre as leituras das amostras de temperatura do arquivo*/
		vTaskDelay(TEMP_SAMPLE_READING_INTERVAL / portTICK_RATE_MS);

		//printf_colored("Task Peri?dica Read Sensor ir? liberar o sem?foro!\n", COLOR_CYAN);
		/*Libera??o do sem?foro bin?rio*/
		xSemaphoreGive(xBinarySemaphore);
	}
}

//Fun??o para leitura do arquivo de dados
int openFileToRead(char* filename, const char* readMode)
{
	if ((fp = fopen(filename, readMode)) == NULL) {
		printf_colored("\n------------------------------------------------\r\n", COLOR_RED);
		printf("> Arquivo \"%s\" n?o localizado", filename);
		printf_colored("\n------------------------------------------------\r\n\n\n", COLOR_RED);
		
		return 1;
	}

	return 0;
}