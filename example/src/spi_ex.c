#include "spi_ex.h"


void SPI3_gpio_init(void)
{
	/** 
	 * SPI example: Use Logic Analyze to monitor signal
	 */
	// PC10: SPI3_SCK
	// PC11: SPI3_MISO
	// PC12: SPI3_MOSI
	// AF6
	GPIO_HANDLE_T SPIPins;
	SPIPins.ptr_gpio = (GPIO_REG_T *) GPIOC_BASEADDR;
	SPIPins.gpio_pin_config.mode = GPIO_MODE_ALT;
	SPIPins.gpio_pin_config.altfuncmode = 6;		// AF6
	SPIPins.gpio_pin_config.optype = GPIO_OUT_TYPE_PP;
	SPIPins.gpio_pin_config.pupdctl = GPIO_PUPD_NPUPD;
	SPIPins.gpio_pin_config.spd = GPIO_OUT_SPD_HIGH;

	// SPI_CLK
	SPIPins.gpio_pin_config.num = GPIO_PIN_10;
	GPIO_init(&SPIPins);
	// SPI_MISO
	SPIPins.gpio_pin_config.num = GPIO_PIN_11;
	GPIO_init(&SPIPins);
	// SPI_MOSI
	SPIPins.gpio_pin_config.num = GPIO_PIN_12;
	GPIO_init(&SPIPins);
}

void SPI3_init(void)
{
	SPI_HANDLE_T spi3_handler;
	spi3_handler.ptr_spi = (SPI_I2S_REG_T *)SPI3_I2S3_BASEADDR;
	spi3_handler.spi_pin_config.busconfig = SPI_BUS_FULL_DPLX;	// Full duplex
	spi3_handler.spi_pin_config.devicemode = SPI_DEV_MODE_MASTER;
	spi3_handler.spi_pin_config.sclkspd = SPI_SCLK_SPD_DIV2;
	spi3_handler.spi_pin_config.ds = SPI_DS_8BITS;
	spi3_handler.spi_pin_config.cpol = SPI_CPOL_LOW;
	spi3_handler.spi_pin_config.cpha = SPI_CPHA_LOW;
	spi3_handler.spi_pin_config.ssm = SPI_SSM_EN;		// Software slave management enabled for NSS

	SPI_init(&spi3_handler);
}
