#include "AT91SAM9263.h"

#define LED_RIGHT 1 << 29
#define SLEEP_TIME 1000
#define MS 6250
#define ON 1
#define OFF 0
#define PITS_FLAG 1<<0

unsigned int timeCounter;
unsigned int unknownInterrupts;

void dbgu_print_ascii(const char *a) {}


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
		AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_RIGHT;
		read = OFF;
	}
	else
	{
		AT91C_BASE_SYS -> SYS_PIOC_CODR =  LED_RIGHT;
		read = ON;
	}
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
	            timeCounter = 0;
	            invertLED2();
	        }
	    }
	}
	else
	{
	    unknownInterrupts++;
	}

}

void initPIT_IRQ()
{
	timeCounter = 0;
	unknownInterrupts = 0;
	AT91C_BASE_AIC -> AIC_IDCR = 1<< AT91C_ID_SYS;//turn off the system interrupts
	AT91C_BASE_AIC -> AIC_SVR[AT91C_ID_SYS] = (unsigned int)PITInterruptHandler;
	AT91C_BASE_AIC -> AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE | AT91C_AIC_PRIOR_LOWEST;//level, 0
	AT91C_BASE_AIC -> AIC_ICCR = 1 << AT91C_ID_SYS;//clear the interrupt flag for system int.
	AT91C_BASE_AIC -> AIC_IECR = 1 << AT91C_ID_SYS;//turn on the system interrupts
	AT91C_BASE_PITC -> PITC_PIMR |= AT91C_PITC_PITIEN;
	AT91C_BASE_PITC -> PITC_PIMR |= AT91C_PITC_PITEN;
}



int main()
{
    initializeLEDs();
    initializePIT();
    initPIT_IRQ();

    while(1);
}
 
 

