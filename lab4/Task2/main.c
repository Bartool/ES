#include "AT91SAM9263.h"

#define RXRDY 1<<0
#define TXRDY 1<<1
#define DTXD 1<31
#define BOUNDRATE 115200 
#define MCK 100000000

void dbgu_print_ascii(const char *a) {}
int toupper(int);
void DBGU_init(void)
{
	AT91C_BASE_DBGU->DBGU_IDR = RXRDY | TXRDY;
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RXDIS;
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX | AT91C_US_TXDIS;
	AT91C_BASE_PIOC->PIO_ASR = AT91C_PC31_DTXD | AT91C_PC30_DRXD;
	AT91C_BASE_PIOC->PIO_PDR = AT91C_PC31_DTXD | AT91C_PC30_DRXD;
	AT91C_BASE_DBGU->DBGU_BRGR = MCK/(16*BOUNDRATE);
	AT91C_BASE_DBGU->DBGU_MR= AT91C_US_CHMODE_NORMAL |  AT91C_US_PAR_NONE;
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;
}
void sendChar(char letter)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY))
	{
				
	}
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

char readChar()
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY))
	{
				
	}
	return AT91C_BASE_DBGU->DBGU_RHR;

}

void readAndCapitalize()
{
	char read = readChar();
	
	sendChar(toupper(read);
}

void main()
{
    	DBGU_init();	
	
   	while(1)
	{
		readAndCapitalize();
	}
}
 
 
