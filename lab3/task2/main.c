#define LED_2_PIN_SET 1 << 29
#define SLEEP_TIME 1000
#define MS 1<<19
#define ON 1
#define OFF 0
#define PT_REACHED 1<<0
#define PITEN 1<<24 

/* BUTTON */
	volatile unsigned int *ptrPIOC_ODR = (volatile unsigned int *) 0xFFFFF614; // button - output disable
	volatile unsigned int *ptrPMC_PCER = (volatile unsigned int *) 0xFFFFFC10; //clock enable
	volatile unsigned int *ptrPIOC_PDSR = (volatile unsigned int *) 0xFFFFF63C; // data status register - read 
/* DIODE 2 */
	volatile unsigned int *ptrPIOC_PER = (volatile unsigned int *) 0xFFFFF600; // enable register
	volatile unsigned int *ptrPIOC_OER = (volatile unsigned int *) 0xFFFFF610; // output enable
	volatile unsigned int *ptrPIOC_SODR = (volatile unsigned int *) 0xFFFFF630; // set turns off
	volatile unsigned int *ptrPIOC_CODR = (volatile unsigned int *) 0xFFFFF634; // clear - turns on	
// DELAY	
	volatile unsigned int *ptrPIT_MR = (volatile unsigned int *) 0xFFFFFD30; // mode
	volatile unsigned int *ptrPIT_SR = (volatile unsigned int *) 0xFFFFFD34; //status	


void dbgu_print_ascii(const char *a) {}

void initializeLEDs()
{
	*ptrPIOC_PER = LED_2_PIN_SET;
	*ptrPIOC_OER = LED_2_PIN_SET;	
	*ptrPIOC_SODR =  LED_2_PIN_SET;
	*ptrPIT_MR = MS;
}

void sleep(unsigned int iterations)
{	
	unsigned int i;
	for(i =0; i < 200; ++i)
	{
		
		*ptrPIT_MR = PITEN;
		while(1)
		{
			if(*ptrPIT_SR & PT_REACHED)
				break;
		}
	}
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
    

    initializeLEDs();
    unsigned int timer = 0;
    unsigned int buttonState;
    while(1)
    {
	
	if(timer == 100)
	{
	    invertLED2();
	    timer = 0;
	}
	sleep(SLEEP_TIME);
	timer++;
    }
}
 
 

