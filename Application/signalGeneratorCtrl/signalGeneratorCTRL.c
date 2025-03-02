#include "signalGeneratorCTRL.h"
#include "signalGeneratorCTRL_public.h"
#include "uartLogging_public.h"

void signalGeneratorCTRLProcessInit()
{
	clearAllTimingProcessParameters(&appSignalGenCtrl.process);
	appSignalGenCtrl.currentTimer3OveflowFrequecy = TIM3_FREQUENCY_DEFAULT_HZ;
}

void signalGeneratorCTRLProcess()
{
	updateAllTimingProcessParameters(&appSignalGenCtrl.process);
	

    switch (appSignalGenCtrl.process.currentState)
	{
		case STATE_INITIALIZE:
		{
            /* Do Nothing */
			signalGeneratorCTRLProcessInit();
			transitionState(&appSignalGenCtrl.process, STATE_INIT_TO_RUNNING);
		}break;

		case STATE_INIT_TO_RUNNING:
		{
			/*Do Nothing*/
			transitionState(&appSignalGenCtrl.process, STATE_RUNNING);
		}break;

		case STATE_RUNNING:
		{

			//Do Work as fast as possible

			/* Responsible for Providing Inputs to DAC for the SINE WAVE */
            checkTimer3Update();

            /* Do Work at 100 Hz*/
			if(runHiSpeedProcess(&appSignalGenCtrl.process.processTime))
			{
				//ADC Sampling Conversion Triggered at every 100 HZ
				startAdcConversion();
				uart_printf("%ld: %d, %d\r\n",appSignalGenCtrl.process.processTime.SystemTick ,getADCpinVoltage(0));
			}

			/* Do Work at 10 Hz*/
			if(runAverageSpeedProcess(&appSignalGenCtrl.process.processTime))
			{
				//Do Nothing	
			}

			/* Do Work at 1 Hz*/
			if(runLowSpeedProcess(&appSignalGenCtrl.process.processTime))
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
			transitionState(&appSignalGenCtrl.process, STATE_INITIALIZE);
		}break;
	}
}


/* This Function Executes as TIMER3 Updates i.e at TIM3_FREQUENCY_DEFAULT_HZ frequency*/
void checkTimer3Update(void)
{
    static uint64_t previousTick = 0;
    uint64_t currentTick = getTimer3Tick();  // Get the latest tick

    if (currentTick != previousTick)  // Detect change
    {
        appSignalGenCtrl.extTimingInfo.externalTimerPreviousStatusFlag = appSignalGenCtrl.extTimingInfo.externalTimerCurrentStatusFlag;
        appSignalGenCtrl.extTimingInfo.externalTimerCurrentStatusFlag = true;  // Mark that an update occurred
        appSignalGenCtrl.extTimingInfo.externalTimerTick = currentTick;  // Store the latest tick
        
        // Execute your custom logic when the timer updates
        static uint16_t indexValR = 0;

		/* Update the dacValue as per the SINE WAVE LUT */
		uint16_t dacValue = Wave_LUT[indexValR];
		
		/* Drive the DAC with the constant value dacValue */
		setValueToDacDevice1(dacValue);
		
		// Increment index and wrap around for smooth continuity
		indexValR = (indexValR + 1);
		if(indexValR > 127)	indexValR = 0;

        previousTick = currentTick;  // Update previousTick
    }
    else
    {
        appSignalGenCtrl.extTimingInfo.externalTimerCurrentStatusFlag = false;  // No update
    }
}

/**
 * @brief Update CW Waveform Frquency
 * 
 */
void incrementContinusWaveformFrequency()
{
	appSignalGenCtrl.currentTimer3OveflowFrequecy += TIM3_INCREMENT_STEP_SIZE;
	if(	(appSignalGenCtrl.currentTimer3OveflowFrequecy > TIM3_MAX_FREQUENCY_HZ) ||
		(appSignalGenCtrl.currentTimer3OveflowFrequecy < TIM3_MIN_FREQUENCY_HZ))
	{
		appSignalGenCtrl.currentTimer3OveflowFrequecy = TIM3_MIN_FREQUENCY_HZ;
	}

	__disable_irq();
	(void)configure_timer(WAVE_GENERATOR_TIMER, appSignalGenCtrl.currentTimer3OveflowFrequecy);
	__enable_irq();  // Enable global interrupts

	// uart_printf("Tim Freq: %d \r\n", appSignalGenCtrl.currentTimer3OveflowFrequecy);
}

