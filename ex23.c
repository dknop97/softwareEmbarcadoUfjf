/* Inlcudes Padrão C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Defines para os intervalos dos timers */
#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS(1500)
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS(200)

/* Protótipos dos Callbacks dos timers*/
static void prvOneShotTimerCallback_ex23(TimerHandle_t xTimer);
static void prvAutoReloadTimerCallback_ex23(TimerHandle_t xTimer);

int main_ex23(void)
{
	/* Variáveis para a criação dos Timers*/
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
	
	/* Variáveis para inicialização dos Timers*/
	BaseType_t xTimer1Started, xTimer2Started;


	
	/* Cria um timer do tipo one shot e armazena o handle deste timer em xOneShotTimer*/
	xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback_ex23);
	printf("OneShot timer criado!\r\n\n");

	/* Cria um timer do tipo Auto Reload e armazena o handle deste timer em xAutoReloadTimer*/
	xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback_ex23);
	printf("AutoReload timer criado!\r\n\n");
	
	/* Verifica se os timers foram criados corretamente */
	if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL))
	{
		xTimer1Started = xTimerStart(xOneShotTimer, 0);
		printf("OneShot timer inicializado!\r\n\n");

		xTimer2Started = xTimerStart(xAutoReloadTimer, 0);
		printf("AutoReload timer inicializado!\r\n\n");

		if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS))
		{
			/*Inicia o Escalonador*/
			printf("--------------------------------\r\n");
			printf("Escalonador Iniciado!\r\n");
			printf("--------------------------------\r\n\n");
			vTaskStartScheduler();

			while (1);
		}
		else
		{
			printf("Erro na inicialização dos Timers.\r\n");
		}
	}
	else
	{
		printf("Erro na criação dos Timers.\r\n");
	}
}

static void prvOneShotTimerCallback_ex23(TimerHandle_t xTimer)
{
	char str[80]; // Auxiliar

	TickType_t xTimeNow; // Variável para obtenção do tempo

	xTimeNow = xTaskGetTickCount(); // Verificação da contagem de ticks

	sprintf(str, "Callback do timer OneShot executando em: %d \n", xTimeNow * portTICK_PERIOD_MS);
	printf_colored(str, COLOR_GREEN);
}


static void prvAutoReloadTimerCallback_ex23(TimerHandle_t xTimer)
{
	char str[80]; // Auxiliar

	TickType_t xTimeNow; // Variável para obtenção do tempo

	xTimeNow = xTaskGetTickCount(); // Verificação da contagem de ticks

	sprintf(str, "Callback do timer AutoReload executando em: %d \n", xTimeNow * portTICK_PERIOD_MS);
	printf_colored(str, COLOR_CYAN);
}

