#include "AT91SAM9263.h"

#define LED_2 1 << 29
#define SLEEP_TIME 1000
#define MS 6250
#define ON 1
#define OFF 0
#define PT_REACHED 1<<0
#define PITS_FLAG 1<<0
/*
#define BAUDRATE 115200
#define MCK 100000000
#define DIVISOR 16
#define ALL_INTERRUPTS 0xFFFFFFFF
#define SIZE 0x10
#define ENTER 13
#define READ 0
#define WRITE 1
*/
unsigned int timeCounter;
unsigned int unknownInterrupts;
//unsigned int FIFO_MODE
void dbgu_print_ascii(const char *a) {}

/*
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

void changeFIFOMode()
{
    if(FIFO_MODE == READ)
    {
        FIFO_MODE = WRITE;
        AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXDIS;
        AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
    }
    else if(FIFO_MODE == WRITE)
    {
        FIFO_MODE = READ;
        AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXDIS;
        AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;
    }

}

void displayFromBuffer()
{
	char readData;

	if(get(&readData))
	    sendChar(toupper(readData));
	else
	    changeFIFOMode();

}
void newLine()
{
	sendChar('\n');
	sendChar('\r');
}

void read()
{
	char input = readChar();
	sendChar(input);
	if(input == ENTER)
	{
		newLine();
        changeFIFOMode();
		newLine();
	}
	else
		put(input);

}

void bufferInit()
{
	FIFO.head = 0;
	FIFO.tail = FIFO.head;
	FIFO_MODE = READ;
}
*/


void clearPITS()
{
	AT91C_BASE_PITC-> PITC_PIVR;//clears a PITS flag
}

void initializeLEDs()
{
	AT91C_BASE_SYS -> SYS_PIOC_PER = LED_2;
	AT91C_BASE_SYS -> SYS_PIOC_OER= LED_2;
	AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_2;
}

void initializePIT()
{
	AT91C_BASE_PITC -> PITC_PIMR = MS;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITEN;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITIEN;
	clearPITS();
}

void invertLED2()
{
	unsigned static int read = OFF;
 	if(read == ON)
	{
		AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_2;
		read = OFF;
	}
	else
	{
		AT91C_BASE_SYS -> SYS_PIOC_CODR =  LED_2;
		read = ON;
	}
}

void initPIT_IRQ()
{
	AT91C_BASE_AIC -> AIC_IDCR = 31;
	AT91C_BASE_AIC -> AIC_SVR[AT91C_ID_SYS] = (unsigned int)PITInterruptHandler;
	AT91C_BASE_AIC -> AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE | AT91C_AIC_PRIOR_LOWEST;//level, 0
	AT91C_BASE_AIC -> AIC_ICCR = AT91C_ID_SYS;
	AT91C_BASE_AIC -> AIC_IECR = AT91C_ID_SYS;
	AT91C_BASE_PITC -> PITC_PIMR = AT91C_PITC_PITIEN;
	AT91C_BASE_PITC -> PITC_PIMR = AT91C_PITC_PITEN;
    //AT91C_BASE_DBGU -> DBGU_IER = AT91C_US_RXRDY;
    //AT91C_BASE_DBGU -> DBGU_IER = AT91C_US_TXRDY;
	LocalCounter = 0;
	unknownInterrupts = 0;
}

void PITInterruptHandler(void)
{
	if(AT91C_BASE_PITC -> PITC_PIMR & AT91C_PITC_PITIEN)
	{
	    if(AT91C_BASE_PITC -> PITC_PISR & PITS_FLAG)
	    {
	        clearPITS();
	        timeCounter++;
	        if(timeCounter == SLEEP_TIME)
	        {
	            timeCounter == 0;
	            invertLED2();
	        }
	    }
	}
	else if(AT91C_BASE_DBGU->DBGU_CSR & AT91C_BASE_DBGU->DBGU_IMR)
	{
	    if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)
        {
            read();
        }
        else if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)
        {
            displayFromBuffer();
        }
	}
	else
	{
	    unknownInterrupts++;
	}

}

int main()
{
    initializeLEDs();
    //DBGU_init();
    //bufferInit();
    initializePIT();
    initPIT_IRQ();

    while(1);
}
 
 

