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

#define LCD_DATA      0x00ff0000  //P1.16-P1.23
#define LCD_E         0x02000000  //P1.25
#define LCD_RW        0x00400000  //P0.22
#define LCD_RS        0x01000000  //P1.24
#define LCD_BACKLIGHT 0x40000000  //P0.30

/*****************************************************************************
 *
 * Description:
 *    xxx
 *
 ****************************************************************************/
static void
initLCD(void)
{
	IODIR1 |= (LCD_DATA | LCD_E | LCD_RS);
	IOCLR1  = (LCD_DATA | LCD_E | LCD_RS);

	IODIR0 |= LCD_RW;
	IOCLR0  = LCD_RW;
	
  IODIR0 |= LCD_BACKLIGHT;
  IOCLR0  = LCD_BACKLIGHT;
}

/*****************************************************************************
 *
 * Description:
 *    xxx
 *
 ****************************************************************************/
static void
delay37us(void)
{
	volatile tU32 i;

//Temp test for 140 uS delay
	for(i=0; i<6*2500; i++)
    asm volatile (" nop"); //delay 15 ns x 2500 = about 37 us delay
}

/*****************************************************************************
 *
 * Description:
 *    xxx
 *
 ****************************************************************************/
static void
writeLCD(tU8 reg, tU8 data)
{
	volatile tU8 i;

	if (reg == 0)
	  IOCLR1 = LCD_RS;
	else
	  IOSET1 = LCD_RS;
	  
  IOCLR0 = LCD_RW;
	IOCLR1 = LCD_DATA;
	IOSET1 = ((tU32)data << 16) & LCD_DATA;
//printf("\nwrite: %x", data);
	
	IOSET1 = LCD_E;
//	osSleep(1);
	for(i=0; i<16; i++)
    asm volatile (" nop"); //delay 15 ns x 16 = about 250 ns delay
	IOCLR1 = LCD_E;
//	osSleep(1);
	for(i=0; i<16; i++)
    asm volatile (" nop"); //delay 15 ns x 16 = about 250 ns delay
}

/*****************************************************************************
 *
 * Description:
 *    xxx
 *
 ****************************************************************************/
static void
lcdBacklight(tU8 onOff)
{
	if (onOff == TRUE)
	  IOSET0 = LCD_BACKLIGHT;
	else
	  IOCLR0 = LCD_BACKLIGHT;
}

void writeString(char *caption)
{
	int i = 0;

	for (i = 0; caption[i] != '\0'; i += 1)
	{

		writeLCD(1, caption[i]);
		delay37us();
	}
}

void writeInt(int number)
{
	char caption[20], tmp[20];

	int divisor = 10, counter = 0;
	int prevModulo = 0;
	int modulo = number % divisor;
	tmp[counter] = (char)(modulo + '0');

	while (prevModulo != modulo)
	{
		counter += 1;
		prevModulo = modulo;
		divisor *= 10;
		modulo = number % divisor;
		int i = 0;
		for (i = 10; i < divisor; i *= 10)
		{
			modulo -= modulo % i;
		}
		

		tmp[counter] = (char)((modulo * 10 / divisor) + '0');

	}

	tmp[counter - 1] = '\0';

	int i = 0;

	for (i = counter - 2; i >= 0; i -= 1)
	{
		caption[i] = tmp[counter - 2 - i];
	}

	caption[counter - 1] = '\0';

	writeString(caption);
}

void writeNewLine()
{
	writeLCD(0, 0x80 | 0x40);
	delay37us();
}

void clearLCD()
{
	writeLCD(0, 0x01);
    osSleep(1);
}

/*****************************************************************************
 *
 * Description:
 *    A process entry function 
 *
 ****************************************************************************/
void
testLcd(void)
{
	printf("asdasd \n");
  initLCD();
	lcdBacklight(FALSE);
  osSleep(10);
  for(;;)
  {
 	 	lcdBacklight(TRUE);
    osSleep(50);

    //function set
    writeLCD(0, 0x30);
    osSleep(1);
    writeLCD(0, 0x30);
    delay37us();
    writeLCD(0, 0x30);
    delay37us();

    //function set
    writeLCD(0, 0x38);
    delay37us();

    //display off
    writeLCD(0, 0x08);
    delay37us();

    //display clear
    writeLCD(0, 0x01);
    osSleep(1); //actually only 1.52 mS needed
      
    //display control set
    writeLCD(0, 0x06);
    osSleep(1);

    //display control set
    writeLCD(0, 0x0c);
    delay37us();

    //cursor home

    writeLCD(0, 0x02);
  osSleep(1);
	
	writeString("trolol");
	writeNewLine();
	writeInt(12345);

    //move curstor to second row
  /*  writeLCD(0, 0x80 | 0x40);
    */

    osSleep(100000);

    //display clear
    //clearLCD();
  }
	lcdBacklight(FALSE);
}


