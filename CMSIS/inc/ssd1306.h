#include "stm32f303xe.h"

// ======== Pin Mapping ========
// PA5  = SPI1_SCK
// PA7  = SPI1_MOSI
// PA4  = CS
// PB0  = DC
// PB1  = RST

// ======== GPIO Macro ========
#define OLED_CS_LOW()      (GPIOA->ODR &= ~(1 << 4))
#define OLED_CS_HIGH()     (GPIOA->ODR |=  (1 << 4))

#define OLED_DC_LOW()      (GPIOB->ODR &= ~(1 << 0))
#define OLED_DC_HIGH()     (GPIOB->ODR |=  (1 << 0))

#define OLED_RST_LOW()     (GPIOB->ODR &= ~(1 << 1))
#define OLED_RST_HIGH()    (GPIOB->ODR |=  (1 << 1))


void OLED_Reset(void);


void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_TestWhite(void);

void OLED_TestPic(void);