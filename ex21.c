/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Define relacionado com a Interrupção */
#define mainINTERRUPT_NUMBER 3

/* Protótipo das funçoes das Tasks*/

static void vPeriodicTask_ex21(void* pvParameters);
static void vHandlerTask_ex21(void* pvParameters);

/* Protótipo da Rotina de Tratamento de Interrupção (ISR - Interrupt Service Routine)*/
static uint32_t ulExampleInterruptHandler_ex21(void);

/* Handle para o semáforo Contador */
xSemaphoreHandle xCountingSemaphore;

void main_ex21(void)
{
	/* Cria o Semáforo Contador inicializado com 0 e limite de 10 */
	xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

	if (xCountingSemaphore != NULL)
	{

		/* Cria a task Handler, responsável por processar o evento gerado pela interrupção */
		xTaskCreate(vHandlerTask_ex21, "Handler", 1000, NULL, 3, NULL);
		printf("Task Handler criada!\r\n\n");

		/* Cria a task Peridic responsável por periodicamente gerar uma interrupção */
		xTaskCreate(vPeriodicTask_ex21, "Periodic", 1000, NULL, 1, NULL);
		printf("Task Periódica criada!\r\n\n");

		/* Realiza a instalação do Handler para tratar a interrupção */
		vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler_ex21);
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
		printf("O Semáforo não foi criado corretamente.\r\n");
	}
}

static void vPeriodicTask_ex21(void* pvParameters)
{
	/* Variável para definir o tempo de delay */
	const TickType_t xDelay500ms = pdMS_TO_TICKS(500);

	for (;; )
	{
		vTaskDelay(xDelay500ms);

		printf_colored("Task Periódica vai gerar uma interrupção\r\n", COLOR_GREEN);
		vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
		printf_colored("Task Periódica gerou uma interrupção\r\n", COLOR_GREEN);
	}
}

static void vHandlerTask_ex21(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	for (;; )
	{
		printf_colored("Task Handler tenta adquirir o semáforo...\n", COLOR_CYAN);

		xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

		printf_colored("Task Handler está processando o evento!\n", COLOR_CYAN);

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


static uint32_t ulExampleInterruptHandler_ex21(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	printf_colored("ISR processando o evento!\n", COLOR_MAGENTA);

	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
	xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);

	printf_colored("ISR liberou o semáforo contador 3 vezes!\n", COLOR_MAGENTA);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}