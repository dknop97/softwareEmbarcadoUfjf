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
void vTask1_ex12(void* pvParameters);
void vTask2_ex12(void* pvParameters);

/* Handle para o Semáforo Contador */
xSemaphoreHandle xCountingSemaphore;

void main_ex12(void)
{
	/*Cria o Semáforo Contador - Inicializado com 0 */
	xCountingSemaphore = xSemaphoreCreateCounting(10, 0);


	/* Testa se o semáforo foi criado corretamente */
	if (xCountingSemaphore != NULL)
	{
		/*Cria a Task Handler*/
		xTaskCreate(vTask1_ex12, "Task1", 1000, NULL, 3, NULL);
		printf("Task Handler Criada!\r\n\n");

		/*Cria a Task Periódica*/
		xTaskCreate(vTask2_ex12, "Task2", 1000, NULL, 1, NULL);
		printf("Task Periódica Criada!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}

	
}

void vTask1_ex12(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	for (;;)
	{
		printf_colored("Task Handler tenta adquirir o semáforo...\n", COLOR_GREEN);

		xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

		printf_colored("Task Handler está processando o evento!\n", COLOR_GREEN);

		/*Este loop for é utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 2; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}
	}
}


void vTask2_ex12(void* pvParameters)
{
	for (;;)
	{
		printf_colored("Task Periódica está executando...\n", COLOR_CYAN);

		vTaskDelay(500 / portTICK_RATE_MS);

		printf_colored("Task Periódica irá liberar o semáforo!\n", COLOR_CYAN);

		xSemaphoreGive(xCountingSemaphore);
		xSemaphoreGive(xCountingSemaphore);
		xSemaphoreGive(xCountingSemaphore);
	}
}