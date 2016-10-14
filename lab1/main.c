#define LED_1_PIN_SET 1 << 8
#define LED_2_PIN_SET 1 << 29
#define BUTTON_1_PIN_SET 1<<5
#define BUTTON_2_PIN_SET 1<<4
#define C_TO_E 1 << 4
#define SLEEP_TIME 20000
#define ON 1
#define OFF 0

/* BUTTON */
	volatile unsigned int *ptrPIOC_ODR = (volatile unsigned int *) 0xFFFFF614; // button - output disable
	volatile unsigned int *ptrPMC_PCER = (volatile unsigned int *) 0xFFFFFC10; //clock enable
	volatile unsigned int *ptrPIOC_PDSR = (volatile unsigned int *) 0xFFFFF63C; // data status register - read 
/* DIODE 2 */
	volatile unsigned int *ptrPIOC_PER = (volatile unsigned int *) 0xFFFFF600; // enable register
	volatile unsigned int *ptrPIOC_OER = (volatile unsigned int *) 0xFFFFF610; // output enable
	volatile unsigned int *ptrPIOC_SODR = (volatile unsigned int *) 0xFFFFF630; // set turns off
	volatile unsigned int *ptrPIOC_CODR = (volatile unsigned int *) 0xFFFFF634; // clear - turns on
/* DIODE 1 */
	volatile unsigned int *ptrPIOB_PER = (volatile unsigned int *) 0xFFFFF400; // enable register
	volatile unsigned int *ptrPIOB_OER = (volatile unsigned int *) 0xFFFFF410; // output enable
	volatile unsigned int *ptrPIOB_SODR = (volatile unsigned int *) 0xFFFFF430; // set -  turns off
	volatile unsigned int *ptrPIOB_CODR = (volatile unsigned int *) 0xFFFFF434; // clear - turns on
/*pull up*/
	volatile unsigned int *ptrPIOC_PUER = (volatile unsigned int *) 0xFFFFF664;  //pull up enable 

	

void dbgu_print_ascii(const char *a) {}

void initializeLEDs()
{
	*ptrPIOC_PER = LED_2_PIN_SET;
	*ptrPIOC_OER = LED_2_PIN_SET;
	*ptrPIOB_PER = LED_1_PIN_SET;
	*ptrPIOB_OER = LED_1_PIN_SET;
	
	*ptrPIOC_SODR =  LED_2_PIN_SET;
	*ptrPIOC_SODR =  LED_1_PIN_SET;
}
void initializeButtons()
{
	*ptrPIOC_PER = BUTTON_1_PIN_SET;
	*ptrPIOC_PER = BUTTON_2_PIN_SET;
	*ptrPIOC_ODR = BUTTON_1_PIN_SET;
	*ptrPIOC_ODR = BUTTON_2_PIN_SET;
	*ptrPMC_PCER = C_TO_E;
	*ptrPIOC_PUER = BUTTON_1_PIN_SET;
	*ptrPIOC_PUER = BUTTON_2_PIN_SET;
}

int checkButton()
{
	if(!(*ptrPIOC_PDSR & BUTTON_1_PIN_SET))
	{
	    return 1;
	}
	if(!(*ptrPIOC_PDSR & BUTTON_2_PIN_SET))
	{
            return 2;
	}
        return 0;
}

void sleep(unsigned int iterations)
{	
	volatile unsigned int i;
	
	for(i = 0; i < iterations; i++)
	{
		
	}
}

void LED1_ON()
{
	*ptrPIOB_CODR = LED_1_PIN_SET;
}

void LED1_OFF()
{
	*ptrPIOB_SODR = LED_1_PIN_SET;
}

void invertLED2()
{
	unsigned static int read = OFF;
 	if(read == ON)
	{
		*ptrPIOC_SODR =  LED_2_PIN_SET;
		read = OFF;
	}
	else
	{
		*ptrPIOC_CODR =  LED_2_PIN_SET;
		read = ON;
	}
}

int main()
{
    
    initializeButtons();
    initializeLEDs();
    unsigned int timer = 0;
    unsigned int buttonState;
    while(1)
    {
	buttonState = checkButton();
	if(buttonState == 1)
	    LED1_ON();
	else if(buttonState == 2)
	    LED1_OFF();
	if(timer == 100)
	{
	    invertLED2();
	    timer = 0;
	}
	sleep(SLEEP_TIME);
	timer++;
    }
}
 
 

