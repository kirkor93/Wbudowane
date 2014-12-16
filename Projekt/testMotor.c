/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 *****************************************************************************/


#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
 
#define BUZZ 7

/*****************************************************************************
 *
 * Description:
 *    A process entry function. 
 *
 ****************************************************************************/
void
testMotor(tU8 p)
{
	/*tU8  continueTest;
	tU32 counter = 0;
	tU8  direction = 0;

  PINSEL0 |= 0x00008000;
  IODIR0   |=   3 << 7; //Configure Port0.7 as O/P pin
  IOCLR0   |= 3 << 7;  //set P0.10 low = disable motor

  //PULSE WIDTH MODULATION INIT*********************************************
  PWM_PR  = 0x00;    // Prescale Register
  PWM_MCR = 0x02;    // Match Control Register
  PWM_MR0 = 0x1000;    // TOTAL PERIODTID   T
  PWM_MR2 = 0x0000;    // HÖG SIGNAL        t
  PWM_LER = 0x04;    // Latch Enable Register
  PWM_PCR = 0x5000;  // Prescale Counter Register PWMENA4, PWMENA6
  PWM_TCR = 0x09;    // Counter Enable och PWM Enable
  //************************************************************************

  PWM_MR2=p*500;
  PWM_LER=0x04;*/

	IODIR0  |=   3 << 7;

	if(p == 0)
	{
        IOSET0 = 1 << 7;
	}
	else
	{
        IOCLR0 = 1 << 7;
	}
}
