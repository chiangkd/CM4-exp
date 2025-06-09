#include "hsi.h"



/**
 * 000: MCO is divided by 1
 * 001: MCO is divided by 2
 * 010: MCO is divided by 4
 * ...
 * 111: MCO is divided by 128
 */
void setPreDiv(int div)
{
    uint8_t RgSetting = 0;
    if (div == 0 || ((div & (div - 1)) != 0))
        RgSetting = 0;
    else
        RgSetting = __builtin_ctz(div);
    
    uint32_t *pRccCfgrReg = (uint32_t *)(0x40021000 + 0x04);    // Base address + offset
    *pRccCfgrReg &= ~(0x3 << 28);       // clear MCOPRE
    
    *pRccCfgrReg |= RgSetting << 28;    // Set MCOPRE
}

void meaHSI()
{
    uint32_t *pRccCfgrReg = (uint32_t *)(0x40021000 + 0x04);    // Base address + offset
    // 1. Configure RCC_CFGR MCO bit fields to select HSI as clock source
    *pRccCfgrReg |= 0x5 << 24;  // Set [26:24] = 101

    // Configure MCO prescaler
    setPreDiv(128);

    // 2. Configure PA8 to AF0 mode to behave as MCO signal
    // GPIOA is connected to AHB2
    uint32_t *pClkCtrlReg = (uint32_t *)0x40021014;	// RCC_AHBENR
	uint32_t temp = *pClkCtrlReg;	                // Read operation
	temp = temp | (0x1 << 17);              		// Modify (Enable GPIOA EN)
	*pClkCtrlReg = temp;	                        // Write back

	uint32_t *pPAModeReg = (uint32_t *)0x48000000;  // GPIOA MODER
	uint32_t temp2 = *pPAModeReg;
	temp2 = temp2 | (2 << 16);                      // Set PA8 as 'Alternate function mode'
	*pPAModeReg = temp2;

    // 3. Configure the alternation function register to set the mode 0 for PA8
    uint32_t *pPAAfrhReg = (uint32_t *)(0x48000000 + 0x24);  // GPIOA alternate function high register
    *pPAAfrhReg &= ~(0xf << 0);

}