/************************************************************************
 * File name	: Vs1011.h
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
#ifndef _VS1011_H_
#define _VS1011_H_

#include "../../musicplayer.h"
#include "player.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "fatfs/src/integer.h"

//define SSI port
#define VS1011_SSI_BASE            		SSI1_BASE
#define VS1011_SSI_SYSCTL_PERIPH   		SYSCTL_PERIPH_SSI1
//GPIO for port SSI
#define VS1011_GPIO_PORT_BASE      		GPIO_PORTE_BASE
#define VS1011_SYSCTL_PERIPH  			SYSCTL_PERIPH_GPIOE
#define VS1011_SSI_CLK           		GPIO_PIN_0
#define VS1011_SSI_TX            		GPIO_PIN_3
#define VS1011_SSI_RX             		GPIO_PIN_2
#define VS1011_SSI_PINS            		(VS1011_SSI_TX | VS1011_SSI_RX | VS1011_SSI_CLK)

// GPIO for chip select VS1011
#define VS1011_CS_GPIO_PORT_BASE      GPIO_PORTD_BASE
#define VS1011_CS_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOD
#define VS1011_xCS                    GPIO_PIN_4
#define VS1011_xRESET                 GPIO_PIN_5
#define VS1011_DREQ					  GPIO_PIN_6
#define VS1011_xDCS                   GPIO_PIN_7
#define VS1011_CONTROL_OUT			(VS1011_xCS | VS1011_xRESET | VS1011_xDCS)

// basic registers list
#define SCI_REG_MODE        0x00    // mode options
#define SCI_REG_STATUS      0x01    // main status reg
#define SCI_REG_BASS        0x02    // enable bass & treble enhancements
#define SCI_REG_CLOCKF      0x03    // clock + freq. multiplier
#define SCI_REG_DECODE_TIME 0x04    // in seconds
#define SCI_REG_AUDATA      0x05    // misc. audio data
#define SCI_REG_WRAM        0x06    // RAM write/read
#define SCI_REG_WRAMADDR    0x07    // base address for RAM write/read
#define SCI_REG_HDAT0       0x08    // stream header data 0
#define SCI_REG_HDAT1       0x09    // stream header data 1
#define SCI_REG_AIADDR      0x0A    // start address of application
#define SCI_REG_VOL         0x0B    // volume control

typedef unsigned char u_int8;
typedef unsigned short u_int16;

void VS1011_Configure_ControlPinss(void);
void VS1011_Configure_SSI(void);
void WaitMilliseconds(int );
void VS1011_WriteSCI(u_int8 ,u_int16 );
unsigned long VS1011_ReadSCI(u_int8 );
void VS1011_Init(void);
void VS1011_DataPut(unsigned long);
unsigned long VS1011_DataGet(void);

int VS1011_WriteSDI(char );
int VS1011_WriteSDI_32(const u_int8 *, u_int8 );

void VS1011_SineTest( unsigned char);

void flushZero(int);

void VS1011_PlaySample(void);

void VS1011_Reset(void);

#endif /* _VS1011_H_ */
