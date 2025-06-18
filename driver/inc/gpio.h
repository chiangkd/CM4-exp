#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdint.h>
#include "stm32f303ze.h"

typedef struct {
    uint8_t num;
    uint8_t mode;
    uint8_t spd;
    uint8_t pupdctl;
    uint8_t optype;
    uint8_t altfuncmode;
} GPIO_CONFIG_T;

typedef struct {
    GPIO_REG_T *ptr_gpio;
    GPIO_CONFIG_T gpio_pin_config;
} GPIO_HANDLE_T;

/**
 * GPIO pin numbers
 */
#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

/**
 * GPIO port modes
 */
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1
#define GPIO_MODE_ALT 2
#define GPIO_MODE_ANA 3

// Interrupt modes
#define GPIO_MODE_INT_FALL_TRIG 4
#define GPIO_MODE_INT_RISE_TRIG 5
#define GPIO_MODE_INT_FALL_RISE_TRIG 6

/**
 * GPIO port output
 */
#define GPIO_OUT_TYPE_PP 0      // push-pull
#define GPIO_OUT_TYPE_OD 1      // open-drain

/**
 * GPIO port output speed
 */
#define GPIO_OUT_SPD_LOW 0
#define GPIO_OUT_SPD MED 1
#define GPIO_OUT_SPD_HIGH 3

/**
 * GPIO port pull-up/pull-down
 */
#define GPIO_PUPD_NPUPD 0       // No pull-up, pull-down
#define GPIO_PUPD_PU 1          // Pull-up
#define GPIO_PUPD_PD 2          // Pull-down
#define GPIO_PUPD_RES 3         // Reserved

/**
 * APIs
 */

// Init/De-init
void GPIO_init(GPIO_HANDLE_T *ptr_gpio_handle);
void GPIO_deinit(GPIO_REG_T *ptr_gpio_reg);


// Clock setting
void GPIO_peri_clk_ctrl(GPIO_REG_T *ptr_gpio_reg, uint8_t enable);


// Data read and write
uint8_t GPIO_read_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin);
uint16_t GPIO_read_port(GPIO_REG_T *ptr_gpio_reg);
void GPIO_write_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin, uint8_t val);
void GPIO_write_port(GPIO_REG_T *ptr_gpio_reg, uint16_t val);
void GPIO_toggle_pin(GPIO_REG_T *ptr_gpio_reg, uint8_t pin);

// IRQ
void GPIO_irq_config(uint8_t irq_number, uint8_t enable);
void GPIO_irq_priority_config(uint8_t irq_number, uint8_t irq_prior);
void GPIO_irq_handle(uint8_t pin);

#endif
