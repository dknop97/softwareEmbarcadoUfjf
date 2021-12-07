/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Protótipo das funçoes das Tasks*/
void vTaskL(void* pvParameters);
void vTaskM(void* pvParameters);
void vTaskH(void* pvParameters);

/* Handle para o Semáforo Contador */
SemaphoreHandle_t xMutex;

void main_ex14(void)
{

	/* Para utilizar o Mutex, descomente a linha abaixo */
	xMutex = xSemaphoreCreateMutex();

	/* Para utilizar o Semáforo Binário, descomente a linha abaixo */
	//vSemaphoreCreateBinary(xMutex);

	/* Testa se o semáforo foi criado corretamente */
	if (xMutex != NULL)
	{
		xTaskCreate(vTaskL, "TaskL", 1000, NULL, 1, NULL);
		printf("Task LOW Criada!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
}

void vTaskL(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	int control = 0;

	printf_colored("Task LOW executa pela primeira vez.\n", COLOR_GREEN);

	for (;;)
	{
		printf_colored("Task LOW tenta adiquirir o MUTEX.\n", COLOR_RED);
		xSemaphoreTake(xMutex, portMAX_DELAY);
		printf_colored("Task LOW conseguiu adiquirir o MUTEX.\n", COLOR_RED);

		if (control == 0)
		{
			printf("Task MEDIUM Criada!\r\n\n");
			xTaskCreate(vTaskM, "TaskM", 1000, NULL, 2, NULL);
			control = 1;
		}

		for (ui = 0; ui < 3; ui++)
		{
			printf_colored("Task LOW está executando.\n", COLOR_GREEN);
			for (ul = 0; ul < 10000000; ul++)
			{

			}
		}

		printf_colored("Task LOW libera o MUTEX.\n", COLOR_RED);
		xSemaphoreGive(xMutex);
	}
}

void vTaskM(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	int control = 0;

	printf_colored("Task MEDIUM executa pela primeira vez.\n", COLOR_CYAN);

	for (;;)
	{
		if (control == 0)
		{
			printf("Task HIGH Criada!\r\n\n");
			xTaskCreate(vTaskH, "Task HIGH", 1000, NULL, 3, NULL);
			control = 1;
		}


		for (ui = 0; ui < 3; ui++)
		{
			printf_colored("Task MEDIUM está executando.\n", COLOR_CYAN);
			for (ul = 0; ul < 10000000; ul++)
			{

			}
		}

		vTaskDelay(500 / portTICK_RATE_MS);
	}
}


void vTaskH(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	printf_colored("Task HIGH executa pela primeira vez.\n", COLOR_YELLOW);

	for (;;)
	{
		printf_colored("Task HIGH tenta adiquirir o MUTEX.\n", COLOR_RED);
		xSemaphoreTake(xMutex, portMAX_DELAY);
		printf_colored("Task HIGH consegue adiquirir o MUTEX.\n", COLOR_RED);

		for (ui = 0; ui < 3; ui++)
		{
			printf_colored("Task HIGH está executando.\n", COLOR_YELLOW);
			for (ul = 0; ul < 10000000; ul++)
			{

			}
		}
		printf_colored("Task HIGH libera o MUTEX.\n", COLOR_RED);
		xSemaphoreGive(xMutex);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}