/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Protótipo das funçoes das Tasks*/
void vTask1_ex15(void* pvParameters); // Remetente
void vTask2_ex15(void* pvParameters); // Destinatário

/* Handle para a Fila de Mensagens */
xQueueHandle xQueue;

void main_ex15(void)
{
	/* Cria a fila com tamanho 5 e com capacidade para variaveis do tipo long */
	xQueue = xQueueCreate(5, sizeof(long)); 

	/* Testa se a Fila de Mensagens foi criada corretamente */
	if (xQueue != NULL)
	{
		/* Cria a Task Remetente 1 */
		xTaskCreate(vTask1_ex15, "Remetente 1", 1000, (void*)100, 3, NULL); // Cria uma instancia da task sender que enviara o parametro 100.
		printf("Remetente 1 criado!\r\n\n");

		/* Cria a Task Remetente 2 */
		xTaskCreate(vTask1_ex15, "Remetente 2", 1000, (void*)200, 3, NULL); // Cria uma instancia da task sender que enviara o parametro 200.
		printf("Remetente 2 criado!\r\n\n");

		/* Cria a Task Destinatário */
		xTaskCreate(vTask2_ex15, "Destinatário", 1000, NULL, 2, NULL); // Cria uma instancia da task receiver.
		printf("Destinatário criado!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
	else
	{
		printf("A Fila de Mensagens não pode ser criada.\r\n");
	}
}

void vTask1_ex15(void* pvParameters)
{
	long lValueToSend;
	portBASE_TYPE xStatus;

	char str[80];
	char str2[80];
	
	lValueToSend = (long)pvParameters; // Parametro a ser enviado

	sprintf(str, "Tentando enviar valor %d para fila...\r\n", lValueToSend);

	for (;;)
	{
		printf_colored(str, COLOR_GREEN);
		xStatus = xQueueSendToBack(xQueue, &lValueToSend, portMAX_DELAY); // A opcao e por nao bloquear.
		sprintf(str2, "Valor %d Enviado!!\r\n", lValueToSend);
		printf_colored(str2, COLOR_YELLOW);
		if (xStatus != pdPASS)
		{
			printf_colored("Não foi possível enviar mensagem.\n", COLOR_RED);
		}

		taskYIELD(); // Forca o scheduler a trocar para outra task
	}
}


void vTask2_ex15(void* pvParameters)
{
	long lReceivedValue;
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	char str[80];

	for (;;)
	{
		printf_colored("Destinatário tenta ler mensagem na fila.\n", COLOR_CYAN);
		xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			sprintf(str, "Valor %d recebido da fila\r\n", lReceivedValue);
			printf_colored(str, COLOR_CYAN);
		}
		else
		{
			printf_colored("Ocorreu algum erro na recepção!\r\n", COLOR_RED);
		}
	}
}