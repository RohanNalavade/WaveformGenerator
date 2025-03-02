#ifndef __STATEMACHINECTRL_API_H__
#define __STATEMACHINECTRL_API_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// Define the possible states in a generic enum
typedef enum {
    STATE_INITIALIZE = 0,
    STATE_INIT_TO_RUNNING,
    STATE_RUNNING,
    STATE_RUNNNING_TO_SLEEP,
    STATE_SLEEP,
    STATE_STOPPED,
    STATE_MAX_STATE,
} eState;

// Define the Stability structure for checking input variable
typedef struct{
    bool inputVal;              //Input variable for checking its stability
    bool stableOutputVal;       //Output will be HIGH, if inputVal is stable 'HIGH' for time defined by stabilityThreshold.
    uint32_t stabilityCounter;  //Counter
    uint32_t stabilityThreshold;//Counter Theshold unitless
}sStability;

// Define the timing structure
typedef struct {

    /* Contains the systemTick 100 Hz*/
    volatile uint32_t SystemTick;
    
    /* Previous Value of SystemTick*/
    volatile uint32_t previousSystemTick;

    /* Increments at 100 Hz*/
    volatile uint32_t hiSpeedTick;
    
    /* Increments at 10 Hz*/
    volatile uint32_t averageSpeedTick;
    
    /* Increments at 1 Hz */
    volatile uint32_t lowSpeedTick;    
    
    /*  Flag which will be Set at 100 Hz and will be 
        reseted by the respective process when work is done */
    volatile bool hiSpeedTickFlag;
    
    /*  Flag which will be Set at 10 Hz and will be 
        reseted by the respective process when work is done */
    volatile bool averageSpeedTickFlag;

    /*  Flag which will be Set at 1 Hz and will be 
        reseted by the respective process when work is done */
    volatile bool lowSpeedTickFlag;

} sTiming;

// Define the state structure
typedef struct {
    sTiming processTime;  //Timing Parameters of the Process
    eState currentState;  // Current state
    eState previousState; // Previous state (optional, for tracking)
} sProcess;

void transitionState(sProcess *pProcess, eState newState);
void updateAllTimingProcessParameters(sProcess *pProcess);
bool runHiSpeedProcess(sTiming *pProcessTiming);
bool runAverageSpeedProcess(sTiming *pProcessTiming);
bool runLowSpeedProcess(sTiming *pProcessTiming);
void clearAllTimingProcessParameters(sProcess *pProcess);

#endif //__STATEMACHINECTRL_API_H__