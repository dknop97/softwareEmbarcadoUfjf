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
void vSenderTask1_ex19(void* pvParameters);
void vSenderTask2_ex19(void* pvParameters);
void vReceiverTask_ex19(void* pvParameters);

/* Handles para as duas filas */
xQueueHandle xQueue1 = NULL, xQueue2 = NULL;

/* Handle para o QueueSet */
static QueueSetHandle_t xQueueSet = NULL;
void main_ex19(void)
{
	/* Cria a Fila de Mensagens 1 */
	xQueue1 = xQueueCreate(1, sizeof(char*));
	
	/* Cria a Fila de Mensagens 2 */
	xQueue2 = xQueueCreate(1, sizeof(char*));

	/* Cria o QueueSet */
	xQueueSet = xQueueCreateSet(1 * 2);

	if (xQueue1 == NULL || xQueue2 == NULL || xQueueSet == NULL)
	{
		printf("Os objetos não puderam ser criados corretamente.\r\n");
	}
	else
	{
		/* Adiciona a Queue1 no QueueSet */
		xQueueAddToSet(xQueue1, xQueueSet);

		/* Adiciona a Queue2 no QueueSet */
		xQueueAddToSet(xQueue2, xQueueSet);

		/* Cria a Task Remetente 1 */
		xTaskCreate(vSenderTask1_ex19, "Remetente 1", 1000, NULL, 1, NULL);
		printf("Remetente 1 criado!\r\n\n");

		/* Cria a Task Remetente 2 */
		xTaskCreate(vSenderTask2_ex19, "Remetente 2", 1000, NULL, 1, NULL);
		printf("Remetente 2 criado!\r\n\n");

		/* Cria a Task Destinatário */
		xTaskCreate(vReceiverTask_ex19, "Destinatário", 1000, NULL, 2, NULL);
		printf("Destinatário criado!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		while (1);
	}
}

void vSenderTask1_ex19(void* pvParameters)
{
	/* Número de Ticks para bloqueio por delay */
	const TickType_t xBlockTime = pdMS_TO_TICKS(100);

	/* Mensagem a ser enviada */
	const char* const pcMessage = "Mensagem do Remetente 1\r\n";

	for (;; )
	{
		/* Bloqueio no Delay */
		vTaskDelay(xBlockTime);

		/*Envia Mensagem para Fila*/
		printf_colored("Remetente 1 enviando mensagem para a Queue 1\r\n", COLOR_GREEN);
		xQueueSend(xQueue1, &pcMessage, 0);
	}
}


void vSenderTask2_ex19(void* pvParameters)
{
	/* Número de Ticks para bloqueio por delay */
	const TickType_t xBlockTime = pdMS_TO_TICKS(100);

	/* Mensagem a ser enviada */
	const char* const pcMessage = "Mensagem do Remetente 2\r\n";

	for (;; )
	{
		/* Bloqueio no Delay */
		vTaskDelay(xBlockTime);

		/*Envia Mensagem para Fila*/
		printf_colored("Remetente 2 enviando mensagem para a Queue 2\r\n", COLOR_GREEN);
		xQueueSend(xQueue2, &pcMessage, 0);
	}
}


void vReceiverTask_ex19(void* pvParameters)
{
	/* Handle para verificar qual fila contém mensagens */
	xQueueHandle xQueueThatContainsData;

	/* Auxiliar para receber a mensagem */
	char* pcReceivedString;

	char str[80];

	for (;; )
	{
		/*Bloqueia a espera de mensagens em qualquer fila do QueueSet */
		xQueueThatContainsData = (xQueueHandle)xQueueSelectFromSet(xQueueSet, portMAX_DELAY);

		/* Recebe a mensagem da fila que contém mensagens */
		xQueueReceive(xQueueThatContainsData, &pcReceivedString, 0);

		/* Imprime a mensagem recebida e de qual fila foi */
		sprintf(str,"String recebida da Queue %d: %s\r\n", (xQueueThatContainsData == xQueue1) ? 1 : 2 ,pcReceivedString);
		printf_colored(str, COLOR_CYAN);
	}
}