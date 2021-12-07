/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Prot�tipo das fun�oes das Tasks*/
void vTask1_ex11(void* pvParameters);
void vTask2_ex11(void* pvParameters);

/* Handle para o Sem�foro Bin�rio */
xSemaphoreHandle xBinarySemaphore;

void main_ex11(void)
{
	/*Cria o Sem�foro Bin�rio*/
	vSemaphoreCreateBinary(xBinarySemaphore);

	/* Testa se o sem�foro foi criado corretamente */
	if (xBinarySemaphore != NULL)
	{
		
		/* Realiza um take no sem�foro para garantir que ele ficar� indispon�vel */
		xSemaphoreTake(xBinarySemaphore, 0);

		/*Cria a Task Handler*/
		xTaskCreate(vTask1_ex11, "Task1", 1000, NULL, 3, NULL);
		printf("Task Handler Criada!\r\n\n");

		/*Cria a Task Peri�dica*/
		xTaskCreate(vTask2_ex11, "Task2", 1000, NULL, 1, NULL);
		printf("Task Peri�dica Criada!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}	
}

void vTask1_ex11(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;
	
	for (;;)
	{
		printf_colored("Task Handler tenta adquirir o sem�foro...\n", COLOR_GREEN);
		// portMAX_DELAY indica bloqueio infinito at� obter o sem�foro
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		// Se quis�ssemos passar um tempo limite, devemos passar como 
		// par�metro a quantidade de TICKS desejada

		printf_colored("Task Handler est� processando o evento!\n", COLOR_GREEN);
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


void vTask2_ex11(void* pvParameters)
{
	for (;;)
	{
		printf_colored("Task Peri�dica est� executando...\n", COLOR_CYAN);

		vTaskDelay(500 / portTICK_RATE_MS);

		printf_colored("Task Peri�dica ir� liberar o sem�foro!\n", COLOR_CYAN);
		xSemaphoreGive(xBinarySemaphore);

	}
}