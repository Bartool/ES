#include "AT91SAM9263.h"

#define LED_2_PIN_SET 1 << 29
#define SLEEP_TIME 1000
#define MS 6250
#define ON 1
#define OFF 0
#define PT_REACHED 1<<0
#define PITEN (1<<24) 
#define PITIEN (1<<25)
#define PITS_FLAG 1<<0

void dbgu_print_ascii(const char *a) {}

void clearPITS()
{
	AT91C_BASE_PITC-> PITC_PIVR;//clears a PITS flag
}

void initializeLEDs()
{
	AT91C_BASE_SYS -> SYS_PIOC_PER = LED_2_PIN_SET;
	AT91C_BASE_SYS -> SYS_PIOC_OER= LED_2_PIN_SET;	
	AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_2_PIN_SET;	
}

void initializePIT()
{
	AT91C_BASE_PITC -> PITC_PIMR = MS;
	AT91C_BASE_PITC -> PITC_PIMR &= ~PITEN;
	AT91C_BASE_PITC -> PITC_PIMR &= ~PITIEN;
	clearPITS();
}

void initPIT_IRQ()
{
	AT91C_BASE_AIC -> AIC_IDCR = 31;
	AIC_SVR[AT91C_ID_SYS] = (unsigned int)PITInterruptHandler;
	AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE | AT91C_AIC_PRIOR_LOWEST;
	AT91C_BASE_AIC -> AIC_ICCR = AT91C_ID_SYS;
	
}

void sleep(unsigned int miliseconds)
{	
	unsigned int countms;
	AT91C_BASE_PITC -> PITC_PIMR |= PITEN;
	for(countms = 0; countms < miliseconds; ++countms)
	{
		
		while(1)
		{
			if(AT91C_BASE_PITC -> PITC_PISR & PITS_FLAG)
			{
				break;
			}
		}
		clearPITS();
		
	}
	AT91C_BASE_PITC -> PITC_PIMR &= ~PITEN;
}	

void invertLED2()
{
	unsigned static int read = OFF;
 	if(read == ON)
	{
		AT91C_BASE_SYS -> SYS_PIOC_SODR =  LED_2_PIN_SET;
		read = OFF;
	}
	else
	{
		AT91C_BASE_SYS -> SYS_PIOC_CODR =  LED_2_PIN_SET;
		read = ON;
	}
}

void PITInterruptHandler(void)
{
	

}


int main()
{
    initializeLEDs();
    initializePIT();
    
    while(1)
    {
		invertLED2();
		sleep(SLEEP_TIME);
	
    }
}
 
 

