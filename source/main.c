/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MilliSec       1
#define Sec           10

#define STATE_INIT 0
#define STATE_LOOP 1
#define STATE_WAIT_DEMO 5
#define STATE_NEXT_LED 6
#define STATE_DEMO 7

#define LED_CURSOR_5050 0
#define LED_CURSOR_3528 1
#define LED_CURSOR_1204 2

#define LED5050_R GPIOB, GPIO_PIN_4
#define LED5050_G GPIOB, GPIO_PIN_5
#define LED5050_B GPIOA, GPIO_PIN_3

#define LED3528_R GPIOC, GPIO_PIN_4
#define LED3528_G GPIOC, GPIO_PIN_3
#define LED3528_B GPIOC, GPIO_PIN_5

#define LED1204_R GPIOC, GPIO_PIN_6
#define LED1204_G GPIOC, GPIO_PIN_7
#define LED1204_B GPIOD, GPIO_PIN_3

#define BUTTON_RED GPIOD, GPIO_PIN_4
#define BUTTON_GREEN GPIOD, GPIO_PIN_5
#define BUTTON_BLUE GPIOD, GPIO_PIN_6
#define BUTTON_NEXT GPIOA, GPIO_PIN_1

#define DEBOUNCE_LIGHT	30
#define DEBOUNCE_SHORT	200
#define DEBOUNCE_LONG		2500
#define INCREMENT 1

#define RAINBOW_DECIMATION 10





/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void ExtraCode_Init(void);
void ExtraCode_StateMachine(void);
void Delay(void action(void), int NumberofTIMCycles);
void Toggle(void);

/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

int NumberOfStart;
int CheckFlag = 1;
int rainbow = 0;

u8 ledCursor ;

u8 led5050blue ;
u8 led5050green ;
u8 led5050red ;

u8 led3528blue ;
u8 led3528green ;
u8 led3528red ;

u8 led1204blue ;
u8 led1204green ;
u8 led1204red ;

u8 sawTooth;
u8 waiting;
u8 currentLED;

u8 led5050on;
u8 led3528on;
u8 led1204on;
u8 swapLedStatus;

u16 debounceRed;
u16 debounceGreen;
u16 debounceBlue;

u8 state;
u16 modeDemo;

u8 rainbowDecimator;

/* Public functions ----------------------------------------------------------*/

/**
  ******************************************************************************
  * @brief Main function.
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void main(void)
{
/* Configures clocks */
CLK_Configuration();

/* Configures GPIOs */
GPIO_Configuration();

led5050on = 1;
led3528on = 1;
led1204on = 1;

led5050blue = 100 ;
led5050green = 0;
led5050red = 0 ;

led3528blue = 0;
led3528green = 152;
led3528red  = 0 ;

led1204blue = 0;
led1204green = 0;
led1204red  = 55 ;

rainbowDecimator = 0;

debounceRed = 0;
debounceGreen = 0;
debounceBlue = 0;

state = STATE_INIT;
modeDemo = 1;

while (1)
{
	
			sawTooth--;
		 
	if (sawTooth == 0) 
	{
		GPIO_WriteHigh(LED5050_B);
		GPIO_WriteHigh(LED5050_B);
		GPIO_WriteHigh(LED5050_G);
		GPIO_WriteHigh(LED5050_R);
		GPIO_WriteHigh(LED3528_B);
		GPIO_WriteHigh(LED3528_G);
		GPIO_WriteHigh(LED3528_R);
		GPIO_WriteHigh(LED1204_B);
		GPIO_WriteHigh(LED1204_G);
		GPIO_WriteHigh(LED1204_R);
				
		switch (state)
		{
			default :
			case STATE_INIT :
			{
				led5050blue 	= 0;
				led5050green 	= 0;
				led5050red 	= 0 ;
			 
				led3528blue 	= 0 ;
				led3528green 	= 0 ;
				led3528red 	= 0;
			 
				led1204blue 	= 0 ;
				led1204green 	= 0 ;
				led1204red 	= 0 ;
			 
				ledCursor = LED_CURSOR_3528;
				
				state = STATE_DEMO;
				
			}
			break;
			
			case STATE_LOOP :
			
					if ( GPIO_ReadInputPin(BUTTON_RED) == 0)
					{
						debounceRed++;
						if (debounceRed > DEBOUNCE_LIGHT )
						{
							debounceRed = 0;
							led3528red = led3528red + INCREMENT;
							led5050red = led5050red + INCREMENT;
							led1204red = led1204red + INCREMENT;
						}                                 
					}
					else
					{
						if (debounceRed > 0) debounceRed-- ;
					}
					
					if ( GPIO_ReadInputPin(BUTTON_GREEN) == 0)
					{
						debounceGreen++;
						if (debounceGreen > DEBOUNCE_LIGHT )
						{
							debounceGreen = 0;
							led3528green = led3528green + INCREMENT ;
							led5050green = led5050green + INCREMENT ;
							led1204green = led1204green + INCREMENT ;
						}                                    
					}
					else
					{
						if (debounceGreen > 0) debounceGreen-- ;
					}
							
					if ( GPIO_ReadInputPin(BUTTON_BLUE) == 0)
					{
						debounceBlue++;
						if (debounceBlue > DEBOUNCE_LIGHT)
						{
							debounceBlue = 0;
							led3528blue = led3528blue + INCREMENT;
							led5050blue = led5050blue + INCREMENT;
							led1204blue = led1204blue + INCREMENT;
						}
					}
					else
					{
						if (debounceBlue > 0) debounceBlue-- ;
					}
					
					
					if ( GPIO_ReadInputPin(BUTTON_NEXT) == 0)
					{
						
						if (modeDemo < DEBOUNCE_LONG )
						{
							modeDemo++;
						}
					}
					else
					{
						if (modeDemo >= DEBOUNCE_LONG ) 
						{
							state = STATE_DEMO;
							modeDemo = 0;
						}
						else
						{
							if ( modeDemo > DEBOUNCE_SHORT ) 
							{
								 swapLedStatus = led1204on ;
								 led1204on = led3528on;
								 led3528on = led5050on;
								 led5050on = swapLedStatus ;
								 modeDemo = 0;
							} 
							else
							{
								if (modeDemo > 0) modeDemo--;
							}
						}
					}
			break;
			
			
			case STATE_DEMO :
			
			
			if ( GPIO_ReadInputPin(BUTTON_NEXT) == 0) 
			{
				modeDemo++;
			}
			else
			{
				if (modeDemo > 0)
				{
					modeDemo--;
				}
				else
				{
					modeDemo = 0;
				}
			}
			
			if (modeDemo > DEBOUNCE_SHORT)
			{
				state = STATE_LOOP;
				modeDemo = 0;
			}
			
			
			if ( GPIO_ReadInputPin(BUTTON_RED) == 0)
			{
				debounceRed++;
				if (debounceRed > DEBOUNCE_SHORT)
				{
					debounceRed = 0;
					led5050on = !led5050on; 					
				}
			}
			else
			{
				if (debounceRed > 0) debounceRed-- ;
			}
					
			 
			if ( GPIO_ReadInputPin(BUTTON_GREEN) == 0)
			{
				debounceGreen++;
				if (debounceGreen > DEBOUNCE_SHORT)
				{
					debounceGreen = 0;
					led3528on = !led3528on; 
				}
			}
			else
			{
				if (debounceGreen > 0) debounceGreen-- ;
			}
					
			if ( GPIO_ReadInputPin(BUTTON_BLUE) == 0)
			{
				debounceBlue++;
				if (debounceBlue > DEBOUNCE_SHORT)
				{
					debounceBlue = 0;
					led1204on = !led1204on; 
				}
			}
			else
			{
				if (debounceBlue > 0) debounceBlue-- ;
			}
			
			if ( rainbowDecimator++ == RAINBOW_DECIMATION)
			{
				rainbowDecimator = 0;
				rainbow++;
					
				if ( rainbow <= 255) 
				{
					led5050blue = 0;
					led5050green = rainbow ;
					led5050red = 255 - rainbow;
				
					led3528blue = 0;
					led3528green = rainbow ;
					led3528red = 255 - rainbow;
				
					led1204blue = 0;
					led1204green = rainbow ;
					led1204red  = 255 - rainbow;
				 
				}
				else
				{
					if ( rainbow <= 511)
					{
						led5050blue = rainbow - 256 ;
						led5050green = 511 - rainbow ;
						led5050red = 0;
					 
						led3528blue = rainbow - 256 ;
						led3528green = 511 - rainbow ;
						led3528red = 0 ;
					 
						led1204blue = rainbow ;
						led1204green = 511 - rainbow ;
						led1204red  = 0;
					}
					else
					{
						led5050blue = 767 - rainbow ;
						led5050green = 0 ;
						led5050red = rainbow - 512 ;
				 
						led3528blue = 767 - rainbow ;
						led3528green = 0; 
						led3528red = rainbow - 512 ;
				 
						led1204blue = 767 - rainbow ;
						led1204green = 0;
						led1204red  = rainbow - 512 ;
					}
					
					if (rainbow >= 766)
					{
						rainbow = 0;
					}
					else
					{
						
					}
				}
		}		
					
		}
	}
	else
	{
		if (sawTooth > 4)
		{
			if ( led5050on > 0)
			{		
				if ( led5050blue == sawTooth ) GPIO_WriteLow(LED5050_B);
				if ( led5050green == sawTooth ) GPIO_WriteLow(LED5050_G);
				if ( led5050red == sawTooth ) GPIO_WriteLow(LED5050_R);
			}
			
			if ( led3528on > 0)
			{
				if ( led3528blue == sawTooth ) GPIO_WriteLow(LED3528_B);
				if ( led3528green == sawTooth ) GPIO_WriteLow(LED3528_G);
				if ( led3528red == sawTooth ) GPIO_WriteLow(LED3528_R);
			}
			
			if (led1204on > 0)
			{
				
				if ( led1204blue == sawTooth ) GPIO_WriteLow(LED1204_B);
				if ( led1204green == sawTooth ) GPIO_WriteLow(LED1204_G);
				if ( led1204red == sawTooth ) GPIO_WriteLow(LED1204_R);
			}
		}
}

		
	}


}






/**
  ******************************************************************************
  * @brief Configures clocks
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void CLK_Configuration(void)
{

  /* Fmaster = 16MHz */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

}

/**
  ******************************************************************************
  * @brief Configures GPIOs
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void GPIO_Configuration(void)
{
  /* GPIOD reset */
  GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);

  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);	// D17 Blue
	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);	// D17 Red
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);	// D17 Green

  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);	// D15 Green
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);	// D15 Red
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);	// D15 Blue
	 
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);	// D16 Red
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);	// D16 Green
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);	// D16 Blue
	
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);	// Button Red
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);	// Button Green
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);	// Button Blue
	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);	// Button Next
	
}
