/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>
#include <string.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Protótipo das funçoes das Tasks */
void PrintTask_ex28(void* pvParameters);
void GateKeeperTask_ex28(void* pvParameters);

/* Protótipo da função para imprimir no terminal */
void PrintString_ex28(const char* pcString);

/* Handle para a Fila de Mensagens */
xQueueHandle xQueue;


void main_ex28(void)
{
	/* Cria a fila com tamanho 5 e com capacidade para variaveis do tipo long */
	xQueue = xQueueCreate(5, sizeof(char *));

	/* Testa se a Fila de Mensagens foi criada corretamente */
	if (xQueue != NULL)
	{
		/*Cria a Task Printer 1 */
		xTaskCreate(PrintTask_ex28, "Print1", 1000, "1", 1, NULL);
		printf("Task Printer 1 Criada!\r\n\n");

		/*Cria a Task Printer 1 */
		xTaskCreate(PrintTask_ex28, "Print2", 1000, "2", 1, NULL);
		printf("Task Printer 2 Criada!\r\n\n");

		/*Cria a Task Printer 1 */
		xTaskCreate(GateKeeperTask_ex28, "GateKeeper", 1000, NULL, 2, NULL);
		printf("Task GateKeeperTask Criada!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
}


static void PrintTask_ex28(void* pvParameters)
{
	char* StringToPrint;

	StringToPrint = (char*)pvParameters;
	for (;;)
	{

		xQueueSendToBack(xQueue, &StringToPrint, 0);

		vTaskDelay(2);
	}
}

static void GateKeeperTask_ex28(void* pvParameters) 
{
	char* pcMessageToPrint;

	for(;;)
	{
		xQueueReceive(xQueue, &pcMessageToPrint, portMAX_DELAY);
		PrintString_ex28(pcMessageToPrint);
	}
}

static void PrintString_ex28(const char* pcString)
{
	unsigned int ui;
	unsigned long ul;

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
}