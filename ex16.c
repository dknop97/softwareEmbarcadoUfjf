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
void vTask1_ex16(void* pvParameters); // Remetente
void vTask2_ex16(void* pvParameters); // Destinatário

/* Handle para a Fila de Mensagens */
xQueueHandle xQueue;

/* Define o tipo de dado a ser utilizado */
typedef struct
{
	unsigned char ucValue;
	unsigned char ucSource;
	char* Color;
} xData;

/* Declara duas váriaveis que serão enviadas para a fila */
static const xData xStructsToSend[2] =
{
	{ 100, 1 , COLOR_GREEN }, /* Used by Sender1. */
	{ 200, 2 , COLOR_MAGENTA } /* Used by Sender2. */
};

void main_ex16(void)
{
	/* Cria a fila com tamanho 3 e com capacidade para variaveis do tipo xData */
	xQueue = xQueueCreate(3, sizeof(xData));

	/* Testa se a Fila de Mensagens foi criada corretamente */
	if (xQueue != NULL)
	{
		/* Cria a Task Remetente 1 */
		xTaskCreate(vTask1_ex16, "Remetente 1", 1000, &(xStructsToSend[0]), 2, NULL); // Cria uma instancia da task sender que enviara o parametro 100.
		printf("Remetente 1 criado!\r\n\n");

		/* Cria a Task Remetente 2 */
		xTaskCreate(vTask1_ex16, "Remetente 2", 1000, &(xStructsToSend[1]), 2, NULL); // Cria uma instancia da task sender que enviara o parametro 200.
		printf("Remetente 2 criado!\r\n\n");

		/* Cria a Task Destinatário */
		xTaskCreate(vTask2_ex16, "Destinatário", 1000, NULL, 1, NULL); // Cria uma instancia da task receiver.
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

void vTask1_ex16(void* pvParameters)
{
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	xData xStructretoSend;

	xStructretoSend = *(xData*)pvParameters;

	char str[80];
	char str2[80];

	sprintf(str, "Remetente %d tenta enviar valor %d para fila...\r\n", xStructretoSend.ucSource, xStructretoSend.ucValue);

	for (;;)
	{

		printf_colored(str, xStructretoSend.Color);
		xStatus = xQueueSendToBack(xQueue, &xStructretoSend, xTicksToWait); 
		sprintf(str2, "Envio realizado pelo Remetente %d\r\n", xStructretoSend.ucSource);
		printf_colored(str2, xStructretoSend.Color);

		if (xStatus != pdPASS)
		{
			printf_colored("Não foi possível enviar mensagem.\n", COLOR_RED);
		}

		taskYIELD(); // Forca o scheduler a trocar para outra task
	}
}


void vTask2_ex16(void* pvParameters)
{
	xData xReceivedStructure;
	portBASE_TYPE xStatus;

	char str[80];

	for (;;)
	{
		printf_colored("Destinatário tenta ler uma mensagem da fila...\r\n", COLOR_CYAN);
		xStatus = xQueueReceive(xQueue, &xReceivedStructure, 0);

		if (xStatus == pdPASS)
		{
			sprintf(str, "Varlor %d recebido do Remetente %d\r\n", xReceivedStructure.ucValue, xReceivedStructure.ucSource);
			printf_colored(str, COLOR_CYAN );
		}
		else
		{
			printf_colored("Ocorreu algum erro na recepção!\r\n", COLOR_RED);
		}
	}
}