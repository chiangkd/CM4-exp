#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "stm32f303xe.h"

void buzzer_rcc_init(void);
void buzzer_gpio_init(void);
void buzzer_tim_init(void);
void buzzer_set_freq(uint32_t freq);
void buzzer_init(void);
void buzzer_set_volume(uint8_t volume);

#endif
