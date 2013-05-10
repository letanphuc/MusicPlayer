
/* File name	: musicplayer.h
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

#ifndef	__MUSIC_PLAYER_H__
#define	__MUSIC_PLAYER_H__


/****************** lib header files **************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hw_types.h"

#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/grlib.h"

#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"

#include "drivers/buttons.h"
#include "drivers/class-d.h"
#include "drivers/formike128x128x16.h"
#include "drivers/buttons.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"

#include "fatfs/src/ff.h"
#include "fatfs/src/diskio.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/grlib.h"
#include "grlib/listbox.h"

#include "drivers/buttons.h"
#include "drivers/class-d.h"
#include "drivers/formike128x128x16.h"

/****************** user header files **************************************/
#include "Mod/Menu/menu.h"
#include "Mod/Music/music.h"
#include "Mod/Setting/setting.h"
#include "Mod/About/about.h"

#include "Driver/Vs1011/Vs1011.h"

/* Define */
#define MODE1	1
#define MODE2	2
#define MODE3	3

#define MENU	0
#define MUSIC	1
#define SETTING	2
#define ABOUT	3

#define SUCCESSFULL	0
#define FAIL		1

typedef struct{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned int milisecond;
} Time;

typedef struct{
	unsigned char isOn;
	Time timeSet;
} Alarm;
typedef struct{
	enum{
		MON,	// 0
		TUE,	// 1
		WES,	// 2
		THU,	// 3
		FRI,	// 4
		SAT,	// 5
		SUN		// 6
	} dayOfWeek;
	unsigned char date;
	unsigned char month;
	unsigned int year;
} Date;


extern unsigned char count;
extern unsigned char count1;
extern unsigned int delay;
extern unsigned int delay_help;
extern unsigned char button;
extern Time currentTime;
extern Alarm alarm;
extern Date date;

#define UP_INDEX 		0
#define DOWN_INDEX 		1
#define LEFT_INDEX 		2
#define RIGHT_INDEX 	3
#define SELECT_INDEX 	4


/*
#define UP 			button&(1<<UP_INDEX)
#define DOWN 		button&(1<<DOWN_INDEX)
#define LEFT 		button&(1<<LEFT_INDEX)
#define RIGHT 		button&(1<<RIGHT_INDEX)
#define SELECT 		button&(1<<SELECT_INDEX)
*/

#define NEXT 			button&(1<<UP_INDEX)
#define PREVIOUS 		button&(1<<DOWN_INDEX)
#define BACK 			button&(1<<LEFT_INDEX)
#define SELECT 			button&(1<<RIGHT_INDEX)
#define HOME 			button&(1<<SELECT_INDEX)


#define BUTTON_IS_PRESSED 	button
#define waitButton() 		while(!button)
#define clearButton()		{button = 0;}


#endif


/****************END FILE**************/

