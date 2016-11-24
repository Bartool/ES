#include "AT91SAM9263.h"

#define LED_1_PIN_SET 1 << 8
#define LED_2_PIN_SET 1 << 29
#define BUTTON_1_PIN_SET 1<<5
#define BUTTON_2_PIN_SET 1<<4
#define C_TO_E 1 << 4
#define ON 1
#define OFF 0

void dbgu_print_ascii(const char *a) {}

void initializeLEDs()
{
	AT91C_BASE_SYS -> PIO_PER = LED_2_PIN_SET;
	AT91C_BASE_PIOC -> PIO_OER = LED_2_PIN_SET;
	AT91C_BASE_PIOC -> PIO_SODR =  LED_2_PIN_SET;
	AT91B_BASE_PIOC -> PIO_PER = LED_1_PIN_SET;
	AT91B_BASE_PIOC -> PIO_OER = LED_1_PIN_SET;
	AT91B_BASE_PIOC -> PIO_SODR =  LED_1_PIN_SET;
}
void initializeButtons()
{
	AT91B_BASE_PIOC -> PIO_PER = BUTTON_1_PIN_SET;
	AT91B_BASE_PIOC -> PIO_PER = BUTTON_2_PIN_SET;
	AT91B_BASE_PIOC -> PIO_ODR = BUTTON_1_PIN_SET;
	AT91B_BASE_PIOC -> PIO_ODR = BUTTON_2_PIN_SET;
	AT91B_BASE_PIOC -> PIO_PCER = C_TO_E;
	AT91B_BASE_PIOC -> PIO_PUER = BUTTON_1_PIN_SET;
	AT91B_BASE_PIOC -> PIO_PUER = BUTTON_2_PIN_SET;
}

int checkButton()
{
	if(!(AT91B_BASE_PIOC -> PIO_PDSR & BUTTON_1_PIN_SET))
	{
		return 1;
	}
	if(!(AT91B_BASE_PIOC -> PIO_PDSR & BUTTON_2_PIN_SET))
	{
		return 2;
	}
	return 0;
}

void LED1_ON()
{
	*AT91B_BASE_PIOB -> PIO_CODR = LED_1_PIN_SET;
}

void LED1_OFF()
{
	AT91B_BASE_PIOB -> PIO_SODR = LED_1_PIN_SET;
}

int main()
{
    
    initializeButtons();
    initializeLEDs();
    unsigned int buttonState;
    while(1)
    {
		buttonState = checkButton();
		if(buttonState == 1)
			LED1_ON();
		else if(buttonState == 2)
			LED1_OFF();
    }
}
 
 

