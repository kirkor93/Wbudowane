//#include <lpc2xxx.h>
//
//
//#define MR1I (1<<0) // przerwijj kiedy TC = MR0
//#define MR1R (1<<1)	// Resetuj TC kiedy TC = MR0
//
//#define PLOCK 0x00000500
//#define DELAY_MS 1000 // 0.5 sceonds delay
//#define PRESCALE 60000 // 60000 PCLK clokc cycles to increment5 TC by 1
//
//void initClocks(void);
//void initTimer1(void);
//void T1ISR(void);
//void setupPLL(void);
//void feedSeq(void);
//void connectPLL(void);
//
//extern int timerCnt;
//
//void timer(void)
//{
//	initClocks();
//	initTimer1();
//	IODIR0 = 0xFFFFFFFF;
//	IOPIN0 = 0x0;
//
//	T1TCR= 0x01;
//
//	while(1);
//}
//
//void initTimer1(void)
//{
//	T1TCR = 0x0;
//	T1PR = PRESCALE -1;
//	T1MR0 = DELAY_MS-1;
//	T1MCR = MR1I | MR1R;
//
//	VICVectAddr5 = (unsigned) T1ISR;
//
//	VICVectCntl5 = 0x20 | 5;
//
//	VICIntEnable = 0x20;
//	T1TCR = 0x02;
//}
//
//void T1ISR()
//{
//	long int regVal;
//	timerCnt +=1;
//	regVal = T1IR;
//	IOPIN0 = ~IOPIN0;
//	T1IR = regVal;
//	VICVectAddr = 0x0;
//}
//
//void setupPLL(void)
//{
//	PLLCON = 0x01;
//	PLLCFG = 0x24;
//}
//
//void feedSeq(void)
//{
//	PLLFEED = 0xAA;
//	PLLFEED = 0x55;
//}
//
//void connectPLL(void)
//{
//	while(!(PLLSTAT & PLOCK));
//	PLLCON = 0x03;
//}
//
//void initClocks(void)
//{
//	setupPLL();
//	feedSeq();
//	connectPLL();
//	feedSeq();
//
//	VPBDIV = 0x01;
//}
