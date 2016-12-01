#include "AT91SAM9263.h"

#define BAUDRATE 115200
#define MCK 100000000
#define DIVISOR 16
#define ALL_INTERRUPTS 0xFFFFFFFF
#define SIZE 0x10
#define ENTER 13
#define READ 0
#define WRITE 1


unsigned int unknownInterrupts;
unsigned int FIFO_MODE;
void dbgu_print_ascii(const char *a) {}


struct Buffer
{
	char data[SIZE];
	unsigned int head, tail;
}FIFO;

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
	//AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;//turn on transmitter
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
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
	AT91C_BASE_DBGU->DBGU_THR = (unsigned int)letter;
}

char readChar()
{
	while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY));
	return AT91C_BASE_DBGU->DBGU_RHR;
}

void displayString(char* string)
{
	unsigned int i;
	for(i=0; string[i]!='\0'; ++i)
	{
		sendChar(string[i]);
	}
}

void newLine()
{
	sendChar('\n');
	sendChar('\r');
}

void displayFromBuffer()
{

	char readData;
	
	if(get(&readData))
	    sendChar(toupper(readData));
	else
	{ 
		newLine();
		AT91C_BASE_DBGU->DBGU_CR=AT91C_US_TXDIS;
	}
}

void read()
{
	char input = readChar();
	int full;
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
	sendChar(input);
	AT91C_BASE_DBGU->DBGU_CR=AT91C_US_TXDIS;
	if(input == ENTER)
	{
		AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
		newLine();
	}
	else
	{
		full = put(input);
		if(full == 0)
		{
			AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
			newLine();
			displayString("Fifo overflow!");
			newLine();
			AT91C_BASE_DBGU->DBGU_CR=AT91C_US_TXDIS;
		}
	}
}

void bufferInit()
{
	FIFO.head = 0;
	FIFO.tail = FIFO.head;
	FIFO_MODE = READ;
}

void DBGUInterruptHandler(void)
{
	int intStatus;
	intStatus = AT91C_BASE_DBGU->DBGU_CSR;
	if(intStatus & AT91C_BASE_DBGU->DBGU_IMR)
	{
	    if (intStatus & AT91C_US_TXRDY)
        {
            displayFromBuffer();
        }
        else if (intStatus & AT91C_US_RXRDY)
        {
            read();
        }
	}
	else
	{
	    unknownInterrupts++;
	}
}

void initDGBU_IRQ()
{
	unknownInterrupts = 0;
	AT91C_BASE_AIC -> AIC_IDCR = 1 << AT91C_ID_SYS;
	AT91C_BASE_AIC -> AIC_SVR[AT91C_ID_SYS] = (unsigned int)DBGUInterruptHandler;
	AT91C_BASE_AIC -> AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | AT91C_AIC_PRIOR_LOWEST;//edge, 0
	AT91C_BASE_AIC -> AIC_ICCR = 1 << AT91C_ID_SYS;
	AT91C_BASE_AIC -> AIC_IECR = 1 << AT91C_ID_SYS;
    AT91C_BASE_DBGU -> DBGU_IER |= AT91C_US_RXRDY;
    AT91C_BASE_DBGU -> DBGU_IER |= AT91C_US_TXRDY;
}

int main()
{
    DBGU_init();
    bufferInit();
    initDGBU_IRQ();
    while(1);
}
 
 

