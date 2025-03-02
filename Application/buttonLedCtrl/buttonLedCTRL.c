#include "buttonLedCTRL.h"
#include "timers_public.h"
#include "gpio.h"
#include "uartLogging_public.h"

void buttonLedCTRLProcessInit()
{
	clearAllTimingProcessParameters(&appButtonLedCtrl.process);

	
	initializeStability(&appButtonLedCtrl.buttonPressed, DEBOUCE_TIME_BUTTON_PRESS_IN_UNITS_100MSEC);
}

void buttonLedCTRLProcess()
{
	updateAllTimingProcessParameters(&appButtonLedCtrl.process);
	
    switch (appButtonLedCtrl.process.currentState)
	{
		case STATE_INITIALIZE:
		{
            /* Do Nothing */
			buttonLedCTRLProcessInit();
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
				
			}

			/* Do Work at 10 Hz*/
			if(runAverageSpeedProcess(&appButtonLedCtrl.process.processTime))
			{
				// Debouncing logic for GPIO
				appButtonLedCtrl.buttonPressed.inputVal = get_gpio_pin_status(GPIOC, BUTTON_PIN);
				appButtonLedCtrl.buttonPressed.stableOutputVal = checkStability(&appButtonLedCtrl.buttonPressed);

				if(appButtonLedCtrl.buttonPressed.stableOutputVal)
				{
					//Button Pressed
					set_gpio(GPIOB, LED_PIN);
					appButtonLedCtrl.currentStatus = true;

					if(appButtonLedCtrl.currentStatus != appButtonLedCtrl.previousStatus)
					{
						//Positive Edge Triggered for the Button
						incrementContinusWaveformFrequency();
					}
				}
				else
				{
					//Button Released
					reset_gpio(GPIOB, LED_PIN);
					appButtonLedCtrl.currentStatus = false;
					
				}
				appButtonLedCtrl.previousStatus = appButtonLedCtrl.currentStatus;
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