#include "stm32f303ze.h"
#include "gpio.h"
#include "led.h"

// Turn ON the LED
// 1. Address of the Clock control register (AHB ENR)
// - 0x40021000 + 0x14 = 0x40021014
// 2. Address of the GPIO mode register (used to control mode)
// - LD1 -> PB0:  base: 0x48000400 + GPIOx_MODER: 0x0  (Set [1:0] to 0x01: General purpose output)
// - LD2 -> PB7:  base: 0x48000400 + GPIOx_MODER: 0x0  (Set [15:14] to 0x01: General purpose output)
// - LD3 -> PB14: base: 0x48000400 + GPIOx_MODER: 0x0  (Set [29:28] to 0x01: General purpose output)

// 3. Address of the GPIO output data register (used to write)
// - LD1 -> PB0:  base: 0x48000400 + GPIOx_ODR: 0x14  (Set [0] to 0x01: Port output data)
// - LD2 -> PB7:  base: 0x48000400 + GPIOx_ODR: 0x14  (Set [7] to 0x01: Port output data)
// - LD3 -> PB14: base: 0x48000400 + GPIOx_ODR: 0x14  (Set [14] to 0x01: Port output data)

// User button: PC13
static void delay(void)
{
	for (uint32_t i = .0; i < 500000; i++);
}

void turnOnLED()
{
	GPIO_HANDLE_T gpio_led;

	// Configure push-pull for LD1
	gpio_led.ptr_gpio = (GPIO_REG_T *) GPIOB_BASEADDR;
	gpio_led.gpio_pin_config.num = GPIO_PIN_0;
	gpio_led.gpio_pin_config.mode = GPIO_MODE_OUT;
	gpio_led.gpio_pin_config.spd = GPIO_OUT_SPD_HIGH;
	gpio_led.gpio_pin_config.optype = GPIO_OUT_TYPE_PP;
	gpio_led.gpio_pin_config.pupdctl = GPIO_PUPD_NPUPD;

	GPIO_peri_clk_ctrl(gpio_led.ptr_gpio, 1);
	GPIO_init(&gpio_led);

	// Configure open-drain for LD2
	gpio_led.gpio_pin_config.num = GPIO_PIN_7;
	gpio_led.gpio_pin_config.optype = GPIO_OUT_TYPE_OD;
	gpio_led.gpio_pin_config.pupdctl = GPIO_PUPD_PU;
	GPIO_init(&gpio_led);

	GPIO_HANDLE_T gpio_btn;
	gpio_btn.ptr_gpio = (GPIO_REG_T *) GPIOC_BASEADDR;
	gpio_btn.gpio_pin_config.num = GPIO_PIN_13;
	gpio_btn.gpio_pin_config.mode = GPIO_MODE_IN;
	gpio_btn.gpio_pin_config.spd = GPIO_OUT_SPD_HIGH;
	gpio_btn.gpio_pin_config.pupdctl = GPIO_PUPD_NPUPD;
	GPIO_peri_clk_ctrl(gpio_btn.ptr_gpio, 1);
	GPIO_init(&gpio_btn);


	while (1)
	{
		if (GPIO_read_pin(gpio_btn.ptr_gpio, GPIO_PIN_13)) {
			delay();	// De-bouncing
			GPIO_toggle_pin(gpio_led.ptr_gpio, GPIO_PIN_0);
			GPIO_toggle_pin(gpio_led.ptr_gpio, GPIO_PIN_7);
		}
	}

}
