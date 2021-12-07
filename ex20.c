/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* O n�mero da interrup��o de software usada neste exemplo. 
O c�digo mostrado � do projeto do Windows, onde os n�meros 
de 0 a 2 s�o usados pelo port do FreeRTOS para Windows, 
portanto, 3 � o primeiro n�mero dispon�vel para a aplica��o. */
#define mainINTERRUPT_NUMBER 3

/* Prot�tipo das fun�oes das Tasks*/
static void vPeriodicTask_ex20(void* pvParameters);
static void vHandlerTask_ex20(void* pvParameters);

/* Prot�tipo da Rotina de Tratamento de Interrup��o (ISR - Interrupt Service Routine)*/
static uint32_t ulExampleInterruptHandler_ex20(void);

/* Handle para o sem�foro bin�rio */
xSemaphoreHandle xBinarySemaphore;

void main_ex20(void)
{
	/* Cria o Sem�foro Bin�rio */
	xBinarySemaphore = xSemaphoreCreateBinary();

	if (xBinarySemaphore != NULL)
	{

		/* Cria a task Handler, respons�vel por processar o evento gerado pela interrup��o */
		xTaskCreate(vHandlerTask_ex20, "Handler", 1000, NULL, 1, NULL);
		printf("Task Handler criada!\r\n\n");

		/* Cria a task Peridic respons�vel por periodicamente gerar uma interrup��o */
		xTaskCreate(vPeriodicTask_ex20, "Periodic", 1000, NULL, 3, NULL);
		printf("Task Peri�dica criada!\r\n\n");

		/* Realiza a instala��o do Handler para tratar a interrup��o */
		vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler_ex20);
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

static void vPeriodicTask_ex20(void* pvParameters)
{
	/* Vari�vel para definir o tempo de delay */
	const TickType_t xDelay500ms = pdMS_TO_TICKS(500);

	for (;; )
	{
		printf_colored("Task Peri�dica vai pedir delay\r\n", COLOR_GREEN);

		vTaskDelay(xDelay500ms);

		printf_colored("Task Peri�dica vai gerar uma interrup��o\r\n", COLOR_GREEN);
		vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
		printf_colored("Task Peri�dica gerou uma interrup��o\r\n", COLOR_GREEN);
	}
}

static void vHandlerTask_ex20(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	for (;; )
	{
		printf_colored("Task Handler tenta adquirir o sem�foro...\n", COLOR_CYAN);
		
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		
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


static uint32_t ulExampleInterruptHandler_ex20(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	printf_colored("ISR processando o evento!\n", COLOR_MAGENTA);

	xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);

	printf_colored("ISR liberou o sem�foro!\n", COLOR_MAGENTA);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}