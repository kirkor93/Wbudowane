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

#define BUZZ 7 // buzzer define

#define  SPI_CS   0x00008000  //<= new board, old board = 0x00800000

const tU8 kolko[] = {0x3c,0x42,0x81,0x81,0x81,0x81,0x42,0x3c};

const tU8 krzyzyk[] = {0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81};

const tU8 krzyzyk2[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


const tU8 empty[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

tU8 pattern[8] = {0,0,0,0,0,0,0,0};

int state = 0;

void ledMatrix(void);


/*****************************************************************************
 *
 * Description:
 *
 ****************************************************************************
*/

#define MR1I (1<<0) // przerwijj kiedy TC = MR0
#define MR1R (1<<1)	// Resetuj TC kiedy TC = MR0

#define PLOCK 0x00000500
#define DELAY_MS 2 // 0.5 sceonds delay
#define PRESCALE 60000 // 60000 PCLK clokc cycles to increment5 TC by 1

void initClocks(void);
void initTimer1(void);
void T1ISR(void);
void setupPLL(void);
void feedSeq(void);
void connectPLL(void);

extern int timerCnt;
extern int lifes;
extern int showResult;

void timer(void)
{
	initClocks();
	initTimer1();

	T1TCR= 0x01;
}

void initTimer1(void)
{
	T1TCR = 0x0;
	T1PR = PRESCALE -1;
	T1MR0 = DELAY_MS-1;
	T1MCR = MR1I | MR1R;
	T1IR  = 0x000000ff;
	VICIntSelect &= ~0x20;
	VICVectAddr5 = (tU32) T1ISR;

	VICVectCntl5 = 0x20 | 5;

	VICIntEnable = 0x20;
	T1TCR = 0x02;
}

void T1ISR()
{
	long int regVal;
	regVal = T1IR;
	timerCnt += 1;
	T1IR = regVal;
	ledMatrix();
}

void setupPLL(void)
{
	PLLCON = 0x01;
	PLLCFG = 0x24;
}

void feedSeq(void)
{
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
}

void connectPLL(void)
{
	while(!(PLLSTAT & PLOCK));
	PLLCON = 0x03;
}

void initClocks(void)
{
	setupPLL();
	feedSeq();
	connectPLL();
	feedSeq();

	VPBDIV = 0x01;
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
  timer();
  IODIR0 |= SPI_CS;

  //testMotor(0);

  srand(123456);
	for( ; ; )
 // while((IOPIN0 & (1<<14)) )
  {
		PINSEL0 |= 0x00001500 ;
		state = 0;
		int znak = rand()%100;
	    cntA++;
	    if (cntA > sizeof(kolko)-8)
	      cntA = 0;
		  
	    //BUTTON START

		//BUTTON END

	    if(lifes > 0)
	    {
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
	    }
	    else
	    {
	    	pattern[0] = krzyzyk2[cntA+0];
	    		      	pattern[1] = krzyzyk2[cntA+1];
	    		      	pattern[2] = krzyzyk2[cntA+2];
	    		      	pattern[3] = krzyzyk2[cntA+3];
	    		      	pattern[4] = krzyzyk2[cntA+4];
	    		      	pattern[5] = krzyzyk2[cntA+5];
	    		      	pattern[6] = krzyzyk2[cntA+6];
	    		      	pattern[7] = krzyzyk2[cntA+7];
	    }
	    //testMotor(1);

	    timerCnt = 0;

	    while(1)
	    {
	    	if(lifes>0)
	    	{
	    		testRGB(255,0,0);
	    		printf("%d\n", timerCnt);
	    		if(timerCnt > 2000)
	    		{
	    			if(pattern[0] == kolko[cntA+0])lifes-=1;
	    			showResult = 1;
	    			break;
	    		}
	    		if( !(IOPIN0 & (1<<14)) ) // Evaluates to True for a 'LOW' on P0.14
					{
						if(pattern[0] == krzyzyk[cntA+0])
						{
							testRGB(0,0,255);
							lifes -= 1;
						}
						else
						{
							testRGB(255,255,255);
						}
						showResult = 1;
						break;
	            }
	    	}
	    	else
	    	{
	    		showResult = 1;
	    		osSleep(1000);
	    		break;
	    	}
	    }

        //testMotor(0);

	
	//while(timerCnt%100 == 0) ;

	    if(lifes <= 0)
	    {
	    	pattern[0] = krzyzyk2[cntA+0];
      	pattern[1] = krzyzyk2[cntA+1];
      	pattern[2] = krzyzyk2[cntA+2];
      	pattern[3] = krzyzyk2[cntA+3];
      	pattern[4] = krzyzyk2[cntA+4];
      	pattern[5] = krzyzyk2[cntA+5];
      	pattern[6] = krzyzyk2[cntA+6];
      	pattern[7] = krzyzyk2[cntA+7];
	    }

	    while(showResult == 1)
	    {
	    	if(lifes <=0)
	    	{
	    		testRGB(255,0,0);
	    	}
	    	osSleep(10);
	    }

	if(lifes > 0)
	{
  	pattern[0] = empty[cntA+0];
  	pattern[1] = empty[cntA+1];
  	pattern[2] = empty[cntA+2];
  	pattern[3] = empty[cntA+3];
  	pattern[4] = empty[cntA+4];
  	pattern[5] = empty[cntA+5];
  	pattern[6] = empty[cntA+6];
  	pattern[7] = empty[cntA+7];
	}


	osSleep(100);
	testRGB(0,0,0);

  	//while(timerCnt % 100 == 0) ;
	}
}
