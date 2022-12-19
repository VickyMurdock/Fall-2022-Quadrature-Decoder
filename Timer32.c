#include "Timer32.h"

#if (TIMER32_MODE == TIMER32_POLLING)
void TimeDelay(uint16_t delay, uint16_t resolution)
{
    if (delay > 0){

        TIMER32_1->LOAD = delay*resolution;                     // delay*resolution = total elapsed time
        TIMER32_1->INTCLR = 0;                                  // any write clears interrupt flag
        TIMER32_1->CONTROL = (TIMER32_CONTROL_ENABLE + TIMER32_CONTROL_SIZE + TIMER32_CONTROL_ONESHOT);
        while ((TIMER32_1->RIS & TIMER32_RIS_RAW_IFG) == 0){};  // count down to zero
        TIMER32_1->CONTROL &= ~(TIMER32_CONTROL_ENABLE);        // disable the timer

    }
}
#else
void TimeDelay(uint16_t delay, uint16_t resolution)
{
    if (delay > 0){

        TIMER32_1->CONTROL = (TIMER32_CONTROL_SIZE | TIMER32_CONTROL_ONESHOT);
        TIMER32_1->LOAD = delay*resolution;                     // delay*resolution = total elapsed time
        TIMER32_1->INTCLR = 0;                                  // any write clears interrupt flag

        NVIC->ISER[0] |= (1 << T32_INT1_IRQn);
        TIMER32_1->CONTROL = (TIMER32_CONTROL_ENABLE | TIMER32_CONTROL_IE);
        WaitForInterrupt();

        // Disable the timer and interrupt
        TIMER32_1->CONTROL &= ~(TIMER32_CONTROL_ENABLE | TIMER32_CONTROL_IE);
    }
}

void T32_INT1_IRQHandler(void)
{
    TIMER32_1->INTCLR |= TIMER32_MIS_IFG;              // Clear Timer32 interrupt flag
}
#endif
