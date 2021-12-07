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
#define BACKL_TIMER_PERIOD pdMS_TO_TICKS(2000)

/* Vari�veis para a cria��o dos Timers*/
TimerHandle_t  xBacklightTimer;

/* Vari�veis para a contrle */
int xSimulatedBacklightOn = pdFALSE;

/* Prot�tipos do Callback do timer*/
static void prvBacklightTimerCallback_ex25(TimerHandle_t xTimer);

/* Prot�tipo da Task para leitura do teclado*/
static void vKeyHitTask_ex25(void* pvParameters);

int main_ex25(void)
{
	/* Vari�vel para inicializa��o do Timer*/
	BaseType_t xTimer1Started;

	/* Cria um timer do tipo one shot e armazena o handle deste timer em xOneShotTimer*/
	xBacklightTimer = xTimerCreate("OneShot", BACKL_TIMER_PERIOD, pdFALSE, 0, prvBacklightTimerCallback_ex25);
	printf("OneShot timer criado!\r\n\n");
	
	/* Cria a task para leitura do teclado */
	xTaskCreate(vKeyHitTask_ex25, "Pooling", 1024, NULL, 3, NULL);
	printf("Task para leitura do teclado criada!\r\n\n");

	/* Verifica se o timer foi criado corretamente */
	if ((xBacklightTimer != NULL))
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

static void vKeyHitTask_ex25(void* pvParameters)
{
	char str[80]; // Auxiliar
	
	const TickType_t xShortDelay = pdMS_TO_TICKS(50); // Auxiliar
	
	TickType_t xTimeNow; // Vari�vel para obten��o do tempo

	printf("Pressione alguma tecla para ligar o backlight.\n");

	/* O ideal � que uma aplica��o desse tipo seja orientada por eventos e use uma 
	interrup��o	para processar o apertar de teclas. N�o � pr�tico usar interrup��es 
	de teclado ao usar o port do FreeRTOS para Windows, portanto, essa task � usada para 
	realizar um pooling no teclado. */

	for (;; )
	{
		/* Alhuma tecla foi pressionada? */
		if (_kbhit() != 0)
		{
			/* Armazena o tempo quando uma tecla for pressionada. */
			xTimeNow = xTaskGetTickCount();

			if (xSimulatedBacklightOn == pdFALSE)
			{
				/* O backlight estava desligado. Ligue o backlight e imprime o tempo */
				xSimulatedBacklightOn = pdTRUE;

				sprintf(str, "Uma tecla foi pressionada. Ligando o backlight em:  %d \n", xTimeNow * portTICK_PERIOD_MS);
				printf_colored(str, COLOR_GREEN);
			}
			else
			{
				sprintf(str, "Uma tecla foi pressionada. Resetando o timer em:  %d \n", xTimeNow * portTICK_PERIOD_MS);
				printf_colored(str, COLOR_CYAN);
			}

			xTimerReset(xBacklightTimer, xShortDelay);

			(void)_getch();
		}
	}
}

static void prvBacklightTimerCallback_ex25(TimerHandle_t xTimer)
{
	char str[80]; // Auxiliar

	TickType_t xTimeNow = xTaskGetTickCount(); // Verifica��o da contagem de ticks
	
	/* O tempo de backlight expirou. Desligando o  backlight*/
	xSimulatedBacklightOn = pdFALSE;
	
	/* Imprime o tempo em que o backlight foi desligado. */
	sprintf(str, "O tempo expirou. Desligando o backlight em:  %d \n", xTimeNow * portTICK_PERIOD_MS);
	printf_colored(str, COLOR_YELLOW);
}



