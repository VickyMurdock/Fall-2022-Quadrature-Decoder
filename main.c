#include "msp.h"
#include <stdbool.h>
#include "Defines.h"
#include "Clock.h"
#include "CortexM.h"
#include "LED.h"
#include "uscib.h"
#include "LCD.h"
#include "Timer32.h"
#include "FSM.h"
#include "pushbutton.h"

// Function prototypes

// Global variables
RotaryDecoderFSMType Rotary_Decoder_FSM;



void main(void)
{
    bool ClockSystemError = false;


    // Stop the watchdog timer.
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }
    TimeDelay(1000, 100);

    // Initialize hardware.
    InitializeLEDPortPins();
    InitializePushButtonPortPin();
    InitializeUSCIB0(I2C_PRESCALE);
    InitializeLCD();
    InitializeFSM(&Rotary_Decoder_FSM);
    Rotary_Decoder_FSM.count = 0;
    Rotary_Decoder_FSM.dash = 0;

    // Infinite loop
    while (TRUE)
    {



        Rotary_Decoder_FSM.CurrentInputA = READ_PORT_PIN(SIGNAL_A_IN, SIGNAL_A_MASK, SIGNAL_A_BIT); // P5.2
        Rotary_Decoder_FSM.CurrentInputB = READ_PORT_PIN(SIGNAL_B_IN, SIGNAL_B_MASK, SIGNAL_B_BIT); // P3.6
        Rotary_Decoder_FSM.ResetButton = READ_PORT_PIN(PUSHBUTTON_S1_IN, PUSHBUTTON_S1_MASK, PUSHBUTTON_S1_BIT); // Pushbutton reset


        Rotary_Decoder_FSM.CurrentState = NextStateFunction(&Rotary_Decoder_FSM);
        OutputFunction(&Rotary_Decoder_FSM);

    }
}

