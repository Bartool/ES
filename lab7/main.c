#include "AT91SAM9263.h"

#define CLOCK 1 << 14
#define DATA 1 << 12
#define CS 1 << 15
#define SLEEP_TIME 1000
#define MS 6250
#define PITS_FLAG 1<<0

int temperature;

void dbgu_print_ascii(const char *a) {}

void clearPITS()
{
	AT91C_BASE_PITC-> PITC_PIVR;//clears a PITS flag
}

void DBGU_init(void)
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
	AT91C_BASE_SYS -> SYS_PIOB_OER= DATA;
	AT91C_BASE_SYS -> SYS_PIOB_ODR= CLOCK;
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
	for(i =0; i < 200; ++i)
	{
		
		*ptrPIT_MR = PITEN;
		while(1)
		{
			if(*ptrPIT_SR & PT_REACHED)
				break;
		}
	}
}

void setClock()
{
	AT91C_BASE_SYS -> SYS_PIOB_SODR = CLOCK;
}

void clearClock()
{
	AT91C_BASE_SYS -> SYS_PIOB_CODR = CLOCK;
}

void readSO()
{
	int bit;
	
	bit = AT91C_BASE_SYS -> SYS_PIOB_PDR= DATA;
		
		

}

int main()
{
    initializeSPI();
    initializePIT();
    initPIT_IRQ();
	int i;
    while(1)
    {
    	for(i = 0; i < 16; ++i)
    	{
    		setClock();
    		readSO();
    		clearClock();
    		sleep(1);
    	}
    	displayString(temperature);
    	sleep(SLEEP_TIME);
    }
}
 
 

