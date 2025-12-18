#include "buzzer.h"

// PA6 output PWM

#define BUZZER_CNT_CLK 1000000UL    // counter clock = 1 MHz

void buzzer_rcc_init(void)
{
    uint32_t timer_clk;

    // GPIOA Clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // TIM3 Clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Close Timer
    TIM3->CR1 &= ~TIM_CR1_CEN;

    /* --- Determine TIM3 clock source --- */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
        timer_clk = SystemCoreClock * 2;
    else
        timer_clk = SystemCoreClock;

    // Timer Configuration
    TIM3->PSC = (timer_clk / BUZZER_CNT_CLK) - 1;

    // TIM3->PSC = 71;      // Prescaler, counter clock frequence (CK_CNT) is equal to f_ck_psc / (PSC[15:0] + 1)
    TIM3->ARR = 1000;   // Auto reload register

    // PWM Mode
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // PWM mode 1
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;

    // Duty Cycle
    TIM3->CCR1 = TIM3->ARR / 2;

    // Enable Channel Output
    TIM3->CCER |= TIM_CCER_CC1E;

    // Preload and enable
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->EGR |= TIM_EGR_UG;        // Timer event generation, update generation. 1: Re-initialize the counter
                                    // and generates an update of the register.
    TIM3->CR1 |= TIM_CR1_CEN;
}

void buzzer_gpio_init(void)
{
    // PA6 → Alternate function
    GPIOA->MODER &= ~(3 << (6 * 2));
    GPIOA->MODER |=  (2 << (6 * 2));   // AF mode

    // AF2 = TIM3_CH1
    GPIOA->AFR[0] &= ~(0xF << (6 * 4));
    GPIOA->AFR[0] |=  (2 << (6 * 4));

    // Optional: push-pull, high speed
    GPIOA->OTYPER &= ~(1 << 6);
    GPIOA->OSPEEDR |= (3 << (6 * 2));
}

void buzzer_set_freq(uint32_t freq)
{
    if (freq == 0) {
        TIM3->CCR1 = 0;   // silence
        return;
    }
    uint32_t timer_clk;

    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
        timer_clk = SystemCoreClock * 2;
    else
        timer_clk = SystemCoreClock;


    uint32_t cnt_clk = timer_clk / (TIM3->PSC + 1);

    uint32_t arr = (cnt_clk / freq) - 1;

    TIM3->ARR  = arr;
    TIM3->CCR1 = (arr + 1) / 2;  // 50% duty
    TIM3->EGR |= TIM_EGR_UG;     // Apply new ARR/CCR immediately
}

void buzzer_init(void)
{
    buzzer_rcc_init();
    buzzer_gpio_init();
}
