#include "spi.h"

// SPI1 Initialization
void SPI1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR;                      // master mode
    SPI1->CR1 |= SPI_CR1_BR_1;                      // Fpclk/8
    SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM;         // Software NSS
    SPI1->CR1 |= SPI_CR1_SPE;                       // Enable SPI
}


// SPI1 SendByte
void SPI1_SendByte(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE));       // Wait TX buffer 空
    *((__IO uint8_t *)&SPI1->DR) = data;

    while (!(SPI1->SR & SPI_SR_TXE));       // Wait SPI send done
    while ( SPI1->SR & SPI_SR_BSY  );       // Wait SPI
}

void SPI1_GPIO_Init(void)
{
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;

    // PA5 (SCK), PA7 (MOSI) -> AF5
    GPIOA->MODER &= ~((3 << (5*2)) | (3 << (7*2)));
    GPIOA->MODER |=  ((2 << (5*2)) | (2 << (7*2))); 
    GPIOA->AFR[0] |= (5 << (5*4)) | (5 << (7*4));

    // PA4 CS = GPIO output
    GPIOA->MODER &= ~(3 << (4*2));
    GPIOA->MODER |=  (1 << (4*2));
    GPIOA->ODR |= (1 << 4);   // default high

    // PB0 DC = GPIO output
    GPIOB->MODER &= ~(3 << (0*2));
    GPIOB->MODER |=  (1 << (0*2));

    // PB1 RST = GPIO output
    GPIOB->MODER &= ~(3 << (1*2));
    GPIOB->MODER |=  (1 << (1*2));
}
