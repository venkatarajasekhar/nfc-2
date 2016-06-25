#include "led.h"


void led1_forceon()
{
	LEDREG[0] &= 0x00ff0000;
	LEDREG[0] |= LED1_FCEON;
}

void led1_forceoff()
{
	LEDREG[0] &= 0x00ff0000;
	LEDREG[0] |= ~LED2_POLAR;
	LEDREG[0] |= LED1_POLAR;
}

void led1_blinkon(int rate)
{
	LEDREG[0] &= 0x00ff0000;
	LEDREG[0] |= (LED1_BLINKON + (rate<<8));
}

void led2_forceon()
{
	LEDREG[0] &= 0x0000ff00;
	LEDREG[0] |= LED2_POLAR;
}

void led2_forceoff()
{
	LEDREG[0] &= 0x0000ff00;
	LEDREG[0] &= ~LED2_POLAR;
	LEDREG[0] |= LED2_POLAR;
}


void led2_blinkon(int rate)
{
	LEDREG[0] &= 0x0000ff00;
	LEDREG[0] |= (LED2_BLINKON + (rate<<16));
}

