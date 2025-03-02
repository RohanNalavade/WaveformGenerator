
#include "timers.h"
#include "stateMachineCtrl_public.h"


// Timer structure instance
sTimer_t app_timer;

void TIM2_IRQHandler(void)
{

	/* if UIF flag is set */
	if(TIM2->SR & TIM_SR_UIF)
	{
		app_timer.timerIsrTick++;
	
		/* Clear the Interrupt Status */
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

void configure_timer(TIM_TypeDef* timer)
{

	
	/* Enable NVIC Interrupt for Timer 3 */
	NVIC_EnableIRQ(TIM2_IRQn);
	
    NVIC_SetPriority(TIM2_IRQn,0);
	
    //16Mhz/(15999+1) = 1kHz = 1 msec Time Period
	timer->PSC = 15999;

	//ARR Register Set //After 10 msec ISR is Called
	timer->ARR = 9;
	
	// Send an update event to reset the timer and apply settings.
	timer->EGR  |= TIM_EGR_UG;
	
    //Clear Status Register
	timer->SR &= ~TIM_SR_UIF;	 

	//Enable the Interrupt
	timer->DIER |= TIM_DIER_UIE;
	
    timer->CR1 &= ~TIM_CR1_UDIS;

	/* Finally enable TIM3 module */
	timer->CR1 |= TIM_CR1_CEN;
    	
}

void usr_delay_mS(uint32_t period_mS) 
{
    // Delay for the requested period
    
    uint64_t last_tick = app_timer.process.processTime.SystemTick;

    while((app_timer.process.processTime.SystemTick - last_tick) < (period_mS/10));

}


void timerProcessInit()
{
	clearAllTimingProcessParameters(&app_timer.process);

	__disable_irq();
	configure_timer(SYSTICK_TIMER);
	__enable_irq();
}

uint64_t getTimerProcessSystemTick()
{
	return app_timer.timerIsrTick;
}


void TIM4_PWM_Init(void)
{
    // Enable clocks for GPIOB and TIM4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  // Enable GPIOB clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   // Enable TIM4 clock

    // Configure PB6 (TIM4_CH1) as alternate function (AF2 for TIM4)
    GPIOB->MODER &= ~GPIO_MODER_MODER6;   // Clear mode bits for PB6
    GPIOB->MODER |= GPIO_MODER_MODER6_1;  // Set PB6 to alternate function mode

    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;    // Clear AFR bits for PB6
    GPIOB->AFR[0] |= (2 << 24);  		  // Set AF2 (TIM4) for PB6

    // Configure TIM4 for PWM mode
    TIM4->PSC = 1599;                     // Set prescaler for 0.1 ms tick
    TIM4->ARR = 199;                      // Set auto-reload for 50 Hz
    TIM4->CCR1 = 15;                      // Set duty cycle (1.5 ms for servo)

    TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;       // Clear output compare mode bits for channel 1
    TIM4->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);  // Set PWM mode 1 (OC1M = 110)
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE;       // Enable preload for CCR1

    TIM4->CCER |= TIM_CCER_CC1E;          // Enable output for channel 1
    TIM4->CR1 |= TIM_CR1_ARPE;            // Enable auto-reload preload

    TIM4->EGR |= TIM_EGR_UG;              // Generate an update event to apply settings
    TIM4->CR1 |= TIM_CR1_CEN;             // Enable TIM4

	TIM4->CCR1 = 15;
}

uint16_t MapAngleToCount(uint16_t angle)
{
    // Ensure the angle is within valid bounds
    if (angle > 180)
        angle = 180;

    // Mapping parameters
    uint16_t CCR_min = 10;  // Corresponds to 1 ms
    uint16_t CCR_max = 20;  // Corresponds to 2 ms

    // Map angle to CCR value
    uint16_t ccr_value = CCR_min + ((CCR_max - CCR_min) * angle) / 180;

    return ccr_value;
}

void SetServoAngle(uint16_t angle)
{
    TIM4->CCR1 = MapAngleToCount(angle); // Set CCR1 for TIM4 Channel 1
}