#ifndef TIMER32_H_
#define TIMER32_H_

#include "msp.h"
#include "Defines.h"
#include "CortexM.h"

#define TIMER32_INTERRUPT 0
#define TIMER32_POLLING 1
#define TIMER32_MODE TIMER32_POLLING

// Function prototype
void TimeDelay(uint16_t delay, uint16_t resolution);

#endif /* TIMER32_H_ */
