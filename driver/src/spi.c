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

    uint32_t ds = (ptr_spi_reg->cr2 >> 8) & 0x0F;   // Get DS[3:0]
    if (ds < 3) ds = 7; // From RM, If DS[3:0] = "Not used", they are forced to the value "0111"
    uint32_t data_size = ds + 1;

    // Blocking
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
void SPI_recv(SPI_I2S_REG_T *ptr_spi_reg, uint8_t *pRxBuffer, uint32_t len)
{
    // Blocking

    uint32_t ds = (ptr_spi_reg->cr2 >> 8) & 0x0F;   // Get DS[3:0]
    if (ds < 3) ds = 7; // From RM, If DS[3:0] = "Not used", they are forced to the value "0111"
    uint32_t data_size = ds + 1;

    while (len > 0) {
        // Wait until Rx FIFO not empty
        while (!SPI_GetStatusFlag(ptr_spi_reg, (1 << SR_RXNE)));
        
        // Check Data Size
        if (data_size <= 8) {
            // Read 1 Byte from dr
            *pRxBuffer = (uint8_t) ptr_spi_reg->dr;
            pRxBuffer++;
            len--;
        } else if (data_size <= 16) {
            // Send 2 bytes (lower byte send first)
            uint16_t data = ptr_spi_reg->dr;
            *pRxBuffer = (uint8_t) (data & 0xFF);
            pRxBuffer++;
            len--;

            if (len > 0) {
                *pRxBuffer = (uint8_t) ((data >> 8) & 0xFF);
                pRxBuffer++;
                len--;
            }

            ptr_spi_reg->dr = data;
        }
    }
}

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


void SPI_irq_config(uint8_t irq_number, uint8_t enable)
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

/**
 * Non-blocking send
 * This API is not going send anything, it just save the pointer, length and
 * enable the TXEIE interrupt
 */
uint8_t SPI_send_intr(SPI_HANDLE_T *ptr_spi_handle, uint8_t *pTxBuffer, uint32_t len)
{
    uint8_t state = ptr_spi_handle->TxState;

    if (state != SPI_BUSY_TX) {
        // Save the Tx FIFO address and length information in some global variables
        ptr_spi_handle->pTxBuffer = pTxBuffer;
        ptr_spi_handle->TxLen = len;

        // Mark the SPI state as busy in transmission so that
        // no other code can take over same SPI peripheral until transmission is over
        ptr_spi_handle->TxState = SPI_BUSY_TX;

        // Enable the TXEIE control bit to get interrupt whenever TXE flag is set in
        // SR
        ptr_spi_handle->ptr_spi->cr2 |= (1 << CR2_TXEIE);
    }

    // Data Transmission will be handled by the ISR code

    return state;
}

uint8_t SPI_recv_intr(SPI_HANDLE_T *ptr_spi_handle, uint8_t *pRxBuffer, uint32_t len)
{
    uint8_t state = ptr_spi_handle->RxState;

    if (state != SPI_BUSY_RX) {
        // Save the Tx FIFO address and length information in some global variables
        ptr_spi_handle->pRxBuffer = pRxBuffer;
        ptr_spi_handle->RxLen = len;

        // Mark the SPI state as busy in transmission so that
        // no other code can take over same SPI peripheral until transmission is over
        ptr_spi_handle->RxState = SPI_BUSY_RX;

        // Enable the TXEIE control bit to get interrupt whenever TXE flag is set in
        // SR
        ptr_spi_handle->ptr_spi->cr2 |= (1 << CR2_RXNEIE);
    }
    
    // Data Transmission will be handled by the ISR code

    return state;
}

// void SPI_irq_priority_config(uint8_t irq_number, uint8_t irq_prior)
// {

// }

static void spi_txe_intr_handle(SPI_HANDLE_T *ptr_spi_handle)
{
    uint32_t ds = (ptr_spi_handle->ptr_spi->cr2 >> 8) & 0x0F;   // Get DS[3:0]
    if (ds < 3) ds = 7; // From RM, If DS[3:0] = "Not used", they are forced to the value "0111"
    uint32_t data_size = ds + 1;

    // Check Data Size
    if (data_size <= 8) {
        // Send 1 byte
        ptr_spi_handle->ptr_spi->dr = *ptr_spi_handle->pTxBuffer;
        ptr_spi_handle->pTxBuffer++;
        ptr_spi_handle->TxLen--;
    } else if (data_size <= 16) {
        // Send 2 bytes (lower byte send first)
        uint16_t data = *ptr_spi_handle->pTxBuffer;
        ptr_spi_handle->pTxBuffer++;
        ptr_spi_handle->TxLen--;

        if (ptr_spi_handle->TxLen > 0) {
            data |= (*ptr_spi_handle->pTxBuffer << 8);
            ptr_spi_handle->pTxBuffer++;
            ptr_spi_handle->TxLen--;
        }
        ptr_spi_handle->ptr_spi->dr = data;
    }

    if (!ptr_spi_handle->TxLen) {
        // TxLen is zero, close the SPI transmission and inform the
        // application that Tx is over.
        SPI_close_transmission(ptr_spi_handle);

        // Inform application
        spi_app_event_callback(ptr_spi_handle, SPI_EVENT_TX_CMPLT);
    }

}
static void spi_rxne_intr_handle(SPI_HANDLE_T *ptr_spi_handle)
{
    uint32_t ds = (ptr_spi_handle->ptr_spi->cr2 >> 8) & 0x0F;   // Get DS[3:0]
    if (ds < 3) ds = 7; // From RM, If DS[3:0] = "Not used", they are forced to the value "0111"
    uint32_t data_size = ds + 1;

    // Check Data Size
    if (data_size <= 8) {
        // Read 1 Byte from dr
        *ptr_spi_handle->pRxBuffer = (uint8_t) ptr_spi_handle->ptr_spi->dr;
        ptr_spi_handle->pRxBuffer++;
        ptr_spi_handle->RxLen--;
    } else if (data_size <= 16) {
        // Send 2 bytes (lower byte send first)
        uint16_t data = ptr_spi_handle->ptr_spi->dr;
        *ptr_spi_handle->pRxBuffer = (uint8_t) (data & 0xFF);
        ptr_spi_handle->pRxBuffer++;
        ptr_spi_handle->RxLen--;

        if (ptr_spi_handle->RxLen > 0) {
            *ptr_spi_handle->pRxBuffer = (uint8_t) ((data >> 8) & 0xFF);
            ptr_spi_handle->pRxBuffer++;
            ptr_spi_handle->RxLen--;
        }

        ptr_spi_handle->ptr_spi->dr = data;
    }

    if (! ptr_spi_handle->RxLen) {
        // Reception is complete
        SPI_close_reception(ptr_spi_handle);

        // Inform application
        spi_app_event_callback(ptr_spi_handle, SPI_EVENT_RX_CMPLT);
    }
}

static void spi_ovr_intr_handle(SPI_HANDLE_T *ptr_spi_handle)
{
    uint8_t tmp;
    // Clear the OVR flag
    if (ptr_spi_handle->TxState != SPI_BUSY_TX) {
        SPI_clear_ovr(ptr_spi_handle->ptr_spi);
    }
    (void) tmp;  // Identified not used.

    // Inform the application
    spi_app_event_callback(ptr_spi_handle, SPI_EVENT_OVR_ERR);
}

void SPI_irq_handling(SPI_HANDLE_T *ptr_spi_handle)
{
    uint8_t tmp1, tmp2;
    // Check TXE
    tmp1 = ptr_spi_handle->ptr_spi->sr & (1 << SR_TXE);
    tmp2 = ptr_spi_handle->ptr_spi->cr2 & (1 << CR2_TXEIE);

    if (tmp1 && tmp2) {
        // Handle TXE
        spi_txe_intr_handle(ptr_spi_handle);
    }

    // Check RXNE
    tmp1 = ptr_spi_handle->ptr_spi->sr & (1 << SR_RXNE);
    tmp2 = ptr_spi_handle->ptr_spi->cr2 & (1 << CR2_RXNEIE);

    if (tmp1 && tmp2) {
        // Handle RXNE
        spi_rxne_intr_handle(ptr_spi_handle);
    }

    // Check OVR
    tmp1 = ptr_spi_handle->ptr_spi->sr & (1 << SR_OVR);
    tmp2 = ptr_spi_handle->ptr_spi->cr2 & (1 << CR2_ERRIE);    
    if (tmp1 && tmp2) {
        // Handle OVR error
        spi_ovr_intr_handle(ptr_spi_handle);
    }

}


void SPI_clear_ovr(SPI_I2S_REG_T *ptr_spi_reg)
{
    uint8_t tmp;
    // Clear the OVR flag
    tmp = ptr_spi_reg->dr;
    tmp = ptr_spi_reg->sr;
    (void) tmp;  // Identified not used.
}

void SPI_close_transmission(SPI_HANDLE_T *ptr_spi_handle)
{
    ptr_spi_handle->ptr_spi->cr2 &= ~(1 << CR2_TXEIE);
    ptr_spi_handle->pTxBuffer = NULL;
    ptr_spi_handle->TxLen = 0;
    ptr_spi_handle->TxState = SPI_RDY;
}
void SPI_close_reception(SPI_HANDLE_T *ptr_spi_handle)
{
    ptr_spi_handle->ptr_spi->cr2 &= ~(1 << CR2_RXNEIE);
    ptr_spi_handle->pRxBuffer = NULL;
    ptr_spi_handle->RxLen = 0;
    ptr_spi_handle->RxState = SPI_RDY;
}

__WEAK void spi_app_event_callback(SPI_HANDLE_T *ptr_spi_handle, uint8_t event)
{
    // Weak implementation
}
