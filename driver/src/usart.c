#include "usart.h"


void USART_init(USART_HANDLE_T *ptr_usart_handle)
{
    uint32_t tmp;

    USART_peri_clk_ctrl(ptr_usart_handle->ptr_usart, 1);

    // Congifure CR1
    // Congigure the Tx Rx mode
    if (ptr_usart_handle->usart_pin_config.mode == USART_MODE_ONLY_RX) {
        tmp |= (1 << CR1_RE);
    } else if (ptr_usart_handle->usart_pin_config.mode == USART_MODE_ONLY_TX) {
        tmp |= (1 << CR1_TE);
    } else if (ptr_usart_handle->usart_pin_config.mode == USART_MODE_TXRX) {
        tmp |= (1 << CR1_RE);
        tmp |= (1 << CR1_TE);
    }
    
    // Configure the word length
    // M[1:0] = 00: 1 Start bit, 8 data bits, n stop bits
    // M[1:0] = 01: 1 Start bit, 9 data bits, n stop bits
    // M[1:0] = 10: 1 Start bit, 7 data bits, n stop bits
    if (ptr_usart_handle->usart_pin_config.length == USART_WDLEN_7BITS) {
        tmp &= ~(1 << CR1_M0);
        tmp |= (1 << CR1_M1);
    } else if (ptr_usart_handle->usart_pin_config.length == USART_WDLEN_8BITS) {
        tmp &= ~(1 << CR1_M0);
        tmp &= ~(1 << CR1_M1);
    } else if (ptr_usart_handle->usart_pin_config.length == USART_WDLEN_9BITS) {
        tmp |= (1 << CR1_M0);
        tmp &= ~(1 << CR1_M1);
    }

    // Configure the parity
    if (ptr_usart_handle->usart_pin_config.parity == USART_PARITY_EN_EVEN) {
        tmp |= (1 << CR1_PCE);
        tmp &= ~(1 << CR1_PS);
    } else if (ptr_usart_handle->usart_pin_config.parity == USART_PARITY_EN_ODD) {
        tmp |= (1 << CR1_PCE);
        tmp |= (1 << CR1_PS);
    }
    ptr_usart_handle->ptr_usart->cr1 = tmp;

    // Configure CR2
    // Configure Stop bits
    tmp = 0;
    tmp |= ptr_usart_handle->usart_pin_config.no_of_stopbit << CR2_STOP;
    ptr_usart_handle->ptr_usart->cr2 = tmp;

    // Configure CR3
    tmp = 0;
    if (ptr_usart_handle->usart_pin_config.hw_flow_ctrl == USART_HW_FLOW_CTRL_CTS) {
        tmp |= (1 << CR3_CTSE);
    } else if (ptr_usart_handle->usart_pin_config.hw_flow_ctrl == USART_HW_FLOW_CTRL_RTS) {
        tmp |= (1 << CR3_RTSE);
    } else if (ptr_usart_handle->usart_pin_config.hw_flow_ctrl == USART_HW_FLOW_CTRL_CTS_RTS) {
        tmp |= (1 << CR3_CTSE);
        tmp |= (1 << CR3_RTSE);
    }
    ptr_usart_handle->ptr_usart->cr3 = tmp;

    // Configure BRR


}
void USART_deinit(USART_REG_T *ptr_usart_reg)
{
    if (ptr_usart_reg == (USART_REG_T *) USART1_BASEADDR) USART1_REG_RST();
    else if (ptr_usart_reg == (USART_REG_T *) USART2_BASEADDR) USART2_REG_RST();
    else if (ptr_usart_reg == (USART_REG_T *) USART3_BASEADDR) USART3_REG_RST();
    else if (ptr_usart_reg == (USART_REG_T *) UART4_BASEADDR) UART4_REG_RST();
    else if (ptr_usart_reg == (USART_REG_T *) UART5_BASEADDR) UART5_REG_RST();
}
uint8_t USART_GetStatusFlag(USART_REG_T *ptr_usart_reg, uint32_t flag)
{
    if (ptr_usart_reg->isr & flag) return 1;
    return 0;
}

void USART_send(USART_HANDLE_T *ptr_usart_handle, uint8_t *pTxBuffer, uint32_t len)
{
    uint16_t *pdata;

    for (uint32_t i = 0; i < len; i++) {
        while (!(USART_GetStatusFlag(ptr_usart_handle->ptr_usart, ISR_TXE)));

        // Check word length
        if (ptr_usart_handle->usart_pin_config.length == USART_WDLEN_9BITS) {
            // If 9 Bit load the DR with 2 bytes masking the bits other than first 9 bits
            pdata = (uint16_t *)pTxBuffer;
            ptr_usart_handle->ptr_usart->tdr = (*pdata & (uint16_t)0x1FF);

            // Check parity
            if (ptr_usart_handle->usart_pin_config.parity == USART_PARITY_DISABLE) {
                // No parity is used in this transfer, so 9 bits of user data will be
                // sent.
                pTxBuffer++;
                pTxBuffer++;
            } else {
                // Parity bit is used in this transfer. So 8 bits of user data will be sent
                pTxBuffer++;
            }
        } else {
            // 7 bits or 8 bits transfer
            ptr_usart_handle->ptr_usart->tdr = (*pTxBuffer & (uint8_t) 0xFF);
            pTxBuffer++;
        }
    }

    // Wait till TC flag
    while (!(USART_GetStatusFlag(ptr_usart_handle->ptr_usart, ISR_TC)));
}


void USART_recv(USART_HANDLE_T *ptr_usart_handle, uint8_t *pRxBuffer, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        // Wait RXNE
        while(!(USART_GetStatusFlag(ptr_usart_handle->ptr_usart, ISR_RXNE)));

        // Check word length
        if (ptr_usart_handle->usart_pin_config.length == USART_WDLEN_9BITS) {
            // Check parity
            if (ptr_usart_handle->usart_pin_config.parity == USART_PARITY_DISABLE) {
                *((uint16_t*)pRxBuffer) = (ptr_usart_handle->ptr_usart->rdr & (uint16_t)0x1FF);

                pRxBuffer++;
                pRxBuffer++;
            } else {
                *pRxBuffer = (ptr_usart_handle->ptr_usart->rdr & (uint8_t)0xFF);
            }
        } else {
            if (ptr_usart_handle->usart_pin_config.parity == USART_PARITY_DISABLE) {
                *pRxBuffer = (ptr_usart_handle->ptr_usart->rdr & (uint8_t)0xFF);
            } else {
                // Parity is used. 7 bits will be of user data and 1 bit is parity
                *pRxBuffer = (uint8_t) 0x7F;
            }
            pRxBuffer++;
        }
    }
}

// Interrupt-triggered Send and Receive
uint8_t USART_send_intr(USART_HANDLE_T *ptr_usart_handle, uint8_t *pTxBuffer, uint32_t len)
{

}
uint8_t USART_recv_intr(USART_HANDLE_T *ptr_usart_handle, uint8_t *pRxBuffer, uint32_t len)
{

}


// Clock setting
void USART_peri_clk_ctrl(USART_REG_T *ptr_usart_reg, uint8_t enable)
{
    if (enable) {
        if (ptr_usart_reg == (USART_REG_T *) USART1_BASEADDR) USART1_PCLK_EN();
        else if (ptr_usart_reg == (USART_REG_T *) USART2_BASEADDR) USART2_PCLK_EN();
        else if (ptr_usart_reg == (USART_REG_T *) USART3_BASEADDR) USART3_PCLK_EN();
        else if (ptr_usart_reg == (USART_REG_T *) UART4_BASEADDR) UART4_PCLK_EN();
        else if (ptr_usart_reg == (USART_REG_T *) UART5_BASEADDR) UART5_PCLK_EN();
    } else {
        if (ptr_usart_reg == (USART_REG_T *) USART1_BASEADDR) USART1_PCLK_DIS();
        else if (ptr_usart_reg == (USART_REG_T *) USART2_BASEADDR) USART2_PCLK_DIS();
        else if (ptr_usart_reg == (USART_REG_T *) USART3_BASEADDR) USART3_PCLK_DIS();
        else if (ptr_usart_reg == (USART_REG_T *) UART4_BASEADDR) UART4_PCLK_DIS();
        else if (ptr_usart_reg == (USART_REG_T *) UART5_BASEADDR) UART5_PCLK_DIS();
    }
}



void USART_irq_config(uint8_t irq_number, uint8_t enable)
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
void USART_irq_priority_config(uint8_t irq_number, uint8_t irq_prior)
{
    uint8_t iprx_index = irq_number / 4;
    uint8_t iprx_section = irq_number % 4;

    // For Coretex-M4, 16 programmable priority levels, but 4 bits of 
    // interrupt priority are used.
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PRIOR_BITS_IMPLEMETED);
    *(NVIC_IPR_BASE_ADDR + iprx_index) |= (irq_prior << (shift_amount));
}

void USART_irq_handling(USART_HANDLE_T *ptr_usart_handle)
{

}

void USART_peri_ctrl(USART_REG_T *ptr_usart_reg, uint8_t enable)
{
    if (enable) {
        ptr_usart_reg->cr1 |= (1 << CR1_UE);
    } else {
        ptr_usart_reg->cr1 &= ~(1 << CR1_UE);
    }
}

void USART_set_baudrate(USART_REG_T *ptr_usart_reg, uint32_t baudrate)
{
    // TBD
}
