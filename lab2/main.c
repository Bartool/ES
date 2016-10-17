#define SEGMENT_A 1<<25
#define SEGMENT_B 1<<24
#define SEGMENT_C 1<<22
#define SEGMENT_D 1<<21
#define SEGMENT_E 1<<20
#define SEGMENT_F 1<<27
#define SEGMENT_G 1<<26
#define DP 1<<23
#define LEFT_DIGIT_TRANSISTOR 1<<30
#define RIGHT_DIGIT_TRANSISTOR 1<<28
#define RIGHT 0
#define LEFT 1
#define SLEEP_TIME 1000
#define ITERATIONS 200

volatile unsigned int *ptrPIOB_PER = (volatile unsigned int *) 0xFFFFF400; // enable register
volatile unsigned int *ptrPIOB_OER = (volatile unsigned int *) 0xFFFFF410; // output enable
volatile unsigned int *ptrPIOB_SODR = (volatile unsigned int *) 0xFFFFF430; // set-turns off
volatile unsigned int *ptrPIOB_CODR = (volatile unsigned int *) 0xFFFFF434; // clear-turns on


void dbgu_print_ascii(const char *a) {}

void initializeDisplay()
{
   	*ptrPIOB_PER = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | DP;
   	*ptrPIOB_OER = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | DP;
	*ptrPIOB_CODR = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | DP;
	*ptrPIOB_PER = LEFT_DIGIT_TRANSISTOR;
	*ptrPIOB_OER = LEFT_DIGIT_TRANSISTOR;
	*ptrPIOB_CODR = LEFT_DIGIT_TRANSISTOR;
    *ptrPIOB_PER = RIGHT_DIGIT_TRANSISTOR;
	*ptrPIOB_OER = RIGHT_DIGIT_TRANSISTOR;
	*ptrPIOB_CODR = RIGHT_DIGIT_TRANSISTOR;
}

void sleep(unsigned int delay)
{	
	volatile unsigned int i;

	for(i = 0; i < delay; i++){}
}

void zero()
{
     *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F;
     *ptrPIOB_CODR = SEGMENT_G;
}
void one()
{
	*ptrPIOB_SODR = SEGMENT_B | SEGMENT_C;
	*ptrPIOB_CODR = SEGMENT_A | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;
}
void two()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_F | SEGMENT_C;
}
void three()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_E | SEGMENT_F;
}
void four()
{
    *ptrPIOB_SODR = SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_A | SEGMENT_D | SEGMENT_E;
}
void five()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_B | SEGMENT_E;
}
void six()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_B;
}
void seven()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_C;
    *ptrPIOB_CODR = SEGMENT_D| SEGMENT_E | SEGMENT_F | SEGMENT_G;
}
void eight()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D| SEGMENT_E | SEGMENT_F | SEGMENT_G;
}
void nine()
{
    *ptrPIOB_SODR = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G;
    *ptrPIOB_CODR = SEGMENT_E;
}

void displayNumber(unsigned int number)
{
    switch(number)
    {
    case 1:
        one();
    break;
    case 2:
        two();
    break;
    case 3:
        three();
    break;
    case 4:
        four();
    break;
    case 5:
        five();
    break;
    case 6:
        six();
    break;
    case 7:
        seven();
    break;
    case 8:
        eight();
    break;
    case 9:
        nine();
    break;
    case 0:
        zero();
    break;
    }
}

void setTransistor(int transistor)
{
    if(transistor == LEFT)
    {
        *ptrPIOB_CODR = RIGHT_DIGIT_TRANSISTOR;
        *ptrPIOB_SODR = LEFT_DIGIT_TRANSISTOR;
    }
    else if(transistor == RIGHT)
    {
        *ptrPIOB_SODR = RIGHT_DIGIT_TRANSISTOR;
        *ptrPIOB_CODR = LEFT_DIGIT_TRANSISTOR;
    }

}


int main()
{
    initializeDisplay();
    volatile unsigned int j, i=0;
    volatile unsigned int tens,units;
    while(1)
    {
        tens = i/10;
		units = i%10;
        for(j = 0; j < ITERATIONS; ++j)
        {
			displayNumber(units);
            setTransistor(RIGHT);
            sleep(SLEEP_TIME);
            
            displayNumber(tens);
	    	setTransistor(LEFT);
            sleep(SLEEP_TIME);
        }
        ++i;
		
        if(i == 100)
            i = 0;
    }
}
 
 

