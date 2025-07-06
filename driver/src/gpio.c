#include "gpio.h"

void GPIO_init(GPIO_HANDLE_T *ptr_gpio_handle)
{
    uint32_t tmp = 0;

    // Enable the peripheral clock
    GPIO_peri_clk_ctrl(ptr_gpio_handle->ptr_gpio, 1);

    // Configure the mode
    if (ptr_gpio_handle->gpio_pin_config.mode <= GPIO_MODE_ANA) {
        tmp = (ptr_gpio_handle->gpio_pin_config.mode << (2 * ptr_gpio_handle->gpio_pin_config.num));
        ptr_gpio_handle->ptr_gpio->moder &= ~(0x3 << (2 * ptr_gpio_handle->gpio_pin_config.num));
        ptr_gpio_handle->ptr_gpio->moder |= tmp;
    } else { // Interrupt mode
        if (ptr_gpio_handle->gpio_pin_config.mode == GPIO_MODE_INT_FALL_TRIG) {
            // Falling Trigger
            EXTI->ftsr1 |= (1 << ptr_gpio_handle->gpio_pin_config.num);     // Configure the FTSR
            EXTI->rtsr1 &= ~(1 << ptr_gpio_handle->gpio_pin_config.num);    // Clear the RTSR
        } else if (ptr_gpio_handle->gpio_pin_config.mode == GPIO_MODE_INT_RISE_TRIG) {
            // Rising Trigger
            EXTI->rtsr1 |= (1 << ptr_gpio_handle->gpio_pin_config.num);     // Configure the RTSR
            EXTI->ftsr1 &= ~(1 << ptr_gpio_handle->gpio_pin_config.num);    // Clear the FTSR
        } else if (ptr_gpio_handle->gpio_pin_config.mode == GPIO_MODE_INT_FALL_RISE_TRIG) {
            // Falling and Rising Trigger
            EXTI->rtsr1 |= (1 << ptr_gpio_handle->gpio_pin_config.num);     // Configure the RTSR
            EXTI->ftsr1 |= (1 << ptr_gpio_handle->gpio_pin_config.num);     // Configure the FTSR
        }
        // Configure GPIO port selection in SYSCFG_EXTICR
        uint8_t tmp1 = ptr_gpio_handle->gpio_pin_config.num / 4;
        uint8_t tmp2 = ptr_gpio_handle->gpio_pin_config.num % 4;
        uint8_t port_code = GPIO_BASEADDR_TO_CODE(ptr_gpio_handle->ptr_gpio);
        SYSCFG_PCLK_EN();
        SYSCFG->exticr[tmp1] = port_code << (tmp2 * 4);
        // Enable the EXTI interrupt delivery using IMR
        EXTI->imr1 |= (1 << ptr_gpio_handle->gpio_pin_config.num);
    }

    tmp = 0;
    // Configure the speed
    tmp = (ptr_gpio_handle->gpio_pin_config.spd << (2 * ptr_gpio_handle->gpio_pin_config.num));
    ptr_gpio_handle->ptr_gpio->ospeedr &= ~(0x3 << (2 * ptr_gpio_handle->gpio_pin_config.num));
    ptr_gpio_handle->ptr_gpio->ospeedr |= tmp;

    tmp = 0;
    // Configure the pupd
    tmp = (ptr_gpio_handle->gpio_pin_config.pupdctl << (2 * ptr_gpio_handle->gpio_pin_config.num));
    ptr_gpio_handle->ptr_gpio->pupdr &= ~(0x3 << (2 * ptr_gpio_handle->gpio_pin_config.num));
    ptr_gpio_handle->ptr_gpio->pupdr |= tmp;

    tmp = 0;
    // Configure the optype
    tmp = (ptr_gpio_handle->gpio_pin_config.optype << (ptr_gpio_handle->gpio_pin_config.num));
    ptr_gpio_handle->ptr_gpio->otyper &= ~(0x1 << ptr_gpio_handle->gpio_pin_config.num);
    ptr_gpio_handle->ptr_gpio->otyper |= tmp;

    tmp = 0;
    // Configure the alt func
    if (ptr_gpio_handle->gpio_pin_config.mode == GPIO_MODE_ALT) {
        uint8_t alt_num = ptr_gpio_handle->gpio_pin_config.num / 8;
        uint8_t alt_ofs = ptr_gpio_handle->gpio_pin_config.num % 8;

        ptr_gpio_handle->ptr_gpio->afr[alt_num] &= ~(0xF << (4 * alt_ofs));
        ptr_gpio_handle->ptr_gpio->afr[alt_num] |= ptr_gpio_handle->gpio_pin_config.altfuncmode << (4 * alt_ofs);
    }

}
void GPIO_deinit(GPIO_REG_T *ptr_gpio_reg)
{
    if (ptr_gpio_reg == (GPIO_REG_T *) GPIOA_BASEADDR) GPIOA_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOB_BASEADDR) GPIOB_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOC_BASEADDR) GPIOC_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOD_BASEADDR) GPIOD_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOE_BASEADDR) GPIOE_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOF_BASEADDR) GPIOF_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOG_BASEADDR) GPIOG_REG_RST();
    else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOH_BASEADDR) GPIOH_REG_RST();
}

void GPIO_peri_clk_ctrl(GPIO_REG_T *ptr_gpio_reg, uint8_t enable)
{
    if (enable) {
        if (ptr_gpio_reg == (GPIO_REG_T *) GPIOA_BASEADDR) GPIOA_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOB_BASEADDR) GPIOB_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOC_BASEADDR) GPIOC_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOD_BASEADDR) GPIOD_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOE_BASEADDR) GPIOE_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOF_BASEADDR) GPIOF_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOG_BASEADDR) GPIOG_PCLK_EN();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOH_BASEADDR) GPIOH_PCLK_EN();
    } else {
        if (ptr_gpio_reg == (GPIO_REG_T *) GPIOA_BASEADDR) GPIOA_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOB_BASEADDR) GPIOB_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOC_BASEADDR) GPIOC_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOD_BASEADDR) GPIOD_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOE_BASEADDR) GPIOE_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOF_BASEADDR) GPIOF_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOG_BASEADDR) GPIOG_PCLK_DIS();
        else if (ptr_gpio_reg == (GPIO_REG_T *) GPIOH_BASEADDR) GPIOH_PCLK_DIS();
    }
}

uint8_t GPIO_read_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin)
{
    uint8_t val;
    val = (uint8_t) (ptr_gpio_reg->idr >> pin) & 0x1;
    return val;
}
uint16_t GPIO_read_port(GPIO_REG_T *ptr_gpio_reg)
{
    uint16_t val;
    val = (uint16_t) (ptr_gpio_reg->idr);
    return val;
}
void GPIO_write_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin, uint8_t val)
{
    if (val) {
        ptr_gpio_reg->odr |= (1 << pin);
    } else {
        ptr_gpio_reg->odr &= ~(1 << pin);
    }
}
void GPIO_write_port(GPIO_REG_T *ptr_gpio_reg, uint16_t val)
{
    ptr_gpio_reg->odr = val;
}
void GPIO_toggle_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin)
{
    ptr_gpio_reg->odr ^= (1 << pin);
}

void GPIO_irq_config(uint8_t irq_number, uint8_t enable)
{
    if (enable) {
        if (irq_number <= 31) {
            // ISER0
            *NVIC_ISER0 |= (1 << irq_number);
        } else if (irq_number > 31 && irq_number < 64) {
            // ISER1
            *NVIC_ISER1 |= (1 << (irq_number % 32));
        } else if (irq_number >= 64 && irq_number < 96) {
            // ISER2
            *NVIC_ISER2 |= (1 << (irq_number % 64));
        }
    } else {
        if (irq_number <= 31) {
            // ICER0
            *NVIC_ICER0 |= (1 << irq_number);
        } else if (irq_number > 31 && irq_number < 64) {
            // ICER1
            *NVIC_ICER1 |= (1 << (irq_number % 32));
        } else if (irq_number >= 64&& irq_number < 96) {
            // ICER2
            *NVIC_ICER2 |= (1 << (irq_number % 64));
        }
    }
}

void GPIO_irq_priority_config(uint8_t irq_number, uint8_t irq_prior)
{
    uint8_t iprx_index = irq_number / 4;
    uint8_t iprx_section = irq_number % 4;

    // For Coretex-M4, 16 programmable priority levels, but 4 bits of 
    // interrupt priority are used.
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PRIOR_BITS_IMPLEMETED);
    *(NVIC_IPR_BASE_ADDR + iprx_index) |= (irq_prior << (shift_amount));
}

void GPIO_irq_handle(uint8_t pin)
{
    // Clear the EXTI PR register corresponding to the pin number
    if (EXTI->pr1 & (1 << pin)) {
        // clear (Pending register is cleared by writing a '1')
        EXTI->pr1 |= (1 << pin);
    }
}
