#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
#include <consol.h>

#define PLOCK 0x00000400
#define MR0I (1<<0) //Przerwanie kiedy time clock jest rowny match register

#define PRESCALE 60000 //60000 cykli procesora zwieksza time clock o 1

int* elapsTime;

void initClocks();
void initTimer0();
__irq void T0ISR();

void setupPLL0();
void feedSeq();
void connectPLL0();

void timer(int* elapsedTime);

void initTimer0()
{
	T0TCR = 0x0;
	
	T0PR = PRESCALE - 1; //Co 60000 cykli procesora zwieksz T0TC o 1
	
	T0MR0 = 499;
	T0MCR = MR0I;
	
	VICVectAddr4 = (unsigned)T0ISR;	//wskaznik na funkcje, ktora ma zostac wywolana
									//jak bedzie przerwanie
									
	VICVectCntl4 = 0x20 | 4; //4 bo taka jest maska VIC Channel dla timera 0
	
	VICIntEnable = 0x10; //Wlaczenie przerwani timera 0
	
	T0TCR = 0x02;
}

__irq void T0ISR()
{
    long int regVal;
    regVal = T0IR;
        
    IO0PIN = ~IO0PIN;

    T0IR = regVal;
    VICVectAddr = 0x0;
	
	(*elapsTime)++;
}

void timer(int* elapsedTime)
{
	elapsTime = elapsedTime;
	initClocks();
	initTimer0();
	
	IODIR0 = 0xFFFFFFFF;
	IOPIN0 = 0xF;
	
	T0TCR = 0x01;
	
	while(1);
}

void initClocks()
{
	setupPLL0();
	feedSeq();
	connectPLL0();
	feedSeq();
	
	VPBDIV = 0x01;
}

void setupPLL0()
{
	PLL0CON = 0x01;
	PLL0CFG = 0x24;
}

void feedSeq()
{
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
}

void connectPLL0()
{
	while(!(PLL0STAT & PLOCK));
	
	PLL0CON = 0x03;
}