#include "AT91SAM9263.h"

#define BAUDRATE 115200 
#define MCK 100000000
#define DIVISOR 16
#define ALL_INTERRUPTS 0xFFFFFFFF

void dbgu_print_ascii(const char *a) {}

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
void sendChar(char letter)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
	AT91C_BASE_DBGU->DBGU_THR = (unsigned int)letter;

}

void displayAlphabet()
{
	char letter;
	for(letter='A'; letter <= 'Z'; ++letter)
	{
		sendChar(letter);
	}
}

void displayString(char* string)
{
	unsigned int i;
	for(i=0; string[i]!='\0'; ++i)
	{
		sendChar(string[i]);
	}

}

void main()
{
    	DBGU_init();
	displayAlphabet();
	sendChar('\n');
	sendChar('\r');
	displayString("Hello World!");
   	while(1);

}
 
 

