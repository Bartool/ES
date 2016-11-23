#include "AT91SAM9263.h"

#define BAUDRATE 115200 
#define MCK 100000000
#define DIVISOR 16
#define ALL_INTERRUPTS 0xFFFFFFFF
#define SIZE 0x10
#define ENTER 13

struct Buffer
{
	char data[SIZE];
	unsigned int head, tail;	
}FIFO;

void dbgu_print_ascii(const char *a) {}
int toupper( int ch );

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
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;//turn on receiver
}

int put(char input)
{
	if((FIFO.head == SIZE -1 && FIFO.tail == 0) || (FIFO.tail-FIFO.head == 1))
		return 0;
	FIFO.data[FIFO.head++] = input;
	if(FIFO.head == SIZE)
		FIFO.head = 0;
	return 1;
}
int get(char* output)
{
	if(FIFO.head == FIFO.tail)
		return 0;
	*output = FIFO.data[FIFO.tail++];
	if(FIFO.tail == SIZE)
		FIFO.tail = 0;
	return 1;
}

void sendChar(char letter)
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY))
	{
				
	}
	AT91C_BASE_DBGU->DBGU_THR = (unsigned int)letter;
}

char readChar()
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY))
	{
				
	}
	return AT91C_BASE_DBGU->DBGU_RHR;

}
void displayFromBuffer()
{
	char* readData;
	while(get(readData))
	{
		sendChar(toupper(*readData));
	}

}
void newLine()
{
	sendChar('\n');
	sendChar('\r');
}

void readAndCapitalize()
{
	char input = readChar();
	sendChar(input);
	if(input == ENTER)
	{
		newLine();
		displayFromBuffer();
		newLine();
	}
	else	
		put(input);
	
}

void bufferInit()
{
	FIFO.head = 0;
	FIFO.tail = FIFO.head;
}


void main()
{
    DBGU_init();
	bufferInit();
	
   	while(1)
	{
		readAndCapitalize();
	}
}
 
 

