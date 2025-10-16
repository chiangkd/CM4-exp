#ifndef _USART_H_
#define _USART_H_
#include <stdint.h>
#include "stm32f303ze.h"

/**
 * Configuration structure for USARTx peripheral
 */
typedef struct {
    uint8_t mode;
    uint32_t baudrate;
    uint8_t no_of_stopbit;
    uint8_t length;
    uint8_t parity;
    uint8_t hw_flow_ctrl;
} USART_CONFIG_T;

typedef struct {
    USART_REG_T *ptr_usart;
    USART_CONFIG_T usart_pin_config;
} USART_HANDLE_T;

/**
 * USART Mode
 */
#define USART_MODE_ONLY_TX  0
#define USART_MODE_ONLY_RX  1
#define USART_MODE_TXRX     2

/**
 * USART Standard Baud rate
 */
#define USART_BAUD_1200     1200
#define USART_BAUD_2400     2400
#define USART_BAUD_9600     9600
#define USART_BAUD_19200    19200
#define USART_BAUD_38400    38400
#define USART_BAUD_57600    57600
#define USART_BAUD_115200   115200
#define USART_BAUD_230400   230400
#define USART_BAUD_460800   460800
#define USART_BAUD_921600   921600
#define USART_BAUD_2M       2000000
#define USART_BAUD_3M       3000000

/**
 * USART Parity
 */
#define USART_PARITY_EN_ODD     2
#define USART_PARITY_EN_EVEN    1
#define USART_PARITY_DISABLE    0

/**
 * USART Word length
 */
#define USART_WDLEN_7BITS   0
#define USART_WDLEN_8BITS   1
#define USART_WDLEN_9BITS   2

/**
 * USART Number of Stop bits
 */
#define USART_STOPBITS_1    0
#define USART_STOPBITS_0_5  1
#define USART_STOP_BITS_2   2
#define USART_STOP_BITS_1_5 3

/**
 * USART Hardware flow control
 */
#define USART_HW_FLOW_CTRL_NONE     0
#define USART_HW_FLOW_CTRL_CTS      1
#define USART_HW_FLOW_CTRL_RTS      2
#define USART_HW_FLOW_CTRL_CTS_RTS  3

// USART CR1 Bit definition
#define CR1_UE      0
#define CR1_UESM    1
#define CR1_RE      2
#define CR1_TE      3
#define CR1_IDLEIE  4
#define CR1_RXNEIE  5
#define CR1_TCIE    6
#define CR1_TXEIE   7
#define CR1_PEIE    8
#define CR1_PS      9
#define CR1_PCE     10
#define CR1_WAKE    11
#define CR1_M0      12
#define CR1_MME     13
#define CR1_CMIE    14
#define CR1_OVER8   15
#define CR1_DEDT    16
#define CR1_DEAT    21
#define CR1_RTOIE   26
#define CR1_EOBIE   27
#define CR1_M1      28

// USART CR2 Bit definition
#define CR2_ADDM7       4
#define CR2_LBDL        5
#define CR2_LBDIE       6
#define CR2_LBCL        8
#define CR2_CPHA        9
#define CR2_CPOL        10
#define CR2_CLKEN       11
#define CR2_STOP        12
#define CR2_LINEN       14
#define CR2_SWAP        15
#define CR2_RXINV       16
#define CR2_TXINV       17
#define CR2_DATAINV     18
#define CR2_MSBFIRST    19
#define CR2_ABREN       20
#define CR2_ABRMOD      21
#define CR2_RTOEN       23
#define CR2_ADD         24

// USART CR3 Bit definition
#define CR3_EIE         0
#define CR3_IREN        1
#define CR3_IRLP        2
#define CR3_HDSEL       3
#define CR3_NACK        4
#define CR3_SCEN        5
#define CR3_DMAR        6
#define CR3_DMAT        7
#define CR3_RTSE        8
#define CR3_CTSE        9
#define CR3_CTSIE       10
#define CR3_ONEBIT      11
#define CR3_OVRDIS      12
#define CR3_DDRE        13
#define CR3_DEM         14
#define CR3_DEP         15
#define CR3_SCARCNT0    17
#define CR3_SCARCNT1    18
#define CR3_SCARCNT2    19
#define CR3_WUS0        20
#define CR3_WUS1        21
#define CR3_WUFIE       22

// USART ISR Bit definition
#define ISR_PE      0
#define ISR_FE      1
#define ISR_NF      2
#define ISR_ORE     3
#define ISR_IDLE    4
#define ISR_RXNE    5
#define ISR_TC      6
#define ISR_TXE     7
#define ISR_LBDF    8
#define ISR_CTSIF   9
#define ISR_CTS     10
#define ISR_RTOF    11
#define ISR_EOBF    12
#define ISR_ABRE    14
#define ISR_ABRF    15
#define ISR_BUSY    16
#define ISR_CMF     17
#define ISR_SBKF    18
#define ISR_RWU     19
#define ISR_WUF     20
#define ISR_TEACK   21
#define ISR_REACK   22

/**
 * APIS
 */

// Init / De-init
void USART_init(USART_HANDLE_T *ptr_usart_handle);
void USART_deinit(USART_REG_T *ptr_usart_reg);

// Data Send and Receive
void USART_send(USART_HANDLE_T *ptr_usart_handle, uint8_t *pTxBuffer, uint32_t len);
void USART_recv(USART_HANDLE_T *ptr_usart_handle, uint8_t *pRxBuffer, uint32_t len);

// Interrupt-triggered Send and Receive
uint8_t USART_send_intr(USART_HANDLE_T *ptr_usart_handle, uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_recv_intr(USART_HANDLE_T *ptr_usart_handle, uint8_t *pRxBuffer, uint32_t len);


// Clock setting
void USART_peri_clk_ctrl(USART_REG_T *ptr_usart_reg, uint8_t enable);

// IRQ
void USART_irq_config(uint8_t irq_number, uint8_t enable);
void USART_irq_priority_config(uint8_t irq_number, uint8_t irq_prior);
void USART_irq_handling(USART_HANDLE_T *ptr_usart_handle);

// Other peripheral control
void USART_peri_ctrl(USART_REG_T *ptr_usart_reg, uint8_t enable);

void USART_set_baudrate(USART_REG_T *ptr_usart_reg, uint32_t baudrate);

#endif