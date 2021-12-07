/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>
#include <string.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"

/* Protótipo das funçoes das Tasks */
void PrintTask_ex27(void* pvParameters);

/* Protótipo da função para imprimir no terminal */
void PrintString_ex27(const char* pcString);

void main_ex27(void)
{
	/*Cria a Task Printer 1 */
	xTaskCreate(PrintTask_ex27, "Print1", 1000, "1", 1, NULL);
	printf("Task Printer 1 Criada!\r\n\n");

	/*Cria a Task Printer 1 */
	xTaskCreate(PrintTask_ex27, "Print2", 1000, "2", 1, NULL);
	printf("Task Printer 2 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}


static void PrintTask_ex27(void* pvParameters)
{
	char* StringToPrint;

	StringToPrint = (char*)pvParameters;
	for (;; )
	{

		PrintString_ex27(StringToPrint);

		vTaskDelay(2);
	}
}

static void PrintString_ex27(const char* pcString)
{
	unsigned int ui;
	unsigned long ul;

	taskDISABLE_INTERRUPTS();
	//taskENTER_CRITICAL();
	//vTaskSuspendAll();
	for (ui = 0; ui < 100; ui++)
	{
		if (strcmp(pcString, "1"))
			printf_colored(pcString, COLOR_GREEN);
		else
			printf_colored(pcString, COLOR_CYAN);

		for (ul = 0; ul < 10000000; ul++)
		{

		}
	}
	printf("\r\n\r\n\r\n");
	//xTaskResumeAll();
	//taskEXIT_CRITICAL();
	taskENABLE_INTERRUPTS();
}