#include "spi.h"


void SPI_init(SPI_HANDLE_T *ptr_spi_handle)
{
    // Peripheral clock enable
    SPI_peri_clk_ctrl(ptr_spi_handle->ptr_spi, 1);

    uint16_t tmp = 0;


    // Configure SPI CR1 register

    // Configure the device mode
    tmp |= ptr_spi_handle->spi_pin_config.devicemode << CR1_MSTR;

    // Configure the Software slave management
    tmp |= ptr_spi_handle->spi_pin_config.ssm << CR1_SSM;

    // Configure the bus
    if (ptr_spi_handle->spi_pin_config.busconfig == SPI_BUS_FULL_DPLX) {
        // Clear bit BIDIMODE
        tmp &= ~(1 << CR1_BIDIMODE);
    } else if (ptr_spi_handle->spi_pin_config.busconfig == SPI_BUS_HALF_DPLX) {
        // Set bit BIDIMODE
        tmp |= (1 << CR1_BIDIMODE);
    } else if (ptr_spi_handle->spi_pin_config.busconfig == SPI_BUS_SIMPLX_RX) {
        // Clear bit BIDIMODE
        tmp &= ~(1 << CR1_BIDIMODE);
        // Set bit RXONLY
        tmp |= (1 << CR1_RXONLY);
    }

    // Configure the SPI serial clock speed (baud rate)
    tmp |= ptr_spi_handle->spi_pin_config.sclkspd << CR1_BR;
    // Configure the CPOL
    tmp |= ptr_spi_handle->spi_pin_config.cpol << CR1_CPOL;
    // Configure the CPHA
    tmp |= ptr_spi_handle->spi_pin_config.cpha << CR1_CPHA;

    // Set
    ptr_spi_handle->ptr_spi->cr1 = 0;

    ptr_spi_handle->ptr_spi->cr1 = tmp;

	// Make NSS signal internally high, otherwise, trigger mode faule (MODF)
	SPI_ssi_config((SPI_I2S_REG_T *)SPI3_I2S3_BASEADDR, 1);
	// Before sending the data, Enable the SPI3 peripheral
	SPI_peri_ctrl((SPI_I2S_REG_T *)SPI3_I2S3_BASEADDR, 1);
    
    // Configure the Data Size
    ptr_spi_handle->ptr_spi->cr2 = ptr_spi_handle->spi_pin_config.ds << CR2_DS;
}
void SPI_deinit(SPI_I2S_REG_T *ptr_spi_reg)
{
    if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI1_BASEADDR) SPI1_REG_RST();
    else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI2_I2S2_BASEADDR) SPI2_REG_RST();
    else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI3_I2S3_BASEADDR) SPI3_REG_RST();
    else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI4_BASEADDR) SPI4_REG_RST();
}

void SPI_peri_clk_ctrl(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable)
{
    if (enable) {
        if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI1_BASEADDR) SPI1_PCLK_EN();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI2_I2S2_BASEADDR) SPI2_PCLK_EN();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI3_I2S3_BASEADDR) SPI3_PCLK_EN();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI4_BASEADDR) SPI3_PCLK_EN();
    } else {
        if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI1_BASEADDR) SPI1_PCLK_DIS();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI2_I2S2_BASEADDR) SPI2_PCLK_DIS();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI3_I2S3_BASEADDR) SPI3_PCLK_DIS();
        else if (ptr_spi_reg == (SPI_I2S_REG_T *) SPI4_BASEADDR) SPI4_PCLK_DIS();

    }
}

uint8_t SPI_GetStatusFlag(SPI_I2S_REG_T *ptr_spi_reg, uint32_t flag)
{
    if (ptr_spi_reg->sr & flag) return 1;
    return 0;
}

void SPI_send(SPI_I2S_REG_T *ptr_spi_reg, uint8_t *pTxBuffer, uint32_t len)
{
    // Blocking

    uint32_t ds = (ptr_spi_reg->cr2 >> 8) & 0x0F;   // Get DS[3:0]
    if (ds < 3) ds = 7; // From RM, If DS[3:0] = "Not used", they are forced to the value "0111"
    uint32_t data_size = ds + 1;

    while (len > 0) {
        // Wait until Tx FIFO empty
        while (!SPI_GetStatusFlag(ptr_spi_reg, (1 << SR_TXE)));
        
        // Check Data Size
        if (data_size <= 8) {
            // Send 1 byte
            ptr_spi_reg->dr = *pTxBuffer;
            pTxBuffer++;
            len--;
        } else if (data_size <= 16) {
            // Send 2 bytes (lower byte send first)
            uint16_t data = *pTxBuffer;
            pTxBuffer++;
            len--;

            if (len > 0) {
                data |= (*pTxBuffer << 8);
                pTxBuffer++;
                len--;
            }

            ptr_spi_reg->dr = data;
        }
    }
}
// void SPI_recv(SPI_I2S_REG_T *ptr_spi_reg, uint8_t *pRxBuffer, uint32_t len)
// {

// }

void SPI_peri_ctrl(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable)
{
    if (enable) {
        ptr_spi_reg->cr1 |= (1 << CR1_SPE);
    } else {
        ptr_spi_reg->cr1 &= ~(1 << CR1_SPE);
    }
}

void SPI_ssi_config(SPI_I2S_REG_T *ptr_spi_reg, uint8_t enable)
{
    if (enable) {
        ptr_spi_reg->cr1 |= (1 << CR1_SSI);
    } else {
        ptr_spi_reg->cr1 &= ~(1 << CR1_SSI);
    }
}


// void SPI_irq_config(uint8_t irq_number, uint8_t enable)
// {

// }
// void SPI_irq_priority_config(uint8_t irq_number, uint8_t irq_prior)
// {

// }
// void SPI(SPI_HANDLE_T *pHandle)
// {

// }

