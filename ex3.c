/* Inlcudes Padrão C */
#include <stdio.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"

/* Protótipo das funçoes das Tasks*/
void vTask_ex3(void *pvParameters);

void main_ex3(void)
{
	/*Cria a Task 1*/
	xTaskCreate(vTask_ex3, "Task1", 1000, "Task 1", 1, NULL);
	printf("Task 1 Criada!\r\n\n");

	/*Cria a Task 2*/
	xTaskCreate(vTask_ex3, "Task2", 1000, "Task 2", 1, NULL);
	printf("Task 2 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}

void vTask_ex3(void *pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	char *pcTaskName;

	pcTaskName = (char *)pvParameters;

	printf("%s executa pela primeira vez!\r\n\n", pcTaskName);

	for (;;)
	{
		printf("%s está executando!\r\n", pcTaskName);

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