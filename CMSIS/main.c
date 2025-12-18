#include "stm32f303xe.h"
#include "systick.h"
// Platform driver
#include "spi.h"

// Modules driver
#include "ssd1306.h"

// Buzzer driver
#include "buzzer.h"

// Nyan Cat
#include "nyan_cat.h"

// Test code
void turnOnLEDs()
{
    // Enable GPIOB clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set PB0, 7, 14 moder as OUTPUT
    GPIOB->MODER &= ~((0x3 << (0*2)) | (0x3 << (7*2)) | (0x3 << (14*2))); // 清空
    GPIOB->MODER |=  ((0x1 << (0*2)) | (0x1 << (7*2)) | (0x1 << (14*2))); // 輸出

    // Set push-pull
    GPIOB->OTYPER &= ~((1 << 0) | (1 << 7) | (1 << 14)); 

    // Set high speed
    GPIOB->OSPEEDR |= ((0x3 << (0*2)) | (0x3 << (7*2)) | (0x3 << (14*2)));

    // Set no pull-up pull-down R
    GPIOB->PUPDR &= ~((0x3 << (0*2)) | (0x3 << (7*2)) | (0x3 << (14*2)));

    // Turn on LED
    GPIOB->ODR |= (1 << 0) | (1 << 7) | (1 << 14);

    return;
}


int main(void)
{
    SPI1_GPIO_Init();
    SPI1_Init();
    systick_init();
    buzzer_init();

    OLED_Init();
    OLED_Clear();

    while (1) {
        NyanCat_sound_update();
        NyanCat_anime_update();
    }
}