#ifndef __SYSTICK_H_
#define __SYSTICK_H_

extern volatile uint32_t sys_ms;

void systick_init(void);
void SysTick_Handler(void);
void delay_ms(uint32_t ms);

#endif
