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
testMotor(void)
{
	PINSEL0   =   0x00;     //Configure Port0.7 as GPIO
   IODIR0   =   3 << BUZZ; //Configure Port0.7 as O/P pin
 
	while(1)
	{
     IOSET0 = 1 << BUZZ;
     osSleep(1000);
    IOCLR0 = 1 << BUZZ;
     osSleep(1000);
	}
    
}
