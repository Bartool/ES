#include "AT91SAM9263.h"

#define CLOCK 1 << 16
#define DATA 1 << 14
#define CS 1 << 15
#define SLEEP_TIME 1000
#define MS 6250
#define PITS_FLAG 1<<0

unsigned int timeCounter;
unsigned int unknownInterrupts;
unsigned int temperature;

void dbgu_print_ascii(const char *a) {}

void clearPITS()
{
	AT91C_BASE_PITC-> PITC_PIVR;//clears a PITS flag
}

void initializeSPI()
{
	AT91C_BASE_SYS -> SYS_PIOB_PER = DATA;
	AT91C_BASE_SYS -> SYS_PIOB_OER= DATA;
	AT91C_BASE_SYS -> PMC_PCER = CLOCK;
}

void initializePIT()
{
	AT91C_BASE_PITC -> PITC_PIMR = MS;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITEN;
	AT91C_BASE_PITC -> PITC_PIMR &= ~AT91C_PITC_PITIEN;
	clearPITS();
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
    initializeSPI();
    initializePIT();
    initPIT_IRQ();

    while(1);
}
 
 

