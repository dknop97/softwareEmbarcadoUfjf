/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

/* Defines para mapear os ingredientes no Registrador de Eventos */
#define TAB	( 1 << 0 )
#define FOS	( 1 << 1 )
#define PAP	( 1 << 2 )

/* Handle para o Registrador de Eventos*/
EventGroupHandle_t ingredientes;

/* Handle para o Sem�foro de sincroniza��o*/
xSemaphoreHandle sem_sinc;

/* Prot�tipo das fun�oes das Tasks*/
void Task_FUMT_ex17(void* pvParameters);
void Task_FUMF_ex17(void* pvParameters);
void Task_FUMP_ex17(void* pvParameters);
void Task_AGT_ex17(void* pvParameters);

void main_ex17(void)
{
	/* Cria o Registrador de Eventos que representar� os ingredientes dispon�veis na mesa */
	ingredientes = xEventGroupCreate();

	/* Cria o Sem�foro Bin�rio que ser� utilizado para sincroniza��o */
	vSemaphoreCreateBinary(sem_sinc);

	/* Testa se os objetos foram criados corretamente */
	if (sem_sinc == NULL || ingredientes == NULL)
	{
		printf("Falha na cria��o dos objetos!\r\n\n");
	}
	else
	{
		/* Garante que o sem�foro ser� inicializado como indispon�vel */
		xSemaphoreTake(sem_sinc, 0);

		/* Cria a Task Fumante Tabaco */
		xTaskCreate(Task_FUMT_ex17, "Task Fumante Tabaco", 1000, NULL, 1, NULL);
		printf("Task Fumante Tabaco!\r\n\n");

		/* Cria a Task Fumante F�sforo */
		xTaskCreate(Task_FUMF_ex17, "Task Fumante Fosforo", 1000, NULL, 1, NULL);
		printf("Task Fumante Fosforo!\r\n\n");

		/* Cria a Task Fumante Papel */
		xTaskCreate(Task_FUMP_ex17, "Task Fumante Papel", 1000, NULL, 1, NULL);
		printf("Task Fumante Papel!\r\n\n");

		/* Cria a Task do Agente N�o Fumante */
		xTaskCreate(Task_AGT_ex17, "Task Agente", 1000, NULL, 2, NULL);
		printf("Task Agente!\r\n\n");

		/*Inicia o Escalonador*/
		printf("--------------------------------\r\n");
		printf("Escalonador Iniciado!\r\n");
		printf("--------------------------------\r\n\n");
		vTaskStartScheduler();

		vTaskStartScheduler();

		while(1);
	}
}

void Task_AGT_ex17(void* pvParameters)
{
	/* Vari�vel para representar os ingredientes selecionados */
	EventBits_t ingr_selecionados;
	
	/* Auxiliares para garantir o sorteio de dois ingredientes diferentes */
	int ingr1, ingr2;

	for (;;)
	{
		printf_colored("Agente Sorteando...\n", COLOR_RED);
		/* Sorteia o primeiro ingrediente e o disponibiliza atrav�s do Registrador de Eventos */
		ingr1 = rand() % 3;
		switch (ingr1)
		{
		case 0:
			printf_colored("Agente seleciona Tabaco.\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, TAB);
			break;
		case 1:
			printf_colored("Agente seleciona Fosforo.\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, FOS);
			break;
		case 2:
			printf_colored("Agente seleciona Papel.\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, PAP);
			break;
		}

		vTaskDelay(50 / portTICK_RATE_MS);

		printf_colored("Agente Sorteando...\n", COLOR_RED);

		/* Sorteia o segundo ingrediente e o disponibiliza atrav�s do Registrador de Eventos */
		ingr2 = ingr1;
		while (ingr2 == ingr1)
		{
			ingr2 = rand() % 3;
		}

		switch (ingr2)
		{
		case 0:
			printf_colored("Agente seleciona Tabaco.\n\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, TAB);
			break;
		case 1:
			printf_colored("Agente seleciona Fosforo.\n\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, FOS);
			break;
		case 2:
			printf_colored("Agente seleciona Papel.\n\n", COLOR_RED);
			ingr_selecionados = xEventGroupSetBits(ingredientes, PAP);
			break;
		}

		/* Bloqueia no Sem�foro de Sincroniza��o para esperar que algum fumante fume o seu cigarro */
		xSemaphoreTake(sem_sinc, portMAX_DELAY);
	}
}

void Task_FUMT_ex17(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	/* Vari�veis para testar o REgistrador de Eventos */
	EventBits_t ingr_aguardados, ingr_recebidos;


	/* Combina��o dos ingredientes aguardados pelo fumante */
	ingr_aguardados = PAP | FOS;

	for (;;)
	{
		/* Se bloqueia na combina��o AND entre os dois ingredientes aguardos */
		ingr_recebidos = xEventGroupWaitBits(ingredientes, ingr_aguardados, pdTRUE, pdTRUE, portMAX_DELAY);

		printf_colored("Fumante Tabaco fumando...\n", COLOR_BLUE);

		int tempo_fumante = rand() % 10;
		for (ui = 0; ui < tempo_fumante; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}

		printf_colored("Fumante Tabaco acabou de fumar\n\n", COLOR_BLUE);

		/* Libera o Sem�foro de Sincroniza��o para indicar que acabou e fumar */
		xSemaphoreGive(sem_sinc);
	}
}

void Task_FUMF_ex17(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	/* Vari�veis para testar o REgistrador de Eventos */
	EventBits_t ingr_aguardados, ingr_recebidos;

	/* Combina��o dos ingredientes aguardados pelo fumante */
	ingr_aguardados = TAB | PAP;

	for (;;)
	{
		/* Se bloqueia na combina��o AND entre os dois ingredientes aguardos */
		ingr_recebidos = xEventGroupWaitBits(ingredientes, ingr_aguardados, pdTRUE, pdTRUE, portMAX_DELAY);

		printf_colored("Fumante Fosforo fumando...\n", COLOR_GREEN);

		int tempo_fumante = rand() % 10;
		for (ui = 0; ui < tempo_fumante; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}

		printf_colored("Fumante Fosforo acabou de fumar\n\n", COLOR_GREEN);

		/* Libera o Sem�foro de Sincroniza��o para indicar que acabou e fumar */
		xSemaphoreGive(sem_sinc);
	}
}

void Task_FUMP_ex17(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;

	/* Vari�veis para testar o REgistrador de Eventos */
	EventBits_t ingr_aguardados, ingr_recebidos;

	/* Combina��o dos ingredientes aguardados pelo fumante */
	ingr_aguardados = TAB | FOS;

	for (;;)
	{
		/* Se bloqueia na combina��o AND entre os dois ingredientes aguardos */
		ingr_recebidos = xEventGroupWaitBits(ingredientes, ingr_aguardados, pdTRUE, pdTRUE, portMAX_DELAY);

		printf_colored("Fumante Papel fumando...\n", COLOR_YELLOW);

		int tempo_fumante = rand() % 10;
		for (ui = 0; ui < tempo_fumante; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}

		printf_colored("Fumante Papel acabou de fumar\n\n", COLOR_YELLOW);

		/* Libera o Sem�foro de Sincroniza��o para indicar que acabou e fumar */
		xSemaphoreGive(sem_sinc);
	}
}