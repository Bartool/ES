#include "AT91SAM9263.h"
#include <stdio.h>

#define LED_RIGHT 1 << 29
#define CLOCK 1 << 14
#define DATA 1 << 12
#define CS 1 << 15
#define SLEEP_TIME 1000
#define MS 6250
#define PITS_FLAG 1<<0
#define PITEN (1<<24)
#define PITIEN (1<<25)
#define BAUDRATE 115200
#define MCK 100000000
#define DIVISOR 16
#define ALL_INTERRUPTS 0xFFFFFFFF
#define TEMP_FACTOR 0.0625

short temperature = 0;
float realTemperature = 0;

void dbgu_print_ascii(const char *a) {}

void setClock()
{
	AT91C_BASE_SYS -> SYS_PIOB_SODR = CLOCK;
}

void clearClock()
{
	AT91C_BASE_SYS -> SYS_PIOB_CODR = CLOCK;
}

void clearCS()
{
	AT91C_BASE_SYS -> SYS_PIOB_CODR= CS;
}

void setCS()
{
    AT91C_BASE_SYS -> SYS_PIOB_SODR= CS;
}

void clearPITS()
{
	AT91C_BASE_PITC-> PITC_PIVR;//clears a PITS flag
}

void initializeLEDs()
{
	AT91C_BASE_SYS -> SYS_PIOC_PER = LED_RIGHT;
	AT91C_BASE_SYS -> SYS_PIOC_OER= LED_RIGHT;
	AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_RIGHT;
}

void initializeDBGU(void)
{
	AT91C_BASE_DBGU->DBGU_IDR = ALL_INTERRUPTS;//deactivate all interrupts
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RXDIS;//reset and turn off receiver
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX | AT91C_US_TXDIS;//reset and turn off transmitter
	AT91C_BASE_PIOC->PIO_ASR = AT91C_PC31_DTXD;//enables peripheral control to transmitter
	AT91C_BASE_PIOC->PIO_PDR = AT91C_PC31_DTXD;//assigns I/O line to transmitter
	AT91C_BASE_DBGU->DBGU_BRGR = MCK/(DIVISOR*BAUDRATE);//configure throughout
	AT91C_BASE_DBGU->DBGU_MR= AT91C_US_CHMODE_NORMAL |  AT91C_US_PAR_NONE;//operation mode
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;//turn on transmitter
}

void initializePIT()
{
	AT91C_BASE_PITC -> PITC_PIMR = MS;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITEN;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITIEN;
	clearPITS();
}

void initializeSPI()
{
	AT91C_BASE_SYS -> SYS_PIOB_PER = DATA;
	AT91C_BASE_SYS -> SYS_PIOB_PER = CLOCK;
	AT91C_BASE_SYS -> SYS_PIOB_PER = CS;
	AT91C_BASE_SYS -> SYS_PIOB_ODR = DATA;
	AT91C_BASE_SYS -> SYS_PIOB_OER = CLOCK;
	AT91C_BASE_SYS -> SYS_PIOB_OER = CS;
	AT91C_BASE_SYS -> SYS_PMC_PCER = 1 << 3;
	setCS();
	clearClock();
}

void sendChar(char letter)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
	AT91C_BASE_DBGU->DBGU_THR = (unsigned int)letter;
}

void displayString(char* string)
{
	unsigned int i;
	for(i=0; string[i]!='\0'; ++i)
	{
		sendChar(string[i]);
	}
}

void sleep(unsigned int iterations)
{	
	unsigned int i;
    AT91C_BASE_PITC -> PITC_PIMR |= PITEN;
    for (i=0; i<iterations; i++)
    {
        while(!(AT91C_BASE_PITC -> PITC_PISR & PITS_FLAG)){}
        clearPITS();
    }
    AT91C_BASE_PITC -> PITC_PIMR &= ~PITEN;
}


void readSO(int bitNumber)
{
	if(AT91C_BASE_SYS -> SYS_PIOB_PDSR & DATA)
		temperature |= 1 << bitNumber;
	else
		temperature &= ~(1 << bitNumber);
}

void readTemperature()
{
	volatile int i;
	clearCS();
	for(i = 15; i >= 0; --i)
	{
		clearClock();
		sleep(10);
		readSO(i);
		setClock();
		sleep(10);
	}
	setCS();
	temperature = temperature >> 3;
	realTemperature = temperature * TEMP_FACTOR;
}

void displayTemperature()
{
	char tempChar[8];
	sprintf(tempChar, "%f", realTemperature);
	displayString(tempChar);
	displayString("\n\r");
}

void LED1_ON()
{
	AT91C_BASE_SYS -> SYS_PIOC_CODR =  LED_RIGHT;
}

void LED1_OFF()
{
	AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_RIGHT;
}

void termostate()
{
	if(realTemperature >= 24)
	    LED1_ON();
	else if(realTemperature < 24)
	    LED1_OFF();
}

int main()
{
    initializeLEDs();
    initializeSPI();
    initializePIT();
    initializeDBGU();
    
    while(1)
    {
    	readTemperature();
    	displayTemperature();
    	termostate();
    	sleep(SLEEP_TIME);
    }
}
 
 

