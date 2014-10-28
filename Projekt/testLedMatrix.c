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
#include <consol.h>
#include <config.h>
#include <stdlib.h>

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define CRYSTAL_FREQUENCY FOSC
#define PLL_FACTOR        PLL_MUL
#define VPBDIV_FACTOR     PBSD

#define  SPI_CS   0x00008000  //<= new board, old board = 0x00800000

const tU8 kolko[] = {0x3c,0x42,0x81,0x81,0x81,0x81,0x42,0x3c};

const tU8 krzyzyk[] = {0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81};

const tU8 empty[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

tU8 pattern[8] = {0,0,0,0,0,0,0,0};

void ledMatrix(void);


/*****************************************************************************
 *
 * Description:
 *
 ****************************************************************************/
static void
startTimer1(tU16 delayInMs)
{
  //initialize VIC for Timer1 interrupts
  VICIntSelect &= ~0x20;           //Timer1 interrupt is assigned to IRQ (not FIQ)
  VICVectAddr5  = (tU32)ledMatrix; //register ISR address
  VICVectCntl5  = 0x25;            //enable vector interrupt for timer1
  VICIntEnable  = 0x20;            //enable timer1 interrupt
  
  //initialize and start Timer #0
  T1TCR = 0x00000002;                           //disable and reset Timer1
  T1PC  = 0x00000000;                           //no prescale of clock
  T1MR0 = delayInMs *                           //calculate no of timer ticks
         ((CRYSTAL_FREQUENCY * PLL_FACTOR) / (1000 * VPBDIV_FACTOR));
  T1IR  = 0x000000ff;                           //reset all flags before enable IRQs
  T1MCR = 0x00000003;                           //reset counter and generate IRQ on MR0 match
  T1TCR = 0x00000001;                           //start Timer1
}


/*****************************************************************************
 *
 * Description:
 *    A process entry function 
 *
 ****************************************************************************/
void
testLedMatrix(void)
{
 	tU8 cntA = 0;

  PINSEL0 |= 0x00001500 ;  //Initiering av SPI
  SPI_SPCCR = 0x08;    
  SPI_SPCR  = 0x60;
  IODIR0 |= SPI_CS;
  
  startTimer1(2);

  srand(123456);

  for(;;)
  {

		int znak = rand()%100;
	    cntA++;
	    if (cntA > sizeof(kolko)-8)
	      cntA = 0;

	    if(znak > 50)
	    {
	      	pattern[0] = kolko[cntA+0];
	      	pattern[1] = kolko[cntA+1];
	      	pattern[2] = kolko[cntA+2];
	      	pattern[3] = kolko[cntA+3];
	      	pattern[4] = kolko[cntA+4];
	      	pattern[5] = kolko[cntA+5];
	      	pattern[6] = kolko[cntA+6];
	      	pattern[7] = kolko[cntA+7];
	    }
	    else
	    	{
	    	pattern[0] = krzyzyk[cntA+0];
	      	pattern[1] = krzyzyk[cntA+1];
	      	pattern[2] = krzyzyk[cntA+2];
	      	pattern[3] = krzyzyk[cntA+3];
	      	pattern[4] = krzyzyk[cntA+4];
	      	pattern[5] = krzyzyk[cntA+5];
	      	pattern[6] = krzyzyk[cntA+6];
	      	pattern[7] = krzyzyk[cntA+7];
	    	}
	
	osSleep(100);
	
  	pattern[0] = empty[cntA+0];
  	pattern[1] = empty[cntA+1];
  	pattern[2] = empty[cntA+2];
  	pattern[3] = empty[cntA+3];
  	pattern[4] = empty[cntA+4];
  	pattern[5] = empty[cntA+5];
  	pattern[6] = empty[cntA+6];
  	pattern[7] = empty[cntA+7];
	
	osSleep(100);
	}
}
