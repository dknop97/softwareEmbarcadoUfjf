/*
* Atividade: Avalia��o Semana 7 - CEL080 - 2021.3 - TURMA A
* Autor: David Nery Henriques Knop
* Matr�cula: 201669038A
* Data: 13/12/2021
*/

/* Includes Padr�o C */
#include <stdio.h>
#include <locale.h>
//#include <string.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Vari�veis de Controle*/
#define TEMP_SAMPLE_READING_INTERVAL 0.00001000 // ms
char filename[] = "temp_AV7.txt";
FILE *fp;
int STRSIZE = 99; /* SEI L�*/
char temperatureSampleStr[] = "";
double temperatureSampleValue;

/* Prot�tipo das fun�oes*/
void vTaskReadSensor_av7(void* pvParameters);
void vTaskDisplay_av7(void* pvParameters);
int readFile(char* filename, const char* readMode);

/* Handle para o Sem�foro Bin�rio */
xSemaphoreHandle xBinarySemaphore;

void main_av7(void)
{
	/*Cria o Sem�foro Bin�rio*/
	vSemaphoreCreateBinary(xBinarySemaphore);

	/* Testa se o sem�foro foi criado corretamente */
	if (xBinarySemaphore != NULL)
	{
		/* Executando a leitura inicial do arquivo de amostras de temperatura
		** > Nome do arquivo presente na variavel e modo de somente leitura. */
		if (readFile(filename, "r")) return;

		/* Realiza um take no sem�foro para garantir que ele ficar� indispon�vel */
		xSemaphoreTake(xBinarySemaphore, 0);

		/*Cria a Task Handler*/
		xTaskCreate(vTaskDisplay_av7, "TaskDisplay", 1000, NULL, 2, NULL);
		printf(">> Task \"Display\" Criada!\r\n\n");

		/*Cria a Task Peri�dica*/
		xTaskCreate(vTaskReadSensor_av7, "TaskReadSensor", 1000, NULL, 1, NULL);
		printf(">> Task Peri�dica \"Sensor\" Criada!\r\n\n");
		
		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf(">> Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
}

// Task para exibi��o da amostra lida pela task de leitura de temperatura
void vTaskDisplay_av7(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	for (;;)
	{
		//printf_colored("Task Display tenta adquirir o sem�foro...\n", COLOR_GREEN);
		// portMAX_DELAY indica bloqueio infinito at� obter o sem�foro
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		// Se quis�ssemos passar um tempo limite, devemos passar como 
		// par�metro a quantidade de TICKS desejada
		printf_colored("Task Display est� processando a amostra obtida!\n", COLOR_GREEN);
		printf("> Temperatura lida: %.02f �C\n", temperatureSampleValue);
		
		/*Este loop for � utilizado para emular o processamento desta task.*/
		//for (ui = 0; ui < 2; ui++)
		//{
		//	for (ul = 0; ul < 10000000; ul++)
		//	{
		//		/////
		//	}
		//}
	}
}

// Task para leitura de amostras de temperatura presentes no arquivo de dados
void vTaskReadSensor_av7(void* pvParameters)
{
	for (;;)
	{
		printf_colored("\nTask Peri�dica Read Sensor est� executando...\n", COLOR_CYAN);

		if (fgets(temperatureSampleStr, STRSIZE, fp) == NULL)
		{
			printf_colored("\n------------------------------------------------------\r\n", COLOR_RED);
			printf_colored(">> Todas as amostras de temperatura foram processadas!\r\n", COLOR_RED);
			printf_colored("------------------------------------------------------\r\n\n", COLOR_RED);

			fclose(fp);
			vTaskDelete(NULL); /* O que acontece aqui?*/
		} else {
			/*Convers�o da amostra lida de string para double*/
			temperatureSampleValue = atof(temperatureSampleStr);
			
			/*Imp�e um intervalo entre as leituras das amostras de temperatura do arquivo*/
			vTaskDelay(TEMP_SAMPLE_READING_INTERVAL / portTICK_RATE_MS);

			//printf_colored("Task Peri�dica Read Sensor ir� liberar o sem�foro!\n", COLOR_CYAN);
			/*Libera��o do sem�foro bin�rio*/
			xSemaphoreGive(xBinarySemaphore);
		}
	}
}

//Fun��o para leitura do arquivo de dados
int readFile(char* filename, const char* readMode)
{
	
	if ((fp = fopen(filename, readMode)) == NULL) {
		printf_colored("\n------------------------------------------------\r\n", COLOR_RED);
		printf("> Arquivo \"%s\" n�o localizado", filename);
		printf_colored("\n------------------------------------------------\r\n\n\n", COLOR_RED);
		return 1;
	}

	return 0;
}