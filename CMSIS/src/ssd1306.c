#include <string.h>
#include "ssd1306.h"
#include "spi.h"

// ======== OLED Command / Data ========
void OLED_WriteCommand(uint8_t cmd)
{
    OLED_DC_LOW();
    OLED_CS_LOW();
    SPI1_SendByte(cmd);
    OLED_CS_HIGH();
}

void OLED_WriteData(uint8_t data)
{
    OLED_DC_HIGH();
    OLED_CS_LOW();
    SPI1_SendByte(data);
    OLED_CS_HIGH();
}

// ======== OLED Reset ========
void OLED_Reset(void)
{
    OLED_RST_LOW();
    for (volatile int i = 0; i < 100000; i++);
    OLED_RST_HIGH();
}

// ======== OLED Init (SSD1306 Minimal) ========
void OLED_Init(void)
{
    OLED_Reset();

    OLED_WriteCommand(0xAE); // Display OFF

    OLED_WriteCommand(0xD5); // Set display clock divide ratio/oscillator frequency
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // Set multiplex ratio
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // Set display offset
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // Set display start line = 0

    OLED_WriteCommand(0x8D); // Enable charge pump
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0x20); // Memory addressing mode
    OLED_WriteCommand(0x00); // Horizontal addressing

    OLED_WriteCommand(0xA1); // Segment remap
    OLED_WriteCommand(0xC8); // COM scan direction remap

    OLED_WriteCommand(0xDA); // COM pins hardware configuration
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // Contrast
    OLED_WriteCommand(0x7F);

    OLED_WriteCommand(0xD9); // Pre-charge
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // VCOMH deselect level
    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0xA4); // Resume to RAM content
    OLED_WriteCommand(0xA6); // Normal display

    OLED_WriteCommand(0xAF); // Display ON
}

// Clear OLED
void OLED_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_WriteCommand(0xB0 + page);
        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);

        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_WriteData(0x00);
        }
    }
}

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_PAGES  (OLED_HEIGHT / 8)

uint8_t oled_buf[OLED_WIDTH * OLED_PAGES];  // 1024 bytes framebuffer

void OLED_TestWhite(void)
{
    for (int page = 0; page < 8; page++) {
        OLED_WriteCommand(0xB0 + page); // page address
        OLED_WriteCommand(0x00);        // low column
        OLED_WriteCommand(0x10);        // high column

        for (int i = 0; i < 128; i++) {
            OLED_WriteData(0xFF);       // set all high (all white)
        }
    }
}

void OLED_DrawBitmap(const unsigned char *bitmap)
{
    for (int page = 0; page < 8; page++) {
        OLED_WriteCommand(0xB0 + page); // page address
        OLED_WriteCommand(0x00);        // low column
        OLED_WriteCommand(0x10);        // high column

        for (int i = 0; i < 128; i++) {
            OLED_WriteData(bitmap[page * 128 + i]);
        }
    }
}
