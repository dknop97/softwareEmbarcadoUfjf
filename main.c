/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
 * The simply blinky demo is implemented and described in main_blinky.c.  The
 * more comprehensive test and demo application is implemented and described in
 * main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and FreeRTOS hook functions.
 *
 *******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *

 *
 *******************************************************************************
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Include para prints*/
#include "defines.h"

/* This project provides two demo applications.  A simple blinky style demo
application, and a more comprehensive test and demo application.  The
mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
The blinky demo is implemented and described in main_blinky.c.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
demo application will be built.  The comprehensive test and demo application is
implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE	7201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	6407

/*-----------------------------------------------------------*/

/*
 * Exemplo 1  : Declarando duas tasks com a mesma prioridade.
 * Exemplo 2  : Criando uma task dentro de outra task.
 * Exemplo 3  : Utilizando os parametros das tasks.
 * Exemplo 4  : Declarando duas tasks com prioridades diferentes.
 * Exemplo 5  : Bloqueando uma task que espera um delay vTaskDelay(). -- Testar com a IdleTask e bloqueando uma e as duas tasks da aplicacao
 * Exemplo 6  : Bloqueando uma task que espera um delay vTaskDelayUntil().
 * Exemplo 7  : Misturando tasks que bloqueiam e que não bloqueiam.
 * Exemplo 8  : Aumentando a prioridade de uma task.
 * Exemplo 9  : Diminuindo a prioridade de uma task.
 * Exemplo 10 : Deletando uma task.
 * Exemplo 11 : Utilizando semaforo BINARIO para sincronizar duas tasks. -- Mostrar a com a IdleTask
 * Exemplo 12 : Utilizando semaforo CONTADOR para sincronizar duas tasks.
 * Exemplo 13 : Mutex Recurso Compartilhado.
 * Exemplo 14 : Inversão de prioridade: Semáforo Binário e Mutex
 * Exemplo 15 : Utilizando filas de mensagem.
 * Exemplo 16 : Bloqueando no envio de mensagem / Enviando estruturas.
 * Exemplo 17 : Registrador de Eventos - Fumantes.
 * Exemplo 18 : StreamBuffer
 * Exemplo 19 : QueueSet
 * Exemplo 20 : Interrupção + Semáforo Binário
 * Exemplo 21 : Interrupção + Semáforo Contador
 * Exemplo 22 : Interrupção + Filas de Mensagem
 * Exemplo 23 : OneShot Timer + AutoReload Timer Callbacks Diferentes
 * Exemplo 24 : OneShot Timer + AutoReload Timer mesmo Callback
 * Exemplo 25 : Emular o Controle de temporização de um backlight
 * Exemplo 26 :
 * Exemplo 27 : 
 * Exemplo 28 : 
 * Exemplo 29 : QUESTÃO 01 DA ATIVIDADE AVALIATIVA DA SEMANA 5 SOBRE TASKS
 * Exemplo 30 : QUESTÃO 02 DA ATIVIDADE AVALIATIVA DA SEMANA 5 SOBRE TASKS
 * Exemplo 31 : ATIVIDADE AVALIATIVA DA SEMANA 7 SOBRE SEMÁFOROS
 * Exemplo 32 : ATIVIDADE AVALIATIVA DA SEMANA 9
*/

/* Escolha qual exemplo deseja executar: */

#define example 32

#if (example == 1)
extern void main_ex1(void);
#endif
#if (example == 2)
extern void main_ex2(void);
#endif
#if (example == 3)
extern void main_ex3(void);
#endif
#if (example == 4)
extern void main_ex4(void);
#endif
#if (example == 5)
extern void main_ex5(void);
#endif
#if (example == 6)
extern void main_ex6(void);
#endif
#if (example == 7)
extern void main_ex7(void);
#endif
#if (example == 8)
extern void main_ex8(void);
#endif
#if (example == 9)
extern void main_ex9(void);
#endif
#if (example == 10)
extern void main_ex10(void);
#endif
#if (example == 11)
extern void main_ex11(void);
#endif
#if (example == 12)
extern void main_ex12(void);
#endif
#if (example == 13)
extern void main_ex13(void);
#endif
#if (example == 14)
extern void main_ex14(void);
#endif
#if (example == 15)
extern void main_ex15(void);
#endif
#if (example == 16)
extern void main_ex16(void);
#endif
#if (example == 17)
extern void main_ex17(void);
#endif
#if (example == 18)
extern void main_ex18(void);
#endif
#if (example == 19)
extern void main_ex19(void);
#endif
#if (example == 20)
extern void main_ex20(void);
#endif
#if (example == 21)
extern void main_ex21(void);
#endif
#if (example == 22)
extern void main_ex22(void);
#endif
#if (example == 23)
extern void main_ex23(void);
#endif
#if (example == 24)
extern void main_ex24(void);
#endif
#if (example == 25)
extern void main_ex25(void);
#endif
#if (example == 26)
extern void main_ex26(void);
#endif
#if (example == 27)
extern void main_ex27(void);
#endif
#if (example == 28)
extern void main_ex28(void);
#endif
// Início dos projetos das avaliações
#if (example == 29)
extern void main_av5_q1(void);
#endif
#if (example == 30)
extern void main_av5_q2(void);
#endif
#if (example == 31)
extern void main_av7(void);
#endif
#if (example == 32)
extern void main_S9(void);
#endif

/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * http://www.freertos.org/a00016.html for more information.
 */
void vFullDemoTickHookFunction( void );
void vFullDemoIdleFunction( void );

/*
 * This demo uses heap_5.c, so start by defining some heap regions.  It is not
 * necessary for this demo to use heap_5, as it could define one large heap
 * region.  Heap_5 is only used for test and example purposes.  See
 * http://www.freertos.org/a00111.html for an explanation.
 */
static void  prvInitialiseHeap( void );

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile( void );

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/*-----------------------------------------------------------*/

int main( void )
{
	setlocale(LC_ALL, "Portuguese"); // Configuração para printar caracteres em Português

	/* This demo uses heap_5.c, so start by defining some heap regions.  heap_5
	is only used for test and example reasons.  Heap_4 is more appropriate.  See
	http://www.freertos.org/a00111.html for an explanation. */
	prvInitialiseHeap();

	/* Initialise the trace recorder.  Use of the trace recorder is optional.
	See http://www.FreeRTOS.org/trace for more information. */
	vTraceEnable( TRC_START );

	//printf("********************************\r\n");
	//printf("Rodando Exemplo %d\r\n", example);
	//printf("********************************\r\n\n");

	/*Dependendo do exemplo escolhido, roda a main do exemplo certo*/
	#if (example == 1)
		main_ex1();
	#endif
	#if (example == 2)
		main_ex2();
	#endif
	#if (example == 3)
		main_ex3();
	#endif
	#if (example == 4)
		main_ex4();
	#endif
	#if (example == 5)
		main_ex5();
	#endif
	#if (example == 6)
		main_ex6();
	#endif
	#if (example == 7)
		main_ex7();
	#endif
	#if (example == 8)
		main_ex8();
	#endif
	#if (example == 9)
		main_ex9();
	#endif
	#if (example == 10)
		main_ex10();
	#endif
	#if (example == 11)
		main_ex11();
	#endif
	#if (example == 12)
		main_ex12();
	#endif
	#if (example == 13)
		main_ex13();
	#endif
	#if (example == 14)
		main_ex14();
	#endif
	#if (example == 15)
		main_ex15();
	#endif
	#if (example == 16)
		main_ex16();
	#endif
	#if (example == 17)
		main_ex17();
	#endif
	#if (example == 18)
		main_ex18();
	#endif
	#if (example == 19)
		main_ex19();
	#endif
	#if (example == 20)
		main_ex20();
	#endif
	#if (example == 21)
		main_ex21();
	#endif
	#if (example == 22)
		main_ex22();
	#endif
	#if (example == 23)
		main_ex23();
	#endif
	#if (example == 24)
		main_ex24();
	#endif
	#if (example == 25)
		main_ex25();
	#endif
	#if (example == 26)
		main_ex26();
	#endif
	#if (example == 27)
		main_ex27();
	#endif
	#if (example == 28)
		main_ex28();
	#endif
	// Início dos projetos das avaliações
	#if (example == 29)
		main_av5_q1();
	#endif
	#if (example == 30)
		main_av5_q2();
	#endif
	#if (example == 31)
		main_av7();
	#endif
	#if (example == 32)
		main_S9();
	#endif
	// Fim dos projetos das avaliações
	#if (example == 99)
		main_correcao();
	#endif

	return 0;
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
	size of the	heap available to pvPortMalloc() is defined by
	configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
	API function can be used to query the size of free heap space that remains
	(although it does not provide information on how the remaining heap might be
	fragmented).  See http://www.freertos.org/a00111.html for more
	information. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	unsigned int ui;
	unsigned long ul;

	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If application tasks make use of the
	vTaskDelete() API function to delete themselves then it is also important
	that vApplicationIdleHook() is permitted to return to its calling function,
	because it is the responsibility of the idle task to clean up memory
	allocated by the kernel to any task that has since deleted itself. */

	/* Uncomment the following code to allow the trace to be stopped with any
	key press.  The code is commented out by default as the kbhit() function
	interferes with the run time behaviour. */
	/*
		if( _kbhit() != pdFALSE )
		{
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
				xTraceRunning = pdFALSE;
			}
		}
	*/

	//printf("IDLE!!!\r\n");
	for (ui = 0; ui < 3; ui++)
	{
		for (ul = 0; ul < 10000000; ul++)
		{
			/////
		}
	}

	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		vFullDemoIdleFunction();
	}
	#endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  This function is
	provided as an example only as stack overflow checking does not function
	when running the FreeRTOS Windows port. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */


	/* Descomentar para o exemplo 27*/
	/*unsigned int ui;
	unsigned long ul;

	static unsigned int ctrl = 0;

	if ((ctrl % 5) == 0) 
	{
		for (ui = 0; ui < 100; ui++)
		{

			printf_colored("3", COLOR_RED);

			for (ul = 0; ul < 10000000; ul++)
			{

			}
		}
		printf("\r\n\r\n\r\n");
	}
	ctrl++;*/
	
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
	/* This function will be called once only, when the daemon task starts to
	execute	(sometimes called the timer task).  This is useful if the
	application includes initialisation code that would benefit from executing
	after the scheduler has been started. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static BaseType_t xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

	printf( "ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError() );

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm{ NOP };
			__asm{ NOP };
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile( void )
{
FILE* pxOutputFile;

	fopen_s( &pxOutputFile, "Trace.dump", "wb");

	if( pxOutputFile != NULL )
	{
		fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
		fclose( pxOutputFile );
		printf( "\r\nTrace output saved to Trace.dump\r\n" );
	}
	else
	{
		printf( "\r\nFailed to create trace dump file\r\n" );
	}
}
/*-----------------------------------------------------------*/

static void  prvInitialiseHeap( void )
{
/* The Windows demo could create one large heap region, in which case it would
be appropriate to use heap_4.  However, purely for demonstration purposes,
heap_5 is used instead, so start by defining some heap regions.  No
initialisation is required when any other heap implementation is used.  See
http://www.freertos.org/a00111.html for more information.

The xHeapRegions structure requires the regions to be defined in start address
order, so this just creates one big array, then populates the structure with
offsets into the array - with gaps in between and messy alignment just for test
purposes. */
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
const HeapRegion_t xHeapRegions[] =
{
	/* Start address with dummy offsets						Size */
	{ ucHeap + 1,											mainREGION_1_SIZE },
	{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
	{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
	{ NULL, 0 }
};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

	/* Prevent compiler warnings when configASSERT() is not defined. */
	( void ) ulAdditionalOffset;

	vPortDefineHeapRegions( xHeapRegions );
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

