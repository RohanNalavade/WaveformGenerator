
#include "main.h"

int main()
{
    initializationSystem();

    while(true)
    {
        /* Process that will Change the DAC Frequency At every Button Press*/
        buttonLedCTRLProcess();

        /* Process Which Feeds the DAC with the SINE WAVE LUT and Samples the ADC Data*/
        signalGeneratorCTRLProcess();
    }   
    return 0;
}

