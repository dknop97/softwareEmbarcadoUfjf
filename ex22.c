/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Define relacionado com a Interrupção */
#define mainINTERRUPT_NUMBER 3

/* Protótipo das funçoes das Tasks*/
static void vIntegerGenerator_ex22(void* pvParameters);
static void vStringPrinter_ex22(void* pvParameters);

/* Protótipo da Rotina de Tratamento de Interrupção (ISR - Interrupt Service Routine)*/
static uint32_t ulExampleInterruptHandler_ex22(void);

/* Handle para as Filas de Mensagens */
xQueueHandle xIntegerQueue, xStringQueue;

void main_ex22(void)
{

	/* Cria a fila de mensagem com capacidade para 10 inteiros de 32 bits */
	xIntegerQueue = xQueueCreate(10, sizeof(uint32_t));
	/* Cria a fila de mensagem com capacidade para 10 strings */
	xStringQueue = xQueueCreate(10, sizeof(char*));

	if (xIntegerQueue != NULL && xStringQueue != NULL)
	{
		/* Cria a task geradora de números inteiros */
		xTaskCreate(vIntegerGenerator_ex22, "IntGen", 1000, NULL, 1, NULL);
		printf("Task geradora de números inteiros criada!\r\n\n");

		/* Cria a task para impressão de strings */
		xTaskCreate(vStringPrinter_ex22, "String", 1000, NULL, 2, NULL);
		printf("Task para impressão de strings criada!\r\n\n");

		/* Realiza a instalação do Handler para tratar a interrupção */
		vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler_ex22);
		printf("Handler para interrupção instalado!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
	else
	{
		printf("Erro na criação das Filas de Mensagens.\r\n");
	}
}

static void vIntegerGenerator_ex22(void* pvParameters)
{
	TickType_t xLastExecutionTime;
	uint32_t ulValueToSend = 0;
	int i;
	xLastExecutionTime = xTaskGetTickCount();
	for (;; )
	{
		/* Chamada do delay para controlar a periodicidade da task */
		vTaskDelayUntil(&xLastExecutionTime, pdMS_TO_TICKS(200));

		/* Escreve 5 números inteiros na Fila de Mensagens de números inteiros */
		for (i = 0; i < 5; i++)
		{
			printf_colored("Task Geradora de Inteiros vai escrever um valor na fila\r\n", COLOR_GREEN);
			xQueueSendToBack(xIntegerQueue, &ulValueToSend, 0);
			ulValueToSend++;
		}

		printf_colored("Task Geradora de Inteiros vai gerar uma interrupção\r\n", COLOR_GREEN);
		vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
	}
}

static uint32_t ulExampleInterruptHandler_ex22(void)
{
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t ulReceivedNumber;

	/*Vetor de Strings para relizar o mapeamento de acordo com o inteiro recebido */
	static const char* pcStrings[] =
	{
	"String 0\r\n",
	"String 1\r\n",
	"String 2\r\n",
	"String 3\r\n"
	};

	xHigherPriorityTaskWoken = pdFALSE;

	/* Enquanto tiver dado na fila dos inteiros, realiza a leitura e escreve a string correspondente na fila de strings*/
	while (xQueueReceiveFromISR(xIntegerQueue, &ulReceivedNumber, &xHigherPriorityTaskWoken) != errQUEUE_EMPTY)
	{
		printf_colored("Recebendo inteiro e escrevendo string!\r\n", COLOR_MAGENTA);
		ulReceivedNumber &= 0x03;
		xQueueSendToBackFromISR(xStringQueue, &pcStrings[ulReceivedNumber], &xHigherPriorityTaskWoken);
	}

	printf_colored("ISR finalizando execução!\n", COLOR_MAGENTA);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void vStringPrinter_ex22(void* pvParameters)
{
	char* pcString;
	for (;; )
	{

		xQueueReceive(xStringQueue, &pcString, portMAX_DELAY);

		printf_colored(pcString, COLOR_CYAN);
	}
}