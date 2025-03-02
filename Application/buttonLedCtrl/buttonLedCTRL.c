#include "buttonLedCTRL.h"
#include "timers_public.h"
#include "gpio.h"
#include "uartLogging_public.h"

void buttonLedCTRLProcessInit()
{
	clearAllTimingProcessParameters(&appButtonLedCtrl.process);
}
int dacValue = 0;
void buttonLedCTRLProcess()
{
	updateAllTimingProcessParameters(&appButtonLedCtrl.process);
	
    switch (appButtonLedCtrl.process.currentState)
	{
		case STATE_INITIALIZE:
		{
            /* Do Nothing */
			// uint8_t buffer[1024] = {0xDE, 0xAD};
			// uart_write((char*)buffer, sizeof(buffer));
			transitionState(&appButtonLedCtrl.process, STATE_INIT_TO_RUNNING);
		}break;

		case STATE_INIT_TO_RUNNING:
		{
			/*Do Nothing*/
			transitionState(&appButtonLedCtrl.process, STATE_RUNNING);
		}break;

		case STATE_RUNNING:
		{
			/* Do Work at 100 Hz*/
			if(runHiSpeedProcess(&appButtonLedCtrl.process.processTime))
			{
				//Do Nothing
				startAdcConversion();
				uart_printf("%ld : %d, %d \r\n", appButtonLedCtrl.process.processTime.SystemTick, getADCpinVoltage(0), getADCpinVoltage(1));
			}

			/* Do Work at 10 Hz*/
			if(runAverageSpeedProcess(&appButtonLedCtrl.process.processTime))
			{
				// Debouncing logic for GPIO input HIGH
				appButtonLedCtrl.buttonPressed.stableOutputVal = get_gpio_pin_status(GPIOC, BUTTON_PIN);

				if(appButtonLedCtrl.buttonPressed.stableOutputVal)
				{
					set_gpio(GPIOB, LED_PIN);
					

				}
				else
				{
					reset_gpio(GPIOB, LED_PIN); 
				}
				setValueToDacDevice1(dacValue++);
				
			}

			/* Do Work at 1 Hz*/
			if(runLowSpeedProcess(&appButtonLedCtrl.process.processTime))
			{
				//Do Nothing
				
			}

		}break;

		case STATE_RUNNNING_TO_SLEEP:
		{
			/* Do Nothing */
		}break;

		case STATE_SLEEP:
		{
			/* Do Nothing */
		}break;

		case STATE_STOPPED:
		{
			/* Do Nothing */
		}break;
		default:
		{
			transitionState(&appButtonLedCtrl.process, STATE_INITIALIZE);
		}break;
	}
}