#include "stm32f303xe.h"
#include "systick.h"

volatile uint32_t sys_ms = 0;

void systick_init(void)
{
    SysTick_Config(SystemCoreClock / 1000);
}

void SysTick_Handler(void)
{
    sys_ms++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = sys_ms;
    while ((sys_ms - start) < ms) {
        __NOP();
    }
}
