/*
* Atividade: Avaliação Semana 7 - CEL080 - 2021.3 - TURMA A
* Autor: David Nery Henriques Knop
* Matrícula: 201669038A
* Data: 13/12/2021
*/

/* Includes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Variáveis de Controle*/
#define TEMP_SAMPLE_READING_INTERVAL 1000 // ms
FILE *fp;
char filename[] = "src\\AV7\\temp_AV7.txt";
char temperatureSampleStr[] = "";
double temperatureSampleValue;
int STRSIZE = 9; /* Como especificar esse valor?*/

/* Protótipo das funçoes*/
void vTaskReadSensor_av7(void* pvParameters);
void vTaskDisplay_av7(void* pvParameters);
int	openFileToRead(char* filename, const char* readMode);

/* Handle para o Semáforo Binário */
xSemaphoreHandle xBinarySemaphore;

void main_av7(void)
{
	/*Cria o Semáforo Binário*/
	vSemaphoreCreateBinary(xBinarySemaphore);

	/* Testa se o semáforo foi criado corretamente */
	if (xBinarySemaphore != NULL)
	{
		/* Executando a leitura inicial do arquivo de amostras de temperatura
		** > Nome do arquivo presente na variavel e modo de somente leitura. */
		if (openFileToRead(filename, "r")) return;
		printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
		printf_colored("> O arquivo de amostras de temperatura foi aberto corretamente!\r\n", COLOR_YELLOW);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_YELLOW);

		/* Realiza um take no semáforo para garantir que ele ficará indisponível */
		xSemaphoreTake(xBinarySemaphore, 0);

		/*Cria a Task Handler*/
		xTaskCreate(vTaskDisplay_av7, "TaskDisplay", 1000, NULL, 2, NULL);
		printf_colored("---------------------------------------------------------------\r\n", COLOR_GREEN);
		printf_colored(">> Task \"Display\" Criada!\r\n", COLOR_GREEN);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_GREEN);

		/*Cria a Task Periódica*/
		xTaskCreate(vTaskReadSensor_av7, "TaskReadSensor", 1000, NULL, 1, NULL);
		printf_colored("---------------------------------------------------------------\r\n", COLOR_CYAN);
		printf_colored(">> Task Periódica \"Read Sensor\" Criada!\r\n", COLOR_CYAN);
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

// Task para exibição da amostra lida pela task de leitura de temperatura
void vTaskDisplay_av7(void* pvParameters)
{
	for (;;)
	{
		//printf_colored("Task Display tenta adquirir o semáforo...\n", COLOR_GREEN);
		// portMAX_DELAY indica bloqueio infinito até obter o semáforo
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		printf_colored("Task Display está processando a amostra obtida!\n", COLOR_GREEN);
		printf("> Temperatura lida: %.02f °C\n", temperatureSampleValue);
	}
}

// Task para leitura de amostras de temperatura presentes no arquivo de dados
void vTaskReadSensor_av7(void* pvParameters)
{
	for (;;)
	{
		printf_colored("\nTask Periódica Read Sensor está executando...\n", COLOR_CYAN);

		if (fgets(temperatureSampleStr, STRSIZE, fp) == NULL)
		{
			printf_colored("\n------------------------------------------------------\r\n", COLOR_RED);
			printf_colored(">> Todas as amostras de temperatura foram processadas!\r\n", COLOR_RED);
			printf_colored("------------------------------------------------------\r\n\n", COLOR_RED);

			fclose(fp);
			vTaskDelete(NULL); /* O que acontece aqui?*/
		}
		/*Conversão da amostra lida de string para double*/
		temperatureSampleValue = atof(temperatureSampleStr);
			
		/*Impôe um intervalo entre as leituras das amostras de temperatura do arquivo*/
		vTaskDelay(TEMP_SAMPLE_READING_INTERVAL / portTICK_RATE_MS);

		//printf_colored("Task Periódica Read Sensor irá liberar o semáforo!\n", COLOR_CYAN);
		/*Liberação do semáforo binário*/
		xSemaphoreGive(xBinarySemaphore);
	}
}

//Função para leitura do arquivo de dados
int openFileToRead(char* filename, const char* readMode)
{
	if ((fp = fopen(filename, readMode)) == NULL) {
		printf_colored("\n------------------------------------------------\r\n", COLOR_RED);
		printf("> Arquivo \"%s\" não localizado", filename);
		printf_colored("\n------------------------------------------------\r\n\n\n", COLOR_RED);
		
		return 1;
	}

	return 0;
}