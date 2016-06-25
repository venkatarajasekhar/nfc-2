/**
 * @file NFC.c
 *
 * @author fox.chao@gmail.com
 */

#include "NFC.h"

#include "pt/pt.h"
#include <string.h>

int32_t cmdff_log = 0;

uint16_t NFC_FMICFF_read(void)
{
    do {} while (!get_count(NFC_REG[NFC_FMICFF_SLOT]));

	NFC_FMICFFRD[0] = 0;
	return (uint16_t)NFC_FMICFFRD[0];
}

/**
  * NFC_init 
  *
  * open and grow the simulated target files if necessary.
  *
  */
void NFC_init(void)
{
	// ONFI Modes
	// mode 0: >=50ns
	// mode 1: >=30ns < 50ns
	// mode 2: >=20ns < 30ns
	// mode 3: >=15ns < 20ns
	// mode 4: >=12ns < 15ns
	// mode 5: >=10ns <=12ns

	// Select clocks for different ONFI modes
	// Actual PLL clock is greater than 400MHz
	// mode 0: 400MHz/20 = 20MHz (50ns) ==> suitable for ONFI mode 1
	// mode 1: 400MHz/12 = 33.33MHz (30ns)  ==> suitable for ONFI mode 2
	// mode 2: 400MHz/8  = 50MHz (20ns) ==> suitable for ONFI mode 3
	// mode 3: 400MHz/6  = 66.66MHz (15ns) ==> suitable for ONFI mode 4
	// mode 4: 400MHz/4  = 100 MHz (10ns) ==> suitable for ONFI mode 5
/*
	GPOO[2] = 0x00000008; // mode 0
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x00000000;
	GPOO[1] = 0x00000000;
*/
	GPOO[2] = 0x00000009; // mode 1
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
/*
	GPOO[2] = 0x00000001;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x0000000A; // mode 2
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x00000002;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x0000000B; // mode 3
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x00000003;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x0000000D; // mode 5
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[1] = 0x00000000;
	GPOO[2] = 0x00000005;
	GPOO[1] = 0x00000000;
*/
	//----------------------------------------------------------------
	// This section of code must be present to enable clocks to NFC	
	GCC[0] |= 0x80000;
	//----------------------------------------------------------------
	// This section of code must be present to enable non-CPU access
	// to FTLRAM
	//rd_value= ICS[0];
	//ICS[0] = rd_value | 0x00020;
	//----------------------------------------------------------------
	// kenichi add
//	GMASK[0] = 0x1;			//  enable global interrupt 
//	IRQREG[0] = (1 << 5);   // enable irq 5
	//----------------------------------------------------------------
	// This section of code is to set DQS delay
	DQSDLYSEL1[0] = 0x00000004; // 5ns
	DQSDLYSEL2[0] = 0x00000000;
	//----------------------------------------------------------------
	//Disable reset
	NFC_REG[NFC_RESET] = 0xffffffff;
	NFC_REG[NFC_RESET] = 0;
}
