#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "stateMachineCtrl_public.h"
#include "stateMachineCtrl.h"
#include "timers_public.h"
#include "timers.h"

// Function to transition state to newState
void transitionState(sProcess *pProcess, eState newState) 
{
    if (pProcess->currentState != newState) 
	{
        pProcess->previousState = pProcess->currentState; // Update previous state
        pProcess->currentState = newState;       		  // Set new state
    }
}

void updateAllTimingProcessParameters(sProcess *pProcess)
{
    if (pProcess == NULL) {
        return;
    }

    uint32_t systemTick = 0;
    uint32_t previousSystemTick = 0;

    pProcess->processTime.previousSystemTick = pProcess->processTime.SystemTick;

    previousSystemTick = pProcess->processTime.previousSystemTick;

    __disable_irq();
    systemTick = getTimerProcessSystemTick();
    __enable_irq();

    pProcess->processTime.SystemTick = systemTick;

    //-----------------------------------------------------------------------------------------------//

    // Clear the flag **only after** the 100Hz event is processed in the main loop
    if (pProcess->processTime.hiSpeedTickFlag) 
    {
        // The flag will be cleared the next time the function is executed
        pProcess->processTime.hiSpeedTickFlag = false;
    }

    //Update 100Hz Tick
    if(((systemTick % 1) == 0) && (systemTick != previousSystemTick))
    {
        pProcess->processTime.hiSpeedTick++;
        pProcess->processTime.hiSpeedTickFlag = true;
    }

    //-----------------------------------------------------------------------------------------------//

    // Clear the flag **only after** the 10Hz event is processed in the main loop
    if (pProcess->processTime.averageSpeedTickFlag) 
    {
        // The flag will be cleared the next time the function is executed
        pProcess->processTime.averageSpeedTickFlag = false;
    }

    //Update 10Hz Tick
    if(((systemTick % 10) == 0) && (systemTick != previousSystemTick))
    {
        pProcess->processTime.averageSpeedTick++;
        pProcess->processTime.averageSpeedTickFlag = true;
    }

    //-----------------------------------------------------------------------------------------------//

    // Clear the flag **only after** the 1Hz event is processed in the main loop
    if (pProcess->processTime.lowSpeedTickFlag) 
    {
        // The flag will be cleared the next time the function is executed
        pProcess->processTime.lowSpeedTickFlag = false;
    }

    //Update 1Hz Tick
    if(((systemTick % 100) == 0) && (systemTick != previousSystemTick))
    {
        pProcess->processTime.lowSpeedTick++;
        pProcess->processTime.lowSpeedTickFlag = true;
    }
}

bool runHiSpeedProcess(sTiming *pProcessTiming)
{
    return pProcessTiming->hiSpeedTickFlag;
}

bool runAverageSpeedProcess(sTiming *pProcessTiming)
{
    return pProcessTiming->averageSpeedTickFlag;
}

bool runLowSpeedProcess(sTiming *pProcessTiming)
{
    return pProcessTiming->lowSpeedTickFlag;
}

void clearAllTimingProcessParameters(sProcess *pProcess)
{
    if (pProcess == NULL) {
        return;
    }

    pProcess->processTime.SystemTick = 0;
    pProcess->processTime.averageSpeedTick = 0;
    pProcess->processTime.averageSpeedTickFlag = 0;
    pProcess->processTime.hiSpeedTick = 0;
    pProcess->processTime.hiSpeedTickFlag = 0;
    pProcess->processTime.lowSpeedTick = 0;
    pProcess->processTime.lowSpeedTickFlag = 0;
    pProcess->processTime.previousSystemTick = 0;
    pProcess->currentState = 0;
    pProcess->previousState = 0;
}
