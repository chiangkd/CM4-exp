#include <stddef.h>

#include "stm32f303xe.h"
#include "systick.h"

#include "spi.h"
#include "ssd1306.h"
#include "buzzer.h"

#include "nyan_cat.h"

// rtos
#include "FreeRTOS.h"
#include "task.h"

#define NYAN_ANIME_TASK_PERIOD   (100)

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

void NyanCatSoundTask(void *pvParameters)
{
    (void) pvParameters;
    uint8_t idx = 0;

    for (;;) {
        uint32_t freq = nyan_music[idx].freq;
        uint32_t dur  = nyan_music[idx].dur;

        buzzer_set_freq(freq);

        vTaskDelay(pdMS_TO_TICKS(dur));
        
        idx = (idx + 1) % MUSIC_LEN;
    }
}

void NyanCatAnimeTask(void *pvParameters)
{
    (void) pvParameters;
    for (;;) {
        NyanCat_anime_update();
        vTaskDelay(pdMS_TO_TICKS(NYAN_ANIME_TASK_PERIOD));
    }
}


int main(void)
{
    BaseType_t status;

    SPI1_GPIO_Init();
    SPI1_Init();
    buzzer_init();

    OLED_Init();
    OLED_Clear();

    // Sound has higher priority
    status = xTaskCreate(NyanCatSoundTask, "NyanSound", 128, NULL, 2, NULL);
    configASSERT(status == pdPASS);

    status = xTaskCreate(NyanCatAnimeTask, "NyanAnime", 512, NULL, 1, NULL);
    configASSERT(status == pdPASS);

    vTaskStartScheduler();

    // Never runs here
    for (;;);
}