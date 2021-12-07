/* Inlcudes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Defines para os intervalos dos timers */
#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS(1700)
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS(500)

/* Prot�tipos do Callback dos timers*/
static void prvTimerCallback_ex24(TimerHandle_t xTimer);

/* Vari�veis para a cria��o dos Timers*/
TimerHandle_t xAutoReloadTimer, xOneShotTimer;

int main_ex24(void)
{
	/* Vari�veis para inicializa��o dos Timers*/
	BaseType_t xTimer1Started, xTimer2Started;

	/* Cria um timer do tipo one shot e armazena o handle deste timer em xOneShotTimer*/
	xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvTimerCallback_ex24);
	printf("OneShot timer criado!\r\n\n");

	/* Cria um timer do tipo Auto Reload e armazena o handle deste timer em xAutoReloadTimer*/
	xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvTimerCallback_ex24);
	printf("AutoReload timer criado!\r\n\n");

	/* Verifica se os timers foram criados corretamente */
	if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL))
	{
		xTimer1Started = xTimerStart(xOneShotTimer, 0);
		printf("OneShot timer inicializado!\r\n\n");

		xTimer2Started = xTimerStart(xAutoReloadTimer, 0);
		printf("AutoReloader timer inicializado!\r\n\n");

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
			printf("Erro na inicializa��o dos Timers.\r\n");
		}
	}
	else
	{
		printf("Erro na cria��o dos Timers.\r\n");
	}
}

static void prvTimerCallback_ex24(TimerHandle_t xTimer)
{
	char str[80]; // Auxiliar

	TickType_t xTimeNow; // Vari�vel para obten��o do tempo
	uint32_t ulExecutionCount; // Vari�vel para o n�mero de vezes que o callback executou

	/* Realiza a Leitura do Timer ID e realiza o incremento para marcar 
	a quantidade de vezes que o callback do timer j� executou*/
	ulExecutionCount = (uint32_t)pvTimerGetTimerID(xTimer);
	ulExecutionCount++;
	vTimerSetTimerID(xTimer, (void*)ulExecutionCount);

	xTimeNow = xTaskGetTickCount(); // Verifica��o da contagem de ticks

	if (xTimer == xOneShotTimer)
	{
		sprintf(str, "Timer OneShot ExecutionCount =  %d \n", ulExecutionCount);
		printf_colored(str, COLOR_GREEN);

		sprintf(str, "Callback do timer OneShot executando em: %d \n", xTimeNow * portTICK_PERIOD_MS);
		printf_colored(str, COLOR_GREEN);
	}
	else
	{
		sprintf(str, "Timer AutoReload ExecutionCount =  %d \n", ulExecutionCount);
		printf_colored(str, COLOR_CYAN);

		sprintf(str, "Callback do timer AutoReload executando em: %d \n", xTimeNow * portTICK_PERIOD_MS);
		printf_colored(str, COLOR_CYAN);
		
		if (ulExecutionCount == 5)
		{
			xTimerStop(xTimer, 0);
			xTimerReset(xOneShotTimer,0);

		}
	}
}



