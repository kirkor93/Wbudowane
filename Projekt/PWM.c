#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>

void PWM(tU8 r, tU8 g, tU8 b)
{
	PINSEL0 &= 0xfff03fff; 
	PINSEL0 |= 0x000a8000;

	PWM_PR = 0x00;   
	PWM_MCR = 0x02; 
	PWM_MR0 = 0x1000;
	//turn off all colors
	PWM_MR2 = 0;
	PWM_MR4 = 0;
	PWM_MR6 = 0;
	PWM_LER = 0x55;

	//turn on colors we want
	PWM_MR2 = r * 3;
	PWM_MR4 = b * 3;       
	PWM_MR6 = g * 3;
	PWM_LER = 0x55;
	PWM_PCR = 0x5400;
	PWM_TCR = 0x09;

}
