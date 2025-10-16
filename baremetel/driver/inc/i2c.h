#ifndef _I2C_H_
#define _I2C_H_
#include <stdint.h>
#include "stm32f303ze.h"

/**
 * Configuration structure for I2Cx peripheral
 */
typedef struct {
    uint32_t scl_speed;
    uint8_t device_address;
    uint8_t ack_ctrl;
    uint8_t fm_duty_cycle;
} I2C_CONFIG_T;

typedef struct {
    I2C_REG_T *ptr_i2c;
    I2C_CONFIG_T i2c_pin_config;
} I2C_HANDLE_T;

/**
 * I2C SCL speed
 */
#define I2C_SCL_SPD_SM      100000  // Sandard mode, 100KHz
#define I2C_SCL_SPD_FM4K    400000  // Fast mode, 400KHz
#define I2C_SCL_SPD_FM2K    200000  // Fast mode, 200KHz

/**
 * I2C ACK control (I2C v1)
 */
#define I2C_ACK_EN      1
#define I2C_ACK_DIS     0

/**
 * I2C FM Duty Cycle (I2C v1)
 */
#define I2C_DUTY_2
#define I2C_DUTY_16_9

// I2C CR1 Bit definition
#define CR1_PE          0
#define CR1_TXIE        1
#define CR1_RXIE        2
#define CR1_ADDRIE      3
#define CR1_NACKIE      4
#define CR1_STOPIE      5
#define CR1_TCIE        6
#define CR1_ERRIE       7
#define CR1_DNF         8
#define CR1_ANFOFF      12
#define CR1_TXDMAEN     14
#define CR1_RXDMAEN     15
#define CR1_SBC         16
#define CR1_NOSTRETCH   17
#define CR1_WUPEN       18
#define CR1_GCEN        19
#define CR1_SMBHEN      20
#define CR1_SMBDEN      21
#define CR1_ALERTEN     22
#define CR1_PECEN       23

// I2C CR2 Bit definition
#define CR2_SADD        0
#define CR2_RD_WRN      10
#define CR2_ADD10       11
#define CR2_HEAD10R     12
#define CR2_START       13
#define CR2_STOP        14
#define CR2_NACK        15
#define CR2_NBYTES      16
#define CR2_RELOAD      24
#define CR2_AUTOEND     25
#define CR2_PECBYTE     26

// I2C ISR Bit definition
#define ISR_TXE         0
#define ISR_TXIS        1
#define ISR_RXNE        2
#define ISR_ADDR        3
#define ISR_NACKF       4
#define ISR_STOPF       5
#define ISR_TC          6
#define ISR_TCR         7
#define ISR_BERR        8
#define ISR_ARLO        9
#define ISR_OVR         10
#define ISR_PECERR      11
#define ISR_TIMEOUT     12
#define ISR_ALERT       13
#define ISR_BUSY        15
#define ISR_DIR         16
#define ISR_ADDCODE     17

/**
 * APIS
 */

// Init / De-init
void I2C_init(I2C_HANDLE_T *ptr_i2c_handle);
void I2C_deinit(I2C_REG_T *ptr_i2c_reg);

// Clock setting
void I2C_peri_clk_ctrl(I2C_REG_T *ptr_i2c_reg, uint8_t enable);

// Data Send and Receive

// IRQ
void I2C_irq_config(uint8_t irq_number, uint8_t enable);
void I2C_irq_priority_config(uint8_t irq_number, uint8_t irq_prior);

// Other peripheral control
void I2C_peri_ctrl(I2C_REG_T *ptr_i2c_reg, uint8_t enable);

/**
 * Application callback (implemented by application)
 */
void I2C_app_event_callback(I2C_HANDLE_T *ptr_i2c_handle, uint8_t event);


#endif