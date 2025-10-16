#include "i2c.h"


void I2C_init(I2C_HANDLE_T *ptr_i2c_handle)
{
    ;
}
void I2C_deinit(I2C_REG_T *ptr_i2c_reg)
{
    if (ptr_i2c_reg == (I2C_REG_T *) I2C1_BASEADDR) I2C1_REG_RST();
    else if (ptr_i2c_reg == (I2C_REG_T *) I2C2_BASEADDR) I2C3_REG_RST();
    else if (ptr_i2c_reg == (I2C_REG_T *) I2C3_BASEADDR) I2C3_REG_RST();
}

void I2C_peri_clk_ctrl(I2C_REG_T *ptr_i2c_reg, uint8_t enable)
{
    if (enable) {
        if (ptr_i2c_reg == (I2C_REG_T *) I2C1_BASEADDR) I2C1_PCLK_EN();
        else if (ptr_i2c_reg == (I2C_REG_T *) I2C2_BASEADDR) I2C2_PCLK_EN();
        else if (ptr_i2c_reg == (I2C_REG_T *) I2C3_BASEADDR) I2C3_PCLK_EN();
    } else {
        if (ptr_i2c_reg == (I2C_REG_T *) SPI1_BASEADDR) I2C1_PCLK_DIS();
        else if (ptr_i2c_reg == (I2C_REG_T *) SPI2_I2S2_BASEADDR) I2C1_PCLK_DIS();
        else if (ptr_i2c_reg == (I2C_REG_T *) SPI3_I2S3_BASEADDR) I2C1_PCLK_DIS();

    }
}

uint8_t I2C_GetStatusFlag(I2C_REG_T *ptr_i2c_reg, uint32_t flag)
{
    if (ptr_i2c_reg->isr & flag) return 1;
    return 0;
}

void I2C_irq_config(uint8_t irq_number, uint8_t enable)
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
void I2C_irq_priority_config(uint8_t irq_number, uint8_t irq_prior)
{
    uint8_t iprx_index = irq_number / 4;
    uint8_t iprx_section = irq_number % 4;

    // For Coretex-M4, 16 programmable priority levels, but 4 bits of 
    // interrupt priority are used.
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PRIOR_BITS_IMPLEMETED);
    *(NVIC_IPR_BASE_ADDR + iprx_index) |= (irq_prior << (shift_amount));
}

void I2C_peri_ctrl(I2C_REG_T *ptr_i2c_reg, uint8_t enable)
{
    if (enable) {
        ptr_i2c_reg->cr1 |= (1 << CR1_PE);
    } else {
        ptr_i2c_reg->cr1 &= ~(1 << CR1_PE);
    }
}

void I2C_app_event_callback(I2C_HANDLE_T *ptr_i2c_handle, uint8_t event)
{

}

