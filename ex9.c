/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"

/* Declara��o dos Handles*/
xTaskHandle xTask1_ex9Handle;
xTaskHandle xTask2_ex9Handle;

/* Prot�tipo das fun�oes das Tasks*/
void vTask1_ex9(void *pvParameters);
void vTask2_ex9(void *pvParameters);

void main_ex9(void)
{
	/*Cria a Task 1*/
	xTaskCreate(vTask1_ex9, "Task1", 1000, NULL, 3, &xTask1_ex9Handle);
	printf("Task 1 Criada!\r\n\n");

	/*Cria a Task 2*/
	xTaskCreate(vTask2_ex9, "Task2", 1000, NULL, 2, &xTask2_ex9Handle);
	printf("Task 2 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}


void vTask1_ex9(void *pvParameters)
{

	unsigned int ui;
	unsigned long ul;

	int count = 0;
	unsigned portBASE_TYPE uxPriority;

	printf_colored("Task 1 executa pela primeira vez!\n", COLOR_GREEN);

	uxPriority = uxTaskPriorityGet(NULL);

	for (;;)
	{
		printf_colored("Task 1 est� executando!\n", COLOR_GREEN);

		if (++count == 5)
		{
			printf_colored("\nDiminuindo a prioridade da Task 1!!!\r\n\n", COLOR_RED);

			vTaskPrioritySet(NULL, (uxPriority - 1));
		}

		if (count == 10)
		{
			printf_colored("\nDiminuindo a prioridade da Task 1!!!\r\n\n", COLOR_RED);

			vTaskPrioritySet(NULL, (uxPriority - 2));
		}

		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}
	}
}

void vTask2_ex9(void *pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	printf_colored("Task 2 executa pela primeira vez!\n", COLOR_CYAN);

	for (;;)
	{
		printf_colored("Task 2 est� executando!\n", COLOR_CYAN);

		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}

	}
}