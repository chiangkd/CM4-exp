#include "gpio.h"



void GPIO_init(GPIO_HANDLE_T *ptr_gpio_handle)
{
    uint32_t tmp = 0;
    // Configure the mode
    if (ptr_gpio_handle->gpio_pin_config.mode <= GPIO_MODE_ANA) {
        tmp = (ptr_gpio_handle->gpio_pin_config.mode << (2 * ptr_gpio_handle->gpio_pin_config.num));
        ptr_gpio_handle->ptr_gpio->moder &= ~(0x3 << (2 * ptr_gpio_handle->gpio_pin_config.num));
        ptr_gpio_handle->ptr_gpio->moder |= tmp;
    } else { // Interrupt mode

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

void GPIO_irq_config(uint8_t irq_number, uint8_t irq_prior, uint8_t enable)
{

}
void GPIO_irq_handle(uint8_t pin)
{

}
