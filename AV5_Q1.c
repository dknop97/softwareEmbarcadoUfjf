/*
* Atividade: Avalia��o Semana 5 - CEL080 - 2021.3 - TURMA A
* Autor: David Nery Henriques Knop
* Matr�cula: 201669038A
* Data: 01/12/2021
*
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* Quest�o 1) Tomando como refer�ncia o diagrama da figura Questao01.png e sabendo que a a Task1 tem prioridade maior do que a Task2,   %
* implemente utilizando o FreeRTOS no Visual Studio uma aplica��o que modele a situa��o A e uma que implemente a situa��o B.		   %
* Grave um v�deo, onde voc� apare�a explicando as suas implementa��es.Explique a diferen�a entre as implementa��es das situa��es A e B.%
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
/* Includes Padr�o C */
#include <stdio.h>
#include <locale.h>

/* Include para prints*/
#include "defines.h"

/* Includes do Kernel */
#include "FreeRTOS.h"
#include "task.h"

/* Vari�veis de Controle*/
// Qual a inicializa��o correta pra essa vari�vel?
unsigned int uTask1TimeToWake = 200; // 200ms

/* Prot�tipo das fun�oes das Tasks*/
void vTask1_av5_q1(void* pvParameters);
void vTask2_av5_q1(void* pvParameters);

void main_av5_q1(void)
{
	printf_colored(" >>>>>>>>>> AV5_Q1 <<<<<<<<<<\n\n", COLOR_YELLOW);

	/*Cria a Task 1*/
	xTaskCreate(vTask1_av5_q1, "Task1_av5_q1", 1000, NULL, 2, NULL);
	printf("AV5_Q1: Task 1 Criada!\r\n\n");

	/*Cria a Task 2*/
	xTaskCreate(vTask2_av5_q1, "Task2_av5_q1", 1000, NULL, 1, NULL);
	printf("AV5_Q1: Task 2 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("AV5_Q1: Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}

void vTask1_av5_q1(void* pvParameters)
{
	portTickType xLastWakeTime;

	printf_colored("AV5_Q1: Task 1 executa pela primeira vez!\n", COLOR_GREEN);
	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		printf_colored("\n-------------------------------\n", COLOR_GREEN);
		printf_colored("AV5_Q1: Task 1 est� executando!\n", COLOR_GREEN);
		printf("xLastWakeTime: %d\n", (int)xLastWakeTime);
		printf_colored("-------------------------------\n\n", COLOR_GREEN);
		/* Nosso tick est� para 10Hz, ou seja, 100ms. Logo, 200/100 dar� 2,
		// Sendo assim, nosso printf deve retornar uma contagem de 2 em 2*/
		
		/*vTaskDelayUntil coloca a task no estado BLOCKED pelo tempo
		* necess�rio para que o tempo uTask3TimeToWake seja totalmente preenchido
		* somando o tempo de execu��o e o tempo como bloqueada
		*/
		// Para a situa��o A
		vTaskDelayUntil(&xLastWakeTime, (uTask1TimeToWake / portTICK_RATE_MS));
		// Para a situa��o B
		// vTaskDelay(uTask1TimeToWake / portTICK_RATE_MS);
	}
}

void vTask2_av5_q1(void* pvParameters)
{
	unsigned int ui;
	unsigned long ul;
	portTickType xNewLastWakeTime;
	printf_colored("AV5_Q1: Task 2 executa pela primeira vez!\n", COLOR_CYAN);
	// Esta task vai executar enquanto a task 1 estiver bloqueada, ou seja,
	// at� que o vTaskDelayUntil() desperte e a mova para execu��o, 
	// visto que sua prioridade � maior do que a task 2
	for (;;)
	{
		xNewLastWakeTime = xTaskGetTickCount();
		printf_colored("AV5_Q1: Task 2 est� executando!\n", COLOR_CYAN);
		printf("xNEWLastWakeTime: %d\n", (int)xNewLastWakeTime);
		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++)
			{
				/////
			}
		}
	}
}