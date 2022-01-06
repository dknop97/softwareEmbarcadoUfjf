
// TASK DISPLAY ATUA (MAIOR PRIORIDADE) E SE BLOQUEIA TENTANDO OBTER MUTEX
		// TASK DE LEITURA ATUA (PRIORIDADE INTERMEDIÁRIA), OBTEM VA, VB e VC; ADICIONA ELAS NAS FILAS; LANÇA UM YELD
		// ENTRA A TASK RMS (MENOR PRIORIDADE)
		//		- LÊ DAS FILAS E CALCULA O RMS ATÉ LER A, B e C
		//		- GIVE MUTEX
		//	DISPLAY ATUA, POIS MUTEX LIBERADO; EXIBE E SE BLOQUEIA TENTANDO OBTER O MUTEX NOVAMENTE
		// O CICLO SE REPETE

/* Includes Padrão C */
#include <stdio.h>
#include <math.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

FILE* fpVA;
FILE* fpVB;
FILE* fpVC;

int N = 16; /*Para o cálculo do valor RMS*/
char endFileFlag = 0;
float vA, vB, vC, vA_RMS, vB_RMS, vC_RMS;

void vTask1_S9(void* pvParameters);
void vTaskDisplay_S9(void* pvParameters);
void vTaskCalcRMS_S9(void* pvParameters);

//float CalcRMS_S9(float valueToConvert);
float CalcRMS_S9(float *pvParameters);

/* Handle para o MUTEX */
//SemaphoreHandle_t xMutex;

/* Handle para o Semáforo Binário */
xSemaphoreHandle xBinarySemaphore;

/*Handle para a Fila de Mensagens*/
xQueueHandle xQueue_vA;
xQueueHandle xQueue_vB;
xQueueHandle xQueue_vC;

void main_S9(void)
{
	fpVA = fopen("src\\AV9\\VA.txt", "r");
	fpVB = fopen("src\\AV9\\VB.txt", "r");
	fpVC = fopen("src\\AV9\\VC.txt", "r");

	if ((fpVA == NULL) || (fpVB == NULL) || (fpVC == NULL))
	{
		printf_colored("---------------------------------------------------------------\r\n", COLOR_RED);
		printf_colored("> Os arquivos de amostras não foram encontrados!\r\n", COLOR_RED);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_RED);
		
		return 1;
	}

	printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
	printf_colored("> O arquivo de amostras de temperatura foi aberto corretamente!\r\n", COLOR_YELLOW);
	printf_colored("---------------------------------------------------------------\r\n\n", COLOR_YELLOW);

	/*Cria o MUTEX*/
	//xMutex = xSemaphoreCreateMutex();

	/*Cria o Semáforo Binário*/
	vSemaphoreCreateBinary(xBinarySemaphore);

	/* Testa se o MUTEX foi criado corretamente */
	//if (xMutex == NULL)
	if (xBinarySemaphore == NULL)
	{
		printf_colored("---------------------------------------------------------------\r\n", COLOR_RED);
		printf_colored("> Ocorreu um erro ao criar o MUTEX!!\r\n", COLOR_RED);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_RED);

		return 1;
	}
	//xSemaphoreTake(xMutex, portMAX_DELAY);
	xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

	/* Cria as filas com tamanho N e com capacidade para variaveis do tipo double */
	xQueue_vA = xQueueCreate(2, sizeof(float));
	xQueue_vB = xQueueCreate(2, sizeof(float));
	xQueue_vC = xQueueCreate(2, sizeof(float));

	if ((xQueue_vA == NULL) || (xQueue_vB == NULL) || (xQueue_vC == NULL))
	{
		printf_colored("---------------------------------------------------------------\r\n", COLOR_RED);
		printf_colored("> Ocorreu um erro ao criar as filas!!\r\n", COLOR_RED);
		printf_colored("---------------------------------------------------------------\r\n\n", COLOR_RED);

		return 1;
	}

	/*Cria a Task Display*/
	xTaskCreate(vTaskDisplay_S9, "TaskDisplay", 1000, NULL, 3, NULL);
	printf_colored("---------------------------------------------------------------\r\n", COLOR_GREEN);
	printf_colored(">> Task \"Display\" Criada!\r\n", COLOR_GREEN);
	printf_colored("---------------------------------------------------------------\r\n\n", COLOR_GREEN);
	
	/*Cria a Task Amostragem*/
	xTaskCreate(vTask1_S9, "TaskAmostragem", 1000, NULL, 2, NULL);
	printf_colored("---------------------------------------------------------------\r\n", COLOR_CYAN);
	printf_colored(">> Task \"Amostragem\" Criada!\r\n", COLOR_CYAN);
	printf_colored("---------------------------------------------------------------\r\n\n", COLOR_CYAN);

	/*Cria a Task calcRMS*/
	xTaskCreate(vTaskCalcRMS_S9, "TaskCalcRMS", 1000, NULL, 1, NULL);
	printf_colored("---------------------------------------------------------------\r\n", COLOR_YELLOW);
	printf_colored(">> Task \"Calcula RMS\" Criada!\r\n", COLOR_YELLOW);
	printf_colored("---------------------------------------------------------------\r\n\n", COLOR_YELLOW);

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	printf_colored("===============================================================\r\n", COLOR_RED);
	vTaskStartScheduler();

	while (1 && !endFileFlag);

	return;
}

// Task para exibição das amostras das tensões lidas e convertidas para RMS
void vTaskDisplay_S9(void* pvParameters)
{
	for (;;)
	{
		//printf_colored("\n Task DISPLAY executando\n\n", COLOR_YELLOW);
		if (endFileFlag) return;

		//xSemaphoreTake(xMutex, portMAX_DELAY);
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		//printf("\r\n\rTask DISPLAY: TAKE\n\r\n");
		if ((vA_RMS != -999) && (vB_RMS != -999) || (vC_RMS != -999)) {
			printf("\n> Amostras Coletadas:\n\r\t VA = %.3f \n\r\t VB = %.3f \n\r\t VC = %.3f \n\n", vA, vB, vC);
			printf("\n========================================\n> Cálculo RMS das últimas %d amostras:\n\r\t VA_RMS = %.3f \n\r\t VB_RMS = %.3f \n\r\t VC_RMS = %.3f \n========================================\n", N, vA_RMS, vB_RMS, vC_RMS);
		}
		else {
			printf("\n> Amostras Coletadas:\n\r\t VA = %.3f \n\r\t VB = %.3f \n\r\t VC = %.3f \n", vA, vB, vC);
			//printf("\n> Amostras Coletadas:\n\r\t VA = %.3f <-.-> VA_rms = %.3f \n\r\t VB = %.3f <-.-> VB_rms = %.3f \n\r\t VC = %.3f <-.-> VC_rms = %.3f\n\n", vA, vA_RMS, vB, vB_RMS, vC, vC_RMS);
		}
	}
}

void vTask1_S9(void* pvParameters)
{
	char str[16];
	char str2[16];
	portBASE_TYPE xStatus;

	for (;;)
	{
		//printf_colored("\n Task AMOSTRAGEM executando\n\n", COLOR_YELLOW);
		if (fgets(str, 16, fpVA) == NULL)
		{
			printf("Final do Arquivo!\r\n\n");
			fclose(fpVA);
			endFileFlag = 1;
			vTaskDelete(NULL);
		}
		vA = atof(str);

		if (fgets(str, 16, fpVB) == NULL)
		{
			printf("Final do Arquivo!\r\n\n");
			fclose(fpVB);
			endFileFlag = 1;
			vTaskDelete(NULL);
		}
		vB = atof(str);

		if (fgets(str, 16, fpVC) == NULL)
		{
			printf("Final do Arquivo!\r\n\n");
			fclose(fpVC);
			endFileFlag = 1;
			vTaskDelete(NULL);
		}
		vC = atof(str);

		xStatus = xQueueSendToBack(xQueue_vA, &vA, portMAX_DELAY); // A opcao e por nao bloquear.
		//sprintf(str2, "Valor %f Enviado para a fila xQueue_vA!!\r\n", vA);
		//printf_colored(str2, COLOR_CYAN);
		if (xStatus != pdPASS)
		{
			//printf_colored("Não foi possível enviar mensagem para a fila xQueue_vA.\n", COLOR_RED);
		}

		xStatus = xQueueSendToBack(xQueue_vB, &vB, portMAX_DELAY); // A opcao e por nao bloquear.
		//sprintf(str2, "Valor %f Enviado para a fila xQueue_vB!!\r\n", vB);
		//printf_colored(str2, COLOR_GREEN);
		if (xStatus != pdPASS)
		{
			//printf_colored("Não foi possível enviar mensagem para a fila xQueue_vB.\n", COLOR_RED);
		}

		xStatus = xQueueSendToBack(xQueue_vC, &vC, portMAX_DELAY); // A opcao e por nao bloquear.
		//sprintf(str2, "Valor %f Enviado para a fila xQueue_vC!!\r\n", vC);
		//printf_colored(str2, COLOR_MAGENTA);
		if (xStatus != pdPASS)
		{
			//printf_colored("Não foi possível enviar mensagem para a fila xQueue_vC.\n", COLOR_RED);
		}

		//taskYIELD(); // Forca o scheduler a trocar para outra task

		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

// Task para calcular o valor RMS da amostra de tensão informada
void vTaskCalcRMS_S9(void* pvParameters)
{
	float lReceivedValue;
	float buffer_vA[16];
	float buffer_vB[16];
	float buffer_vC[16];
	char rmsCalcFlag = 0;
	char str[80];
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	for (int i = 0; i >= 0; i++)
	{
		//printf_colored("\n Task RMS executando\n\n", COLOR_YELLOW);
		if (endFileFlag) {
			//printf("\r\n\rTask RMS: FINAL GIVE \n\r\n");
			xSemaphoreGive(xBinarySemaphore);
			//xSemaphoreGive(xMutex);
			//return;
		}

		//printf_colored("Destinatário tenta ler mensagem na fila xQueue_vA.\n", COLOR_CYAN);
		xStatus = xQueueReceive(xQueue_vA, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			//sprintf(str, "Valor %.3f recebido da fila xQueue_vA\r\n", lReceivedValue);
			//printf_colored(str, COLOR_CYAN);
			buffer_vA[i] = lReceivedValue;

			if (i == (N - 1)) {
				vA_RMS = CalcRMS_S9(&buffer_vA);
				rmsCalcFlag = 1;
			}
		}
		else
		{
			//printf_colored("Ocorreu algum erro na recepção da fila xQueue_vA!\r\n", COLOR_RED);
		}
		// -------------------------
		//printf_colored("Destinatário tenta ler mensagem na fila xQueue_vB.\n", COLOR_GREEN);
		xStatus = xQueueReceive(xQueue_vB, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			//sprintf(str, "Valor %.3f recebido da fila xQueue_vB\r\n", lReceivedValue);
			//printf_colored(str, COLOR_GREEN);
			buffer_vB[i] = lReceivedValue;

			if (i == (N - 1)) {
				vB_RMS = CalcRMS_S9(&buffer_vB);
				rmsCalcFlag = 1;
			}
		}
		else
		{
			//printf_colored("Ocorreu algum erro na recepção da fila xQueue_vB!\r\n", COLOR_RED);
		}
		// -------------------------
		//printf_colored("Destinatário tenta ler mensagem na fila xQueue_vC.\n", COLOR_MAGENTA);
		xStatus = xQueueReceive(xQueue_vC, &lReceivedValue, xTicksToWait);

		if (xStatus == pdPASS)
		{
			//sprintf(str, "Valor %.3f recebido da fila xQueue_vC\r\n", lReceivedValue);
			//printf_colored(str, COLOR_MAGENTA);
			buffer_vC[i] = lReceivedValue;
			if (i == (N - 1)) {
				vC_RMS = CalcRMS_S9(&buffer_vC);
				rmsCalcFlag = 1;
			}
		}
		else
		{
			//printf_colored("Ocorreu algum erro na recepção da fila xQueue_vC!\r\n", COLOR_RED);
		}

		if (rmsCalcFlag) {
			i = 0;
			rmsCalcFlag = 0;
		}
		else {
			vA_RMS = -999;
			vB_RMS = -999;
			vC_RMS = -999;
		}

		//printf("\r\n\rTask RMS: GIVE \n\r\n");
		xSemaphoreGive(xBinarySemaphore);
		//xSemaphoreGive(xMutex);
	}
}

// Função para realizar o cálculo RMS do valor informado
float CalcRMS_S9(float* pvParameters) {
	unsigned int ui;
	float average = 0.0;

	for (ui = 0; ui < N; ui++)
	{
		average += pow(pvParameters[ui], 2);
	}
	// fazer a média dos quadrados dos N itens lidos e Tirar a raiz
	return sqrt(average/N);
}