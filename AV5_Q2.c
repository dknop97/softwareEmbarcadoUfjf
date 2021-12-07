/*
* Atividade: Avalia��o Semana 5 - CEL080 - 2021.3 - TURMA A
* Autor: David Nery Henriques Knop
* Matr�cula: 201669038A
* Data: 01/12/2021
*
* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
* Quest�o 2) Tomando como refer�ncia o diagrama da figura Questao02.png e sabendo que a Task3 possui o maior n�vel de prioridade entre %
* as tasks e que o escalonador foi iniciado em t = t0, implemente utilizando o FreeRTOS no Visual Studio uma aplica��o que modele essa %
* situa��o.																															   %
* Grave um v�deo, onde voc� apare�a explicando a sua implementa��o. Justifique a escolha dos n�veis de prioridade das tasks 1 e 2.     %
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
unsigned int uTask3TimeToWake = 200; // 200ms

/* Prot�tipo das fun�oes das Tasks*/
void vTask1_av5_q2(void* pvParameters);
void vTask2_av5_q2(void* pvParameters);
void vTask3_av5_q2(void* pvParameters);

xTaskHandle vTask3_av5_q2Handle;

void main_av5_q2(void)
{
	printf_colored(" >>>>>>>>>> AV5_Q2 <<<<<<<<<<\n\n", COLOR_YELLOW);

	/*Cria a Task 2*/
	xTaskCreate(vTask2_av5_q2, "Task2_av5_q2", 1000, NULL, 1, NULL);
	printf("AV5_Q2: Task 2 Criada!\r\n\n");

	/*Cria a Task 1*/
	xTaskCreate(vTask1_av5_q2, "Task1_av5_q2", 1000, NULL, 1, NULL);
	printf("AV5_Q2: Task 1 Criada!\r\n\n");

	/*Inicia o Escalonador*/
	printf("--------------------------------\r\n");
	printf("AV5_Q2: Escalonador Iniciado!\r\n");
	printf("--------------------------------\r\n\n");
	vTaskStartScheduler();

	while (1);
}

void vTask1_av5_q2(void* pvParameters)
{
	/*  Essa task vai executar revezando o tempo de processamento com a
		task 2, executando inicialmente depois dela, pois possuem mesma prioridade,
		por�m a task 2 foi declarada primeiro
	*/

	unsigned int ui;
	unsigned long ul;

	printf_colored("AV5_Q2: Task 1 executa pela primeira vez!\n", COLOR_GREEN);

	/*Cria a Task 3
	* A cria��o foi alocada aqui para que pudesse seguir o diagrama do enunciado.
	* O diagrama indica que a primeira execu��o da task 3 � realizada ap�s
	* task 1 e 2 terem sido executadas por pelo menos uma vez e a task � sempre
	* retomada ap�s a task 3.
	*/
	xTaskCreate(vTask3_av5_q2, "Task3_av5_q2", 1000, NULL, 1, vTask3_av5_q2Handle);
	printf_colored("\tAV5_Q2: Task 3 Criada!\r\n", COLOR_RED);

	for (;;)
	{
		printf_colored("AV5_Q2: Task 1 est� executando!\n", COLOR_GREEN);

		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++);
		}
	}
}

void vTask2_av5_q2(void* pvParameters)
{
	/*  Essa task vai executar revezando o tempo de processamento com a
	*	task 1, executando inicialmente antes dela, pois possuem mesma prioridade,
	*	por�m a task 2 foi declarada primeiro
	*/

	unsigned int ui;
	unsigned long ul;

	printf_colored("AV5_Q2: Task 2 executa pela primeira vez!\n", COLOR_CYAN);

	for (;;)
	{
		printf_colored("AV5_Q2: Task 2 est� executando!\n", COLOR_CYAN);
		/*Este loop for � utilizado para emular o processamento desta task.*/
		for (ui = 0; ui < 3; ui++)
		{
			for (ul = 0; ul < 10000000; ul++);
		}
	}
}

void vTask3_av5_q2(void* pvParameters)
{
	portTickType xLastWakeTime;

	vTaskPrioritySet(vTask3_av5_q2Handle, 2);

	printf_colored("AV5_Q2: Task 3 executa pela primeira vez!\n", COLOR_MAGENTA);
	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		xLastWakeTime = xTaskGetTickCount();

		printf_colored("\n-------------------------------\n", COLOR_MAGENTA);
		printf_colored("AV5_Q2: Task 3 est� executando!\n", COLOR_MAGENTA);
		printf("AV5_Q2: xLastWakeTime: %d\n\n", (int)xLastWakeTime);
		printf_colored("-------------------------------\n\n", COLOR_MAGENTA);

		/* Nosso tick est� para 10Hz, ou seja, 100ms. Logo, 200/100 dar� 2.
		*  Sendo assim, nosso printf deve retornar uma contagem de 2 em 2
		*/

		/*vTaskDelayUntil coloca a task no estado BLOCKED pelo tempo
		* necess�rio para que o tempo uTask3TimeToWake seja totalmente preenchido
		* somando o tempo de execu��o e o tempo como bloqueada
		*/
		vTaskDelayUntil(&xLastWakeTime, (uTask3TimeToWake / portTICK_RATE_MS));
	}
}