/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Define relacionado com a Interrup��o */
#define mainINTERRUPT_NUMBER 3

/* Prot�tipo das fun�oes das Tasks*/

static void vPeriodicTask_ex21(void* pvParameters);
static void vHandlerTask_ex21(void* pvParameters);

/* Prot�tipo da Rotina de Tratamento de Interrup��o (ISR - Interrupt Service Routine)*/
static uint32_t ulExampleInterruptHandler_ex21(void);

/* Handle para o sem�foro Contador */
xSemaphoreHandle xCountingSemaphore;

void main_ex21(void)
{
	/* Cria o Sem�foro Contador inicializado com 0 e limite de 10 */
	xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

	if (xCountingSemaphore != NULL)
	{

		/* Cria a task Handler, respons�vel por processar o evento gerado pela interrup��o */
		xTaskCreate(vHandlerTask_ex21, "Handler", 1000, NULL, 3, NULL);
		printf("Task Handler criada!\r\n\n");

		/* Cria a task Peridic respons�vel por periodicamente gerar uma interrup��o */
		xTaskCreate(vPeriodicTask_ex21, "Periodic", 1000, NULL, 1, NULL);
		printf("Task Peri�dica criada!\r\n\n");

		/* Realiza a instala��o do Handler para tratar a interrup��o */
		vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler_ex21);
		printf("Handler para interrup��o instalado!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
	else
	{
		printf("O Sem�foro n�o foi criado corretamente.\r\n");
	}
}

static void vPeriodicTask_ex21(void* pvParameters)
{
	/* Vari�vel para definir o tempo de delay */
	const TickType_t xDelay500ms = pdMS_TO_TICKS(500);

	for (;; )
	{
		vTaskDelay(xDelay500ms);

		printf_colored("Task Peri�dica vai gerar uma interrup��o\r\n", COLOR_GREEN);
		vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
		printf_colored("Task Peri�dica gerou uma interrup��o\r\n", COLOR_GREEN);
	}
}

static void vHandlerTask_ex21(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	for (;; )
	{
		printf_colored("Task Handler tenta adquirir o sem�foro...\n", COLOR_CYAN);

		xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

		printf_colored("Task Handler est� processando o evento!\n", COLOR_CYAN);

		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 2; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}
	}
}


static uint32_t ulExampleInterruptHandler_ex21(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	printf_colored("ISR processando o evento!\n", COLOR_MAGENTA);

	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);

	printf_colored("ISR liberou o sem�foro contador 3 vezes!\n", COLOR_MAGENTA);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}