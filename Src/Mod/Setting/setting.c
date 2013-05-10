
/* File name	: setting.h
 * Project 		: MUSIC PLAYER
 * Version 		: 1.0
 * Creation Date: 23/03/2013
 * Tab size		: 4
 * Author  		: Group PHM                  
 * Copyright	: (c) 2013
 * License		: Free.
 * Comments		: 
 * 
 */
 
#include "setting.h"
#include "../../musicplayer.h"

/* sates for sate machine */
typedef enum {
	SETTING_OPTION,

	/* 1 2 3*/
	ALARM_ON_OFF,
	ALARM_TIME_HOUR,
	ALARM_TIME_MIN,

	/* 4 5 */
	HOUR,
	MINUTE,

	/* 6 7 8 9 */
	DAY_OF_WEEK,
	DATE,
	MONTH,
	YEAR
} settingState;

/* variable for graphic display */
tCanvasWidget setting_Background;
tCanvasWidget setting_PanelOption;
tListBoxWidget setting_ListOption;
tCanvasWidget setting_Question;
tCanvasWidget setting_Value;



/******************* BACKGROUND *************************************/
Canvas(
	setting_Background,
	WIDGET_ROOT,
	0,
	0,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	128,
	CANVAS_STYLE_FILL,
	ClrGreen,
	0,
	0,
	0,
	0,
	0,
	0
);

/************** SETTING_OPTION *************************************/
Canvas(
	setting_PanelOption,
	0,
	0,
	0,
	&g_sFormike128x128x16,
	0,
	0,
	128,
	30,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL),
	ClrDarkBlue,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	"SETTING",
	0,
	0
);

char setting_ListOptionData[5][16];
char setting_ListOptionData1[5][16];;
ListBox(
	setting_ListOption,
	0,
	0,
	0,
	&g_sFormike128x128x16,
	0,
	30,
	128,
	98,
	LISTBOX_STYLE_OUTLINE,
	ClrWhite,
	ClrBlue,
	ClrBlack,
	ClrWhite,
	ClrBlue,
	g_pFontCm22,
	(const char**)setting_ListOptionData1,
	3,
	0,
	0
);

/************** OTHER ***********************************/
Canvas(
	setting_Question,
	0,
	0,
	0,
	&g_sFormike128x128x16,
	0,
	30,
	128,
	30,
	(CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL),
	ClrDarkBlue,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	"question",
	0,
	0
);
Canvas(
	setting_Value,
	0,
	0,
	0,
	&g_sFormike128x128x16,
	0,
	70,
	128,
	30,
	(CANVAS_STYLE_TEXT ),
	ClrDarkBlue,
	ClrWhite,
	ClrWhite,
	g_pFontCm22b,
	"value",
	0,
	0
);




/************** function ***************************************/
settingState state = SETTING_OPTION;
unsigned char selection = 0;

/* if exit signal return 1 else return 0 */
int settingStateMachine(){
	if (HOME)
		return 1;
	
	switch (state){
		case SETTING_OPTION:
			if (BACK )
				return 1;
			
			if (SELECT) switch (selection){
				case 0:
					state = ALARM_ON_OFF;
					break;
				case 1:
					state = HOUR;
					break;
				case 2:
					state = DAY_OF_WEEK;
					break;
			}

			if (NEXT)
				selection = (selection + 2)%3;
			else if (PREVIOUS)
				selection = (selection + 1)%3;
			UARTprintf("sel = %d\n",selection);
			
			break;
			
		/**********************************************/
		case ALARM_ON_OFF:
			if (BACK)
				state = SETTING_OPTION;

			if (SELECT)
				state = (alarm.isOn)? ALARM_TIME_HOUR:SETTING_OPTION;

			if (NEXT || PREVIOUS)
				alarm.isOn = !alarm.isOn;
			
			break;
			
		case ALARM_TIME_HOUR:
			if (BACK)
				state = ALARM_ON_OFF;

			if (SELECT)
				state = ALARM_TIME_MIN;

			if (NEXT)
				alarm.timeSet.hour = (alarm.timeSet.hour + 1)%24;
			else if (PREVIOUS)
				alarm.timeSet.hour = (alarm.timeSet.hour + 23)%24;
			
			break;

		case ALARM_TIME_MIN:
			if (BACK)
				state = ALARM_TIME_HOUR;

			if (SELECT){
				alarm.timeSet.second = alarm.timeSet.milisecond = 0;
				state = SETTING_OPTION;
			}
			
			if (NEXT)
				alarm.timeSet.minute = (alarm.timeSet.minute + 1)%60;
			else if (PREVIOUS)
				alarm.timeSet.minute = (alarm.timeSet.minute + 59)%60;

			
			break;

		/**********************************************/
		case HOUR:
			if (BACK)
				state = SETTING_OPTION;

			if (SELECT)
				state = MINUTE;

			if (NEXT)
				currentTime.hour = (currentTime.hour + 1)%24;
			else if (PREVIOUS)
				currentTime.hour = (currentTime.hour + 23)%24;
			
			break;
		case MINUTE:
			if (BACK)
				state = HOUR;

			if (SELECT){
				currentTime.milisecond  =currentTime.second  =0;
				state = SETTING_OPTION;
			}
			
			if (NEXT)
				currentTime.minute = (currentTime.minute+ 1)%24;
			else if (PREVIOUS)
				currentTime.minute= (currentTime.minute+ 23)%24;
			
			break;
			
		/**********************************************/	
		case DAY_OF_WEEK:
			if (BACK)
				state = SETTING_OPTION;

			if (SELECT)
				state = DATE;
			if (NEXT)
				date.dayOfWeek = (date.dayOfWeek + 1)%7;
			else if (PREVIOUS)
				date.dayOfWeek = (date.dayOfWeek + 6)%7;
			
			break;
			
		case DATE:
			if (BACK)
				state = DAY_OF_WEEK;

			if (SELECT)
				state = MONTH;

			if (NEXT){
				date.date = (date.date + 1)%13;
				if (!date.date) date.date = 1;
			}
			else if (PREVIOUS){
				date.date = (date.date + 12)%13;
				if (!date.date) date.date = 12;
			}
			
			break;
			
		case MONTH:
			if (BACK)
				state = DATE;

			if (SELECT)
				state = YEAR;
			if (NEXT)
				date.month = (date.month + 1)%12;
			else if (PREVIOUS)
				date.month = (date.month + 6)%7;
			
			break;
			
		case YEAR:
			if (BACK)
				state = MONTH;

			if (SELECT)
				state = SETTING_OPTION;

			if (NEXT)
				date.year = (date.year+1);
			else if (PREVIOUS)
				date.year = (date.year-1);
			
			break;
	}

	clearButton();
	return 0;
}


void settingUpdateDisplay(){
	char str[16];
	UARTprintf("state %d \n",state);
	
	switch (state){
		case SETTING_OPTION:
			if (alarm.isOn)
				sprintf(&setting_ListOptionData[0][0],"Alarm %d:%d",
				alarm.timeSet.hour,alarm.timeSet.minute);
			else
				sprintf(&setting_ListOptionData[0][0],"Alarm (OFF)");
				
			sprintf(&setting_ListOptionData[1][0],"Time %d:%d",
				currentTime.hour, currentTime.minute);
			sprintf(&setting_ListOptionData[2][0],"Date %d-%d-%d",
				date.date, date.month, date.year%100);

			ListBoxClear(&setting_ListOption);
			ListBoxTextAdd(&setting_ListOption,setting_ListOptionData[0]);
			ListBoxTextAdd(&setting_ListOption,setting_ListOptionData[1]);
			ListBoxTextAdd(&setting_ListOption,setting_ListOptionData[2]);
			ListBoxSelectionSet(&setting_ListOption, selection);
	
			ListBoxSelectionSet(&setting_ListOption, selection);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_PanelOption);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_ListOption);
			WidgetMessageQueueProcess();
			break;
			
		/**********************************************/
		case ALARM_ON_OFF:
			CanvasTextSet(&setting_Question, "Alarm:");
			
			if (alarm.isOn)
				CanvasTextSet(&setting_Value, "ON");
			else 
				CanvasTextSet(&setting_Value, "OFF");
			
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
			
		case ALARM_TIME_HOUR:
			CanvasTextSet(&setting_Question, "Hour:");
			sprintf(str,"%d",alarm.timeSet.hour);
			CanvasTextSet(&setting_Value, str);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;

		case ALARM_TIME_MIN:
			CanvasTextSet(&setting_Question, "Minute:");
			sprintf(str,"%d",alarm.timeSet.minute);
			CanvasTextSet(&setting_Value, str);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;

		/**********************************************/
		case HOUR:
			CanvasTextSet(&setting_Question, "Hour:");
			sprintf(str,"%d",currentTime.hour);
			CanvasTextSet(&setting_Value, str);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
		case MINUTE:
			CanvasTextSet(&setting_Question, "Minute:");
			sprintf(str,"%d",currentTime.minute);
			CanvasTextSet(&setting_Value, str);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
			
		/**********************************************/	
		case DAY_OF_WEEK:
			CanvasTextSet(&setting_Question, "Day:");
			switch(date.dayOfWeek){
				case MON:
					sprintf(str, "MON");
					break;
				case TUE:
					sprintf(str, "TUE");
					break;
				case WES:
					sprintf(str, "WES");
					break;
				case THU:
					sprintf(str, "THU");
					break;
				case FRI:
					sprintf(str, "FRI");
					break;
				case SAT:
					sprintf(str, "SAT");
					break;
				case SUN:
					sprintf(str, "SUN");
					break;
				default:
					date.dayOfWeek = MON;
					sprintf(str, "MON");
					break;
			}
			CanvasTextSet(&setting_Value, str);
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
			
		case DATE:
			CanvasTextSet(&setting_Question, "Date:");
			sprintf(str,"%d",date.date);
			CanvasTextSet(&setting_Value, str);
			
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
			
		case MONTH:
			CanvasTextSet(&setting_Question, "Month:");
			sprintf(str,"%d",date.month);
			CanvasTextSet(&setting_Value, str);
			
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			
			break;
			
		case YEAR:
			CanvasTextSet(&setting_Question, "Year:");
			sprintf(str,"%d",date.year);
			CanvasTextSet(&setting_Value, str);
			
			WidgetPaint((tWidget *)&setting_Background);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Question);
			WidgetMessageQueueProcess();
			WidgetPaint((tWidget *)&setting_Value);
			WidgetMessageQueueProcess();
			break;
	}
}

/* Function name: setting
 * Description:
 * 		First mode will be run, it gives user to choose what is the 
 * 		next mode. Display: music, setting,... and get button value to
 * 		determine what mode is chosen.
 * Return value:
 * 		SUCCESSFULL
 * 		FAIL
 **/
int setting(void){
	
	/* init string buffer */
	clearButton();

	while (1){
		if (settingStateMachine())
			break;
		settingUpdateDisplay();
		waitButton();
	}

	return SUCCESSFULL;
	
}
