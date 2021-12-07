/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"

/* Protótipo das funçoes das Tasks*/
void vTask1_ex5(void *pvParameters);
void vTask2_ex5(void *pvParameters);

void main_ex5(void)
{
	/*Cria a Task 1*/
	xTaskCreate(vTask1_ex5, "Task1", 1000, NULL, 2, NULL);
	printf("Task 1 Criada!\r\n\n");

	/*Cria a Task 2*/
	xTaskCreate(vTask2_ex5, "Task2", 1000, NULL, 1, NULL);
	printf("Task 2 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}

void vTask1_ex5(void *pvParameters)
{
	printf_colored("Task 1 executa pela primeira vez!\n", COLOR_GREEN);

	for (;;)
	{
		printf_colored("Task 1 está executando!\n", COLOR_GREEN);

		/*vTaskDelay coloca a task no estado BLOCKED*/
		vTaskDelay(100 / portTICK_RATE_MS);

	}
}


void vTask2_ex5(void *pvParameters)
{		
	unsigned int ui;
	unsigned long ul;

	printf_colored("Task 2 executa pela primeira vez!\n", COLOR_CYAN);

	for (;;)
	{
		printf_colored("Task 2 está executando!\n", COLOR_CYAN);

		/*Este loop for é utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}
	}
}