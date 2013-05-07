/************************************************************************
 * File name	: Vs1011.c
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
 
#include "Vs1011.h"
#include "sample.c"



void VS1011_Configure_ControlPinss(void){	
	SysCtlPeripheralEnable(VS1011_CS_GPIO_SYSCTL_PERIPH);

	GPIOPinTypeGPIOOutput(VS1011_CS_GPIO_PORT_BASE,VS1011_CONTROL_OUT);

	
	GPIOPadConfigSet(VS1011_CS_GPIO_PORT_BASE, VS1011_CONTROL_OUT,
		 GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOPinTypeGPIOInput(VS1011_CS_GPIO_PORT_BASE, VS1011_DREQ);

	GPIOPadConfigSet(VS1011_CS_GPIO_PORT_BASE, VS1011_DREQ,
                     GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
}


void VS1011_Configure_SSI(void){
	unsigned long ulTemp;

	SSIDisable(VS1011_SSI_BASE);

	GPIOPinTypeSSI(VS1011_GPIO_PORT_BASE, VS1011_SSI_PINS);
	GPIOPadConfigSet(VS1011_GPIO_PORT_BASE, VS1011_SSI_PINS, GPIO_STRENGTH_8MA,
	     GPIO_PIN_TYPE_STD_WPU);
	

	/* Deassert the SSI0 chip select */
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);

	/* Configure the SSI0 port */
	SSIConfigSetExpClk(VS1011_SSI_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
	       SSI_MODE_MASTER, 2000000, 8);
	SSIEnable(VS1011_SSI_BASE);
	while(SSIDataGetNonBlocking(SSI0_BASE, &ulTemp) != 0);
	
}


void WaitMilliseconds(int milliseconds){
	unsigned long tmp = SysCtlClockGet()/1000*milliseconds;
	while(tmp--);
}


void VS1011_WriteSCI(u_int8 addr,u_int16 data){
	while(GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)==0);// waitting DREQ high
	
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, 0);	// activate xCS
	
	VS1011_DataPut(2);
	
	VS1011_DataPut(addr);
	
	VS1011_DataPut((u_int8)(data >> 8));
	
	VS1011_DataPut((u_int8)(data & 0xFF));
	
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);
}

unsigned long VS1011_ReadSCI(u_int8 addr){
	unsigned long res =0;
	unsigned long res1 =0;

	while(GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)==0);// waitting DREQ high

	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, 0);// activate xCS

	VS1011_DataPut(3);
	VS1011_DataPut(addr);
	res =VS1011_DataGet();
	res= (unsigned long)res<<8;
	res1 =VS1011_DataGet();
	res|= (unsigned long)res1;

	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);

	return res;
}


void VS1011_Init(void){
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xRESET, 0);// reset vs1011
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);// xcs high
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); // xdcs high

	// set pin cs other device spi to high
	WaitMilliseconds(20);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xRESET, VS1011_xRESET);

	while((GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)&&VS1011_DREQ)==0);

	VS1011_WriteSCI(SCI_REG_MODE,0x0804);

	VS1011_WriteSCI(SCI_REG_CLOCKF, 0x9800);
	/*VS1011_WriteSCI(SCI_REG_HDAT1, 0x8E12);		//0x8E12 - ID = 2
	VS1011_WriteSCI(SCI_REG_HDAT0, 0x8440);		//0x8440 - 24kHz, 64kbps*/
	
	WaitMilliseconds(500);
}

void VS1011_DataPut(unsigned long data){
	unsigned long dummy;
	while(SSIBusy(VS1011_SSI_BASE));
	SSIDataPut(VS1011_SSI_BASE, data);
	SSIDataGet(VS1011_SSI_BASE, &dummy);
}

unsigned long VS1011_DataGet(void){
	unsigned long data;
	while(SSIBusy(VS1011_SSI_BASE));
	SSIDataPut(VS1011_SSI_BASE, 0xff);
	SSIDataGet(VS1011_SSI_BASE,&data);
	return data;
}


int VS1011_WriteSDI_32(const u_int8 *data, u_int8 bytes){
	
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, 0);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);	
	
	while((GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)&&VS1011_DREQ)==0);
	
	while(bytes--)
		VS1011_DataPut(*data++);

	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); 
	
	return 0;
}

int VS1011_WriteSDI(char data){
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, 0);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);	
	
	while((GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)&&VS1011_DREQ)==0);
	
	VS1011_DataPut(data);
	
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); 
	
	return 0;
}

void VS1011_SineTest( unsigned char n){
	VS1011_Reset();
	WaitMilliseconds(100);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS);
	WaitMilliseconds(100);


	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, 0);
	VS1011_DataPut(0x02);
	VS1011_DataPut(0x00);
	VS1011_DataPut(0x0C);
	VS1011_DataPut(0x20);

	while(SSIBusy(VS1011_SSI_BASE));
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, VS1011_xCS);

	while (GPIOPinRead(VS1011_CS_GPIO_PORT_BASE, VS1011_DREQ) == 0);

	while(1){
	    // Send a Sine Test Header to Data port
	    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, 0); // xdcs low       // enable data bus
	    VS1011_DataPut(0x53);		// special Sine Test Sequence
	    VS1011_DataPut(0xef);		
	    VS1011_DataPut(0x6e);		
	    VS1011_DataPut(n);           // n, Fsin = Fsamp[n>>5] * (n & 0x1f) / 128
	    VS1011_DataPut(0x00);		// where Fsamp[x] = {44100,48000,32000,22050,
	    VS1011_DataPut(0x00);		//                   24000,16000,11025,12000}
	    VS1011_DataPut(0x00);		// for example n = 0x44 -> 1KHz
	    VS1011_DataPut(0x00);		
	    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); // xdcs high		

	    WaitMilliseconds(500);

	    // Stop the sine test
	    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, 0); // xdcs low  		// enable data bus
	    VS1011_DataPut(0x45);		// special Sine Test termination sequence
	    VS1011_DataPut(0x78);		
	    VS1011_DataPut(0x69);		
	    VS1011_DataPut(0x74);		
	    VS1011_DataPut(0x00);		
	    VS1011_DataPut(0x00);		
	    VS1011_DataPut(0x00);		
	    VS1011_DataPut(0x00);		
	    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); // xdcs high		

	    WaitMilliseconds(500);
	}
}

void flushZero(int nZero)
{
    int i;

    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, 0); // xdcs low       // enable data bus
    for( i = 0; i < nZero; i++ )
    {
        while(GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)==0);// waitting DREQ high;
        VS1011_DataPut( 0x00 );
    }
    GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xDCS, VS1011_xDCS); // xdcs high	
}



void VS1011_Reset(void){
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE,VS1011_xRESET,0);
	WaitMilliseconds(100);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE,VS1011_xRESET,VS1011_xRESET);
}

void VS1011_PlaySample(void){
	int i;

	VS1011_Reset();
	while(GPIOPinRead(VS1011_CS_GPIO_PORT_BASE,VS1011_DREQ)==0);
	GPIOPinWrite(VS1011_CS_GPIO_PORT_BASE, VS1011_xCS, 0);
	VS1011_DataPut(0x02);
	VS1011_DataPut(0x00);
	VS1011_DataPut(0x0C);
	VS1011_DataPut(0x20);

	VS1011_WriteSCI(SCI_REG_CLOCKF, 0x0800);
	
	for (i=0; i<sizeof(HelloMP3); i+= 32){
		VS1011_WriteSDI_32((const u_int8*)&HelloMP3[i], (sizeof(HelloMP3) -i <32)? sizeof(HelloMP3)-1 : 32);
	}
	flushZero(2048);
}

