#include "pre_emptive_os/api/osapi.h"
#include "pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <lpc2xxx.h>
#include <consol.h>

#define PROC1_STACK_SIZE 2048
#define PROC2_STACK_SIZE 2048
#define PROC3_STACK_SIZE 2048
#define INIT_STACK_SIZE  400

static tU8 proc1Stack[PROC1_STACK_SIZE];
static tU8 proc2Stack[PROC2_STACK_SIZE];
static tU8 proc3Stack[PROC3_STACK_SIZE];
static tU8 initStack[INIT_STACK_SIZE];
static tU8 pid1;
static tU8 pid2;
static tU8 pid3;

int timerCnt = 0;
int lifes = 3;
int showResult = 0;

static void proc1(void* arg);
static void proc2(void* arg);
static void proc3(void* arg);
static void initProc(void* arg);

void timerAndLedMatrix(void);
void LCD(void);
void PWM(void);

int main(void)
{
	timerCnt = 0;
	tU8 error;
	tU8 pid;

	osInit();
	osCreateProcess(initProc, initStack, INIT_STACK_SIZE, &pid, 1, NULL, &error);
	osStartProcess(pid, &error);

	osStart();
	return 0;
}

static void proc1(void* arg)
{
	tU8  centerReleased = TRUE;
	tU8  i;
	tU32 pattern;
	tU8  error;

	for (;;)
	{
		PINSEL0 &= 0x0000ffff;
		IODIR0 |= 0x0000ff00;
		IOSET0 = 0x0000fb00;
		IOCLR0 = 0x00000400;

		for (i = 0; i < 4; i++)
		{
			pattern = 0x00000100;
			while (pattern <= 0x00008000)
			{
				IOCLR = pattern;
				osSleep(20);
				IOSET0 = pattern;
				IOCLR0 = 0x00000400;

				pattern <<= 1;
			}
		}

		IOCLR0 = 0x00000700;
		osSleep(20);
		IOSET0 = 0x00000400;
		osSleep(20);
		IOCLR0 = 0x00000400;
		osSleep(20);
		IOSET0 = 0x00000400;
		osSleep(20);
		IOCLR0 = 0x00000400;
		osSleep(20);
		IOSET0 = 0x00000400;

		osCreateProcess(proc2, proc2Stack, PROC2_STACK_SIZE, &pid2, 3, NULL, &error);
		osStartProcess(pid2, &error);
		osCreateProcess(proc3, proc3Stack, PROC3_STACK_SIZE, &pid3, 3, NULL, &error);
		osStartProcess(pid3, &error);

		//wait for a short while
		osSleep(100);

		IOPIN &= ~0x001f0000;
		for (;;)
		{
			if ((IOPIN & 0x00010000) == 0)
			{
				if (centerReleased == TRUE)
				{
					centerReleased = FALSE;
				}
			}
			else
			{
				if (centerReleased == FALSE)
				{
					centerReleased = TRUE;
				}
			}

			osSleep(2);
		}
	}
}

static void proc2(void* arg)
{
	timerAndLedMatrix();
}

static void proc3(void* arg)
{
	LCD();
}

static void initProc(void* arg)
{
	tU8 error;

	eaInit();
	osCreateProcess(proc1, proc1Stack, PROC1_STACK_SIZE, &pid1, 3, NULL, &error);
	osStartProcess(pid1, &error);

	osDeleteProcess();
}

void appTick(tU32 elapsedTime)
{
}
