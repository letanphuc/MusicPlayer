
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
 
#include "menu.h"


tCanvasWidget menuBackground;
tCanvasWidget menuString;
tCanvasWidget menuIcon;



// menuBackground
Canvas(
	menuBackground,
	WIDGET_ROOT,
	0,
	&menuString,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	128,
	CANVAS_STYLE_FILL,
	ClrWhite,
	0,
	0,
	0,
	0,
	0,
	0
);

// menuString
Canvas(
	menuString,
	&menuBackground, 
	0,
	&menuIcon, 
	&g_sFormike128x128x16,
	10,
	10,
	108,
	30,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL),
	ClrDarkBlue,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	"Music",
	0,
	0
);

// menuIcon
Canvas(
	menuIcon,
	&menuBackground, 
	0, 
	0,
	&g_sFormike128x128x16,
	24,
	40,
	80,
	80,
	(CANVAS_STYLE_IMG ),
	ClrBlack,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	0,
	musicIcon, //image
	0
);

void updateDisplayMenu(unsigned char mode){
	switch(mode){
		case MUSIC:
			CanvasTextSet(&menuString,"MUSIC");
			CanvasImageSet(&menuIcon,musicIcon);
			break;
		case SETTING:
			CanvasTextSet(&menuString,"SETTING");
			CanvasImageSet(&menuIcon,settingIcon);
			break;
		case ABOUT:
			CanvasTextSet(&menuString,"ABOUT");
			CanvasImageSet(&menuIcon,aboutIcon);
			break;
	}	

	WidgetPaint((tWidget *)&menuBackground);
	WidgetMessageQueueProcess();
}


/* Function name: menu
 * Description:
 * 		First mode will be run, it gives user to choose what is the 
 * 		next mode. Display: music, setting,... and get button value to
 * 		determine what mode is choosen.
 * Return value:
 * 		MUSIC:	mode MUSIC is choosen
 * 		SETTING:
 * 		ABOUT:
 **/ 
int menu(void){
	unsigned int mode = MUSIC;
	
	WidgetPaint((tWidget *)&menuBackground);
	WidgetMessageQueueProcess();

	updateDisplayMenu (mode);

	while(1) {
		waitButton ();
		
		if (SELECT)
			return mode;
		else if (LEFT){
			if (mode == 1) mode = 3;
			else mode--;
		}
		else if (RIGHT){
			if (mode == 3) mode = 1;
			else mode++;
		}
		else continue;

		updateDisplayMenu (mode);
	}
}
