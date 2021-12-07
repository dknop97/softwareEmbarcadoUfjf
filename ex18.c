/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

/* Protótipo das funçoes das Tasks */
void vTask1_ex18(void* pvParameters); 
void vTask2_ex18(void* pvParameters); 

/* Handle para o StreamBuffer */
StreamBufferHandle_t xStreamBuffer;

/* Parâmetros do StreamBuffer */
const size_t xStreamBufferSizeBytes = 100, xTriggerLevel = 10;

void main_ex18(void)
{
	/* Criação do StreamBuffer*/
	xStreamBuffer = xStreamBufferCreate(xStreamBufferSizeBytes, xTriggerLevel);

	if (xStreamBuffer != NULL)
	{
		/* Cria a Task Remetente 1 */
		xTaskCreate(vTask1_ex18, "Remetente 1", 1000, "Mensagem1", 1, NULL); // Cria uma instancia da task sender que enviara o parametro 100.
		printf("Remetente 1 criado!\r\n\n");

		xTaskCreate(vTask1_ex18, "Remetente 2", 1000, "Mensagem2", 1, NULL); // Cria uma instancia da task sender que enviara o parametro 200.
		printf("Remetente 2 criado!\r\n\n");

		xTaskCreate(vTask2_ex18, "Destinatário", 1000, NULL, 2, NULL); // Cria uma instancia da task receiver.
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
		printf("O Buffer não pode ser criado.\r\n");
	}
}

void vTask1_ex18(void* pvParameters)
{
	/* Variável para verificar quantos bytes foram enviados */
	size_t xBytesSent;

	/* Informação a ser enviada, definida nos parâmetros de criação da task */
	char* pcStringToSend = (char*)pvParameters;

	/* Tempo de bloqueio caso o StreamBuffer esteja cheio */
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

	/* Auxiliar */
	char str[80];

	for (;;)
	{
		/* Envia os dados pelo StreamBuffer */
		xBytesSent = xStreamBufferSend(xStreamBuffer, (void*)pcStringToSend, strlen(pcStringToSend), xTicksToWait);

		sprintf(str, "%d bytes enviados\r\n", xBytesSent);
		printf_colored(str, COLOR_GREEN);
	
		if (xBytesSent == 0)
		{
			printf_colored("Erro no envio!!!.\r\n", COLOR_RED);
		}
	}
}


void vTask2_ex18(void* pvParameters)
{
	/* Variável para armazenar os dados recebidos */
	char ucRxData[50];

	/* Variável para verificar quantos bytes foram recebidos */
	size_t xReceivedBytes;

	/* Auxiliar */
	char str[80];

	for (;;)
	{
		xReceivedBytes = xStreamBufferReceive(xStreamBuffer, (void*)ucRxData, sizeof(ucRxData), portMAX_DELAY);
		ucRxData[xReceivedBytes] = '\0';

		sprintf(str,"Bytes Recebidos = %d\nString recebida: %s\n\n", xReceivedBytes, ucRxData);
		printf_colored(str, COLOR_CYAN);
	}
}