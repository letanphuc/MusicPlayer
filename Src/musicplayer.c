
/* File name	: musicplayer.c
 * Project 		: MUSIC PLAYER
 * Version 		: 1.0
 * Creation Date: 23/03/2013
 * Tabsize		: 4
 * Author  		: Group PHM                  
 * Copyright	: (c) 2013
 * License		: Free.
 * Comments		: 
 * 
 */
 
#include "musicplayer.h"


#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


unsigned char count1 = 0;
unsigned char count = 0;
unsigned char button = 0;
unsigned int delay;
unsigned int delay_help = 85;
unsigned char ucButtons, ucChanged, ucRepeat;

Time currentTime = {0,0,0};
Alarm alarm;
Date date = {FRI,10,5,2013};

/* check Alarm -right return 1 else return 0 */	
int checkAlarm(){
	if (alarm.timeSet.hour!= currentTime.hour)
		return 0;
	if (alarm.timeSet.minute!= currentTime.minute)
		return 0;
	return 1;
}

/* interrupt function */
void Timer0IntHandler(void)
{
	// Clear the timer interrupt.
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	// Update the interrupt status on the display.
	IntMasterDisable();

	count++;
	count1++;
	if (NEXT)   delay_help+= 10;
	if (PREVIOUS) delay_help-= 10;
	if (delay > 0) delay--;
	micro_second++;
	if (micro_second > 99){
		micro_second = 0;
		second++;
		if (second > 59){
			second = 0;
			minute++;
		}
	}

	/* update realtime */
	currentTime.milisecond+=10;
	if (currentTime.milisecond == 1000){
		currentTime.milisecond = 0;
		currentTime.second++;
		if (currentTime.second == 60){
			currentTime.second = 0;
			currentTime.minute++;
			if (checkAlarm())
				UARTprintf("AAAAAAAAAAAAAAAAAA\n");
			if (currentTime.minute == 60){
				currentTime.hour = (currentTime.hour + 1)%24;
			}
		}
	}
	IntMasterEnable();
}

void Timer1IntHandler(void){
	ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	IntMasterDisable();

	/* Read buttons */
	ucButtons = ButtonsPoll(&ucChanged, &ucRepeat);

	button = 0;
	if(BUTTON_PRESSED(UP_BUTTON, ucButtons, ucChanged)){
		button |= 1<<UP_INDEX;
	}
	if(BUTTON_PRESSED(DOWN_BUTTON, ucButtons, ucChanged)){
		button |= 1<<DOWN_INDEX;
	}
	if(BUTTON_PRESSED(LEFT_BUTTON, ucButtons, ucChanged)){
		button |= 1<<LEFT_INDEX;
	}
	if(BUTTON_PRESSED(RIGHT_BUTTON, ucButtons, ucChanged)){
		button |= 1<<RIGHT_INDEX;
	}
	if(BUTTON_PRESSED(SELECT_BUTTON, ucButtons, ucChanged)){
		button |= 1<<SELECT_INDEX;
	}

	IntMasterEnable();
	
}


/***************************************************************************************
************** 			main function 			****************************************
****************************************************************************************/

int init(void){
	//SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	
	Formike128x128x16Init();
	Formike128x128x16BacklightOn();

	ButtonsInit();
	
	/* init UART */
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioInit(0);
	SysTickPeriodSet(ROM_SysCtlClockGet() / 100);
	SysTickEnable();
	
	/* init VS1011 */
	VS1011_Configure_ControlPinss();
	VS1011_Configure_SSI();
	VS1011_Init();
	playerInit();

	/* initiate timer */
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/100 );   //10 ms
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/100 );   //10 ms

	ROM_IntEnable(INT_TIMER0A);
	ROM_IntEnable(INT_TIMER1A);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	ROM_TimerEnable(TIMER1_BASE, TIMER_A);
	
	IntMasterEnable();

	alarm.isOn = 0;
	alarm.timeSet = currentTime;
	
	return 1;
}



int main(void){
	init();
	
	/* sate machine */
	while(1){
		switch(menu()){
			case MUSIC:
				music();
				break;
			case SETTING:
				setting();
				break;
			case ABOUT:
				about();
				break;
		}
	}
}

