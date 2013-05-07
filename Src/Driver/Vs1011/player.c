/************************************************************************
 * File name	: player.c
 * Project 		: MUSIC PLAYER
 * Version 		: 1.0
 * Creation Date: 3/5/2013
 * Tabsize		: 4
 * Author  		: Group PHM                  
 * Copyright	: (c) 2013
 * License		: Free.
 * Comments		: 
 * 
 ************************************************************************/
 
#include "player.h"

static FATFS g_sFatFs;
static FIL g_sFileObject;

#define BUFFER_SIZE 1024

u_int8 buffer[BUFFER_SIZE];
int i;
unsigned short usBytesRead;
FRESULT fresult;
unsigned char music_state = 0;

#define FINISH 1
#define ENDOFFILE 2


void VsDreqIntHandle(void){
	GPIOPinIntClear(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ);
	
	
	if (usBytesRead && music_state != FINISH){
		VS1011_WriteSDI_32(buffer + i, 32);
		i+=32;
	}
	
	if (i == usBytesRead && music_state != ENDOFFILE) {	
		fresult = f_read(&g_sFileObject, buffer, BUFFER_SIZE,
						 &usBytesRead);
		
		if(fresult != FR_OK) {
			UARTprintf("read file error\n");
			return;
		}
		if (usBytesRead < BUFFER_SIZE)
			music_state = ENDOFFILE;
		i = 0;
	}
	if (i == usBytesRead && music_state == ENDOFFILE){
		music_state = FINISH;
		IntDisable(VS1011_CS_GPIO_PORT_BASE);
	}
	
	return;
}

void playerInit(void){
	FRESULT fresult;
	fresult = f_mount(0, &g_sFatFs);
    if(fresult != FR_OK)
    {
        UARTprintf("f_mount error\n");
		while(1);
    }
	UARTprintf("f_mount OK\n");
	
	VS1011_WriteSCI(SCI_REG_VOL, 0x4040);
	return;
}


void playerMP3(char * fileName){
	
	fresult = f_open(&g_sFileObject, fileName, FA_READ);
	if(fresult != FR_OK) {
		UARTprintf("Can not open file 1st time\n");
		
        fresult = f_open(&g_sFileObject, fileName, FA_READ);
		if(fresult != FR_OK) {
			UARTprintf("Can not open file\n");
	        return;
	    }
    }
	
	UARTprintf("Open file OK\n");


	GPIOIntTypeSet(VS1011_CS_GPIO_PORT_BASE, VS1011_DREQ, GPIO_HIGH_LEVEL );
	GPIOPinIntClear(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ);
	GPIOPortIntRegister(VS1011_CS_GPIO_PORT_BASE,VsDreqIntHandle);
	GPIOPinIntEnable(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ);
	
	IntMasterEnable();
	usBytesRead = i = 0;
	music_state = 0;

	return;
}

