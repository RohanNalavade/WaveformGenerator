
#include "timers.h"
#include "stateMachineCtrl_public.h"


// Timer structure instance
sTimer_t app_timer;

// Timer for Controlling the DAC Waveform Generation
sTimer_t waveformGen_timer;


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

void TIM3_IRQHandler(void)
{
    /* Check if Update Interrupt Flag (UIF) is set */
    if (TIM3->SR & TIM_SR_UIF)
    {
        waveformGen_timer.timerIsrTick++;

        /* Clear the Interrupt Status */
        TIM3->SR &= ~TIM_SR_UIF;
    }
}

void configure_timer(TIM_TypeDef* timer, uint32_t frequency)
{
    // Select a prescaler and ARR value dynamically
    uint32_t prescaler = 0;
    uint32_t arr_value = 0;
    IRQn_Type irq;
    
    // Enable RCC Clock for the timer
    if (timer == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        while (!(RCC->APB1ENR & RCC_APB1ENR_TIM2EN)); // Ensure clock is enabled

        prescaler = 15999; // Keep high prescaler for low frequencies
    }
    else if (timer == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
        while (!(RCC->APB1ENR & RCC_APB1ENR_TIM3EN)); // Ensure clock is enabled

        prescaler = 15; // Reduce prescaler for high frequencies
    }
    else
    {
        return; // Unsupported timer, exit function
    }

    // Assuming a base clock of APB1 Timer frequency in MHz
    uint32_t base_clock = CLK_FREQ_APB1_MHZ * 1000000;

    // Determine the appropriate IRQ number based on the timer instance
    if (timer == TIM2)
        irq = TIM2_IRQn;
    else if (timer == TIM3)
        irq = TIM3_IRQn;
    else
        return; // Unsupported timer, exit function


    // Compute ARR value
    arr_value = (base_clock / ((prescaler + 1) * frequency)) - 1;

    /* Enable NVIC Interrupt */
    NVIC_EnableIRQ(irq);
    NVIC_SetPriority(irq, 0);

    // Set Prescaler
    timer->PSC = prescaler;

    // Set Auto-Reload Register to achieve the desired frequency
    timer->ARR = arr_value;

    // Send an update event to reset the timer and apply settings.
    timer->EGR |= TIM_EGR_UG;

    // Clear Status Register
    timer->SR &= ~TIM_SR_UIF;

    // Enable the Interrupt
    timer->DIER |= TIM_DIER_UIE;

    timer->CR1 &= ~TIM_CR1_UDIS;

    // Enable the timer
    timer->CR1 |= TIM_CR1_CEN;
}



uint64_t getTimerProcessSystemTick()
{
	return app_timer.timerIsrTick;
}

uint64_t getTimer3Tick()
{
    return waveformGen_timer.timerIsrTick;
}