#include "stm32f303ze.h"
#include "gpio.h"

// Connect an external button to PD5 pin and toggle the led whenever
// interrupt is triggered by the button press.
// Interrupt should be triggered during falling edge of button press.

static void delay(void)
{
	for (uint32_t i = .0; i < 500000; i++);
}

void EXTI9_5_IRQHandler(void)
{
    delay();
    GPIO_irq_handle(GPIO_PIN_5);

    // If interrupt trigger, toggle LD1 (push-pull) and LD2 (open-drain)
    GPIO_REG_T * gpiob = (GPIO_REG_T *) GPIOB_BASEADDR;
    GPIO_toggle_pin(gpiob, GPIO_PIN_0);
    GPIO_toggle_pin(gpiob, GPIO_PIN_7);
}

void btn_interrupt()
{
	GPIO_HANDLE_T gpio_btn = {0};
    // Set PB7 as button input, with 
    // - Internal pull-up
    // - Interrupt trigger with falling edge
	gpio_btn.ptr_gpio = (GPIO_REG_T *) GPIOD_BASEADDR;
	gpio_btn.gpio_pin_config.num = GPIO_PIN_5;
	gpio_btn.gpio_pin_config.mode = GPIO_MODE_INT_FALL_TRIG;
	gpio_btn.gpio_pin_config.spd = GPIO_OUT_SPD_HIGH;
	gpio_btn.gpio_pin_config.pupdctl = GPIO_PUPD_PU;
	GPIO_peri_clk_ctrl(gpio_btn.ptr_gpio, 1);
	GPIO_init(&gpio_btn);

    // Configure IRQ
    GPIO_irq_priority_config(IRQ_NO_EXTI9_5, 15);
    GPIO_irq_config(IRQ_NO_EXTI9_5, 1);

	GPIO_HANDLE_T gpio_led = {0};

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

    while (1);    
}
