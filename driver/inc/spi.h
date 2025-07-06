#ifndef _SPI_H_
#define _SPI_H_
#include <stdint.h>
#include "stm32f303ze.h"

/**
 * Configuration structure for SPIx peripheral
 */
typedef struct {
    uint8_t devicemode;
    uint8_t busconfig;
    uint8_t sclkspd;
    uint8_t ds;
    uint8_t cpol;
    uint8_t cpha;
    uint8_t ssm;
} SPI_CONFIG_T;

typedef struct {
    SPI_I2S_REG_T *ptr_spi;
    SPI_CONFIG_T spi_pin_config;
} SPI_HANDLE_T;

/**
 * SPI Device mode
 */
#define SPI_DEV_MODE_MASTER 1
#define SPI_DEV_MODE_SLAVE  0

/**
 * SPI Bus configuration
 */
#define SPI_BUS_FULL_DPLX   1       // Full duplex
#define SPI_BUS_HALF_DPLX   2       // Half duplex
#define SPI_BUS_SIMPLX_RX   3       // Simplex, only Rx

/**
 * SPI Clock speed
 */
#define SPI_SCLK_SPD_DIV2   0
#define SPI_SCLK_SPD_DIV4   1
#define SPI_SCLK_SPD_DIV8   2
#define SPI_SCLK_SPD_DIV16  3
#define SPI_SCLK_SPD_DIV32  4
#define SPI_SCLK_SPD_DIV64  5
#define SPI_SCLK_SPD_DIV128 6
#define SPI_SCLK_SPD_DIV256 7

/**
 * SPI Data frame format
 */
typedef enum {
    SPI_DS_4BITS = 3,
    SPI_DS_5BITS = 4,
    SPI_DS_6BITS = 5,
    SPI_DS_7BITS = 6,
    SPI_DS_8BITS = 7,
    SPI_DS_9BITS = 8,
    SPI_DS_10BITS = 9,
    SPI_DS_11BITS = 10,
    SPI_DS_12BITS = 11,
    SPI_DS_13BITS = 12,
    SPI_DS_14BITS = 13,
    SPI_DS_15BITS = 14,
    SPI_DS_16BITS = 15,
} SPI_DATA_SIZE_T;

/**
 * SPI CPOL
 */
#define SPI_CPOL_HIGH   1
#define SPI_CPOL_LOW    0

/**
 * SPI CPHA
 */
#define SPI_CPHA_HIGH   1
#define SPI_CPHA_LOW    0

/**
 * SPI slave select management
 */
#define SPI_SSM_EN  1
#define SPI_SSM_DIS 0


// SPI CR1 Bit definition
#define CR1_CPHA        0
#define CR1_CPOL        1
#define CR1_MSTR        2
#define CR1_BR          3
#define CR1_SPE         6
#define CR1_LSBFIRST    7
#define CR1_SSI         8
#define CR1_SSM         9
#define CR1_RXONLY      10
#define CR1_CRCL        11
#define CR1_CRCNEXT     12
#define CR1_CRCEN       13
#define CR1_BIDIOE      14
#define CR1_BIDIMODE    15

// SPI CR2 Bit definition
#define CR2_RXDMAEN     0
#define CR2_TXDMAEN     1
#define CR2_SSOE        2
#define CR2_NSSP        3
#define CR2_FRF         4
#define CR2_ERRIE       5
#define CR2_RXNEIE      6
#define CR2_TXEIE       7
#define CR2_DS          8
#define CR2_FRXTH       12
#define CR2_LDMA_RX     13
#define CR2_LDMA_TX     14

// SPI SR Bit definition
#define SR_RXNE         0
#define SR_TXE          1
#define SR_CHSIDE       2
#define SR_UDR          3
#define SR_CRCERR       4
#define SR_MODF         5
#define SR_OVR          6
#define SR_BSY          7
#define SR_FRE          8
#define SR_FRLVL        9
#define SR_FTLVL        11

/**
 * APIS
 */

// Init / De-init
void SPI_init(SPI_HANDLE_T *ptr_spi_handle);
void SPI_deinit(SPI_I2S_REG_T *ptr_spi_reg);

// Clock settint
void SPI_peri_clk_ctrl(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable);

// Data Send and Receive
void SPI_send(SPI_I2S_REG_T *ptr_spi_reg, uint8_t *pTxBuffer, uint32_t len);
void SPI_recv(SPI_I2S_REG_T *ptr_spi_reg, uint8_t *pRxBuffer, uint32_t len);

// IRQ
void SPI_irq_config(uint8_t irq_number, uint8_t enable);
void SPI_irq_priority_config(uint8_t irq_number, uint8_t irq_prior);
void SPI(SPI_HANDLE_T *pHandle);

// Other peripheral control
void SPI_peri_ctrl(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable);
void SPI_ssi_config(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable);
#endif