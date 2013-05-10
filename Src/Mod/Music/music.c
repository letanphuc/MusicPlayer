
/* File name	: music.c
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
#define PATH_BUF_SIZE           20
#include "music.h"
#include "../../musicplayer.h"
// variable for display oled
tCanvasWidget musicBackground;
tCanvasWidget musicPanel;
tCanvasWidget musicString1;
tCanvasWidget musicString2;
tCanvasWidget musicString3;
tCanvasWidget musicString4;
tCanvasWidget musicString5;


static FIL g_sFileObject1;


unsigned int minute = 0;
unsigned int second = 0;
unsigned int micro_second = 0;

Canvas(
	musicBackground,
	WIDGET_ROOT,
	0,
	&musicPanel,
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

// Panel
Canvas(
	musicPanel,
	&musicBackground, 
	0, 
	&musicString1,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	30,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm14,
	"Music Playing",
	0,
	0
);
		
		// String1
Canvas(
	musicString1,
	&musicBackground, 
	0, 
	&musicString2,
	&g_sFormike128x128x16,
	0,
	110,
	128,
	18,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm12,
	"",
	0,
	0
);

		// String2
Canvas(
	musicString2,
	&musicBackground, 
	0, 
	&musicString3,
	&g_sFormike128x128x16,
	0,
	90,
	128,
	20,
	(CANVAS_STYLE_TEXT  | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm12,
	"",
	0,
	0
);
		// String3
Canvas(
	musicString3,
	&musicBackground, 
	0, 
	&musicString4,
	&g_sFormike128x128x16,
	0,
	70,
	128,
	20,
	(CANVAS_STYLE_TEXT  | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm12,
	"",
	0,
	0
);

		// String4
Canvas(
	musicString4,
	&musicBackground, 
	0, 
	&musicString5,
	&g_sFormike128x128x16,
	0,
	50,
	128,
	20,
	(CANVAS_STYLE_TEXT  | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm12,
	"",
	0,
	0
);

		// String5
Canvas(
	musicString5,
	&musicBackground, 
	0, 
	0,
	&g_sFormike128x128x16,
	0,
	30,
	128,
	20,
	(CANVAS_STYLE_TEXT  | CANVAS_STYLE_FILL ),
	ClrGreen,
	ClrWhite,
	ClrWhite,
	g_pFontCm12,
	"",
	0,
	0
);


/*
 * Function name : check_time_lyric
 * Description:
 *       Check wherther the lyric of a sentences can be displayed or not
 * Return value:
 *       SUCCESSFULL
 *       FAIL 
**/
int check_time_lyric(int mm, int ss, int micro_ss){
	if (( mm > minute ) || (( mm == minute ) && (ss > second)) || ((( mm == minute ) && (ss == second) && (micro_ss >= micro_second))))
		return SUCCESSFULL;
	return FAIL;
}

/*
 * Function name : display_lyric
 * Description:
 * display lyric which is suitable with song is playing
 * Input: * char name        
**/
void display_lyric(char * name_of_file){
	FRESULT fresult;
	unsigned short usBytesRead = 0;
	char str[2], time_detect[9], var[20], strDisplay[5][20];
	int mm,ss,micro_ss,flag_song_title,i,k,j,time_pointer,flag_time_detect;
	
	str[1] = '\0';
	mm = ss = micro_ss = flag_song_title = i = k = j = time_pointer = flag_time_detect = 0;
	sprintf(var, name_of_file);
	WidgetPaint((tWidget *)&musicBackground);
	WidgetMessageQueueProcess();
	
	fresult = f_open(&g_sFileObject1, var, FA_READ);
	if(fresult != FR_OK)
	{
		UARTprintf("\n music 1.c\n");
		return;
	}
	
	while(1){
		for(j = 0;j < 5; j++)
			sprintf( strDisplay[j], "");
		sprintf(time_detect, "");
		flag_time_detect = 0;
		time_pointer = 0;
		i = 0;
		
// Read the first title of this song 
		if (flag_song_title == 0){
			flag_song_title = 1; 
			do{
				fresult = f_read(&g_sFileObject1, str, 1, &usBytesRead);
				strcat(strDisplay[0],str);	
			}while (str[0] != '\n' && fresult == FR_OK && usBytesRead == 1);
		sprintf( strDisplay[0], "");
		}
		
// Read content of lyric each sentences...
		do {
			fresult = f_read(&g_sFileObject1, str, 1, &usBytesRead);
			if ((flag_time_detect) && (str[0] != ']') && (time_pointer <= 7)){
				strcat(time_detect, str);
				time_pointer++;
			}
			if (str[0] == '[') flag_time_detect = 1;
			
// filter content of lyric without time
			if (time_pointer >= 10){
				strcat(strDisplay[i/19], str);
				i++;
			}
			if ((time_pointer >= 8) && (time_pointer < 10))
				if (time_pointer >= 8)
					time_pointer++;
		}while (str[0] != '\n' && fresult == FR_OK && usBytesRead == 1);	
		if (usBytesRead < 1) break;

// check time terminated to display this lyric comment or not?
		mm = (time_detect[0] - 48)*10 + (time_detect[1] - 48);
		ss = (time_detect[3] - 48)*10 + (time_detect[4] - 48);
		micro_ss = (time_detect[6] - 48)*10 +(time_detect[7] - 48);
		while(!check_time_lyric(mm, ss, micro_ss));

// display lyric one sentence and float on oled		
		for (k = 0;k < 5; k++){
			CanvasTextSet(&musicString1, strDisplay[k]);
			if ((k-1) >= 0)
				CanvasTextSet(&musicString2, strDisplay[k-1]);
			if ((k-2) >= 0)
				CanvasTextSet(&musicString3, strDisplay[k-2]);
			if ((k-3) >= 0)
				CanvasTextSet(&musicString4, strDisplay[k-3]);
			if ((k-4) >= 0)
				CanvasTextSet(&musicString5, strDisplay[k-4]);
			WidgetPaint((tWidget *)&musicBackground);
			WidgetMessageQueueProcess();
			delay = delay_help ;
			while (delay >0);
		}
		
// clear all thing in olead LCD
		CanvasTextSet(&musicString1, "");
		CanvasTextSet(&musicString2, "");
		CanvasTextSet(&musicString3, "");	
		CanvasTextSet(&musicString4, "");			
		CanvasTextSet(&musicString5, "");	
		WidgetPaint((tWidget *)&musicBackground);
		WidgetMessageQueueProcess();	
	}			
}

/* Function name: music
 * Description:
 * 		The most important function, it will hold some function of music
 * 		player such as: play, stop, pause,... display lyric,...
 * Return value:
 * 		SUCCESSFULL
 * 		FAIL
 **/
int music(void){	
	int mode = PLAY_SONG_LYRIC;
	playerMP3("Maroon5.mp3");
	switch (mode){
		case CHOOSE_SONG:	
		break;
		
		case PLAY_SONG_LYRIC:	
				display_lyric("MAROON5.TXT");
		break;
		
		case WAITING:
		break;
	}
	return 0;
} 
