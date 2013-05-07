
/* File name	: about.c
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
 
#include "about.h"
#include "../../musicplayer.h"
 

tCanvasWidget aboutBackground;
tCanvasWidget aboutPanel;
tCanvasWidget aboutString;
tCanvasWidget aboutString1;
tCanvasWidget aboutString2;
tCanvasWidget aboutString3;





// aboutBackground
Canvas(
	aboutBackground,
	WIDGET_ROOT,
	0,
	&aboutPanel,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	128,
	CANVAS_STYLE_FILL,
	ClrBlue,
	0,
	0,
	0,
	0,
	0,
	0
);

// aboutPanel
Canvas(
	aboutPanel,
	&aboutBackground, 
	0, 
	&aboutString,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	30,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	"About",
	0,
	0
);

// aboutString
Canvas(
	aboutString,
	&aboutBackground, 
	0,
	&aboutString1,
	&g_sFormike128x128x16,
	0,
	30,
	128,
	25,
	(CANVAS_STYLE_TEXT ),
	ClrWhite,
	ClrWhite,
	ClrWhite,
	g_pFontCm14,
	"Music Player v1.00",
	0,
	0
);
// aboutString1
Canvas(
	aboutString1,
	&aboutBackground, 
	0,
	&aboutString2,
	&g_sFormike128x128x16,
	0,
	55,
	128,
	25,
	(CANVAS_STYLE_TEXT ),
	ClrWhite,
	ClrWhite,
	ClrWhite,
	g_pFontCm14,
	"Author: Group PHM",
	0,
	0
);

// aboutString2
Canvas(
	aboutString2,
	&aboutBackground, 
	0,
	&aboutString3,
	&g_sFormike128x128x16,
	0,
	80,
	128,
	25,
	(CANVAS_STYLE_TEXT ),
	ClrWhite,
	ClrWhite,
	ClrWhite,
	g_pFontCm14,
	"Assignment of ES",
	0,
	0
);
// aboutString3
Canvas(
	aboutString3,
	&aboutBackground, 
	0,
	0,
	&g_sFormike128x128x16,
	0,
	105,
	128,
	25,
	(CANVAS_STYLE_TEXT ),
	ClrWhite,
	ClrWhite,
	ClrWhite,
	g_pFontCm14,
	"HCMUT - CSE",
	0,
	0
);



/* Function name: about
 * Description:
 * 		Display developer's information.
 * Return value:
 * 		SUCCESSFUL:
 * 		FAIL
 */

int about(void){
	WidgetPaint((tWidget *)&aboutBackground);
	WidgetMessageQueueProcess();


	while(1){
		waitButton();
		if (SELECT)
			return SUCCESSFULL;
	}
}
