#ifndef FSM_H_
#define FSM_H_

#include "msp.h"


#include "Defines.h"
#include "LED.h"


// Type Definitions
typedef enum {
    ResetState, AInactiveBInactive, AInactiveBActive, AActiveBInactive,
    AActiveBActive
} FSMState;

typedef struct {
    FSMState CurrentState;      // Current state of the FSM
    uint8_t CurrentInputA;      // TP4
    uint8_t CurrentInputB;      // TP3
    uint8_t ResetButton;
    int count;              // counts signals
    int dash;               // counts dashes
} RotaryDecoderFSMType;

// Function Prototypes
void InitializeFSM(RotaryDecoderFSMType *FSM);
FSMState NextStateFunction(RotaryDecoderFSMType *FSM);
void OutputFunction(RotaryDecoderFSMType *FSM);


#endif /* FSM_H_ */

