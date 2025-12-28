#ifndef INC_FREERTOSCONFIG_H
#define INC_FREERTOSCONFIG_H

#include "system_stm32f3xx.h"

#define configUSE_PREEMPTION            1
#define configCPU_CLOCK_HZ              ( SystemCoreClock )
#define configTICK_RATE_HZ              ( 1000 )   // 1 ms tick
#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        128
#define configTOTAL_HEAP_SIZE           ( 10 * 1024 )
#define configUSE_16_BIT_TICKS          0
#define configUSE_MUTEXES               1
#define configUSE_TIMERS                0
#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0

#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;); }

/* Cortex-M interrupt priority bits (STM32F303 = 4 bits) */
#define configPRIO_BITS                         4

/* Lowest interrupt priority */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15

/* Highest interrupt priority that can use FreeRTOS API */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Kernel interrupt priority (lowest) */
#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Max syscall interrupt priority (critical!) */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define INCLUDE_vTaskDelay        1
#define INCLUDE_vTaskDelayUntil   1
#define INCLUDE_xTaskGetSchedulerState 1

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif