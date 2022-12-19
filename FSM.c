# include "FSM.h"
#include "LCD.h"


//unsigned char DashBuffer[20] = { 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0};
//unsigned char StartBuffer[1] = { 0x7C};

FSMState NextStateFunction(RotaryDecoderFSMType *FSM)
{

    FSMState NextState = ResetState;

    switch (FSM->CurrentState){
    case ResetState:
        if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 0)){
            NextState = AInactiveBInactive;
        }
        else if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 1)){
            NextState = AInactiveBActive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 0)){
            NextState = AActiveBInactive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 1)){
            NextState = AActiveBActive;
        }
        else{
            NextState = ResetState;
        }


        break;
    case AInactiveBInactive:
        if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 0)){
            NextState = AInactiveBInactive;
        }
        else if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 1)){
            FSM->count--;
            NextState = AInactiveBActive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 0)){
            FSM->count++;
            NextState = AActiveBInactive;
        }
        else{
            NextState = ResetState;
        }

        break;
    case AInactiveBActive:
        if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 0)){
            FSM->count++;
            NextState = AInactiveBInactive;
        }
        else if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 1)){
            NextState = AInactiveBActive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 1)){
            FSM->count--;
            NextState = AActiveBActive;
        }
        else{
            NextState = ResetState;
        }



        break;
    case AActiveBInactive:
        if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 0)){
            FSM->count--;
            NextState = AInactiveBInactive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 0)){
            NextState = AActiveBInactive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 1)){
            FSM->count++;
            NextState = AActiveBActive;
        }
        else{
            NextState = ResetState;
        }



        break;
    case AActiveBActive:
        if ((FSM->CurrentInputA == 0) && (FSM->CurrentInputB == 1)){
            FSM->count++;
            NextState = AInactiveBActive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 0)){
            FSM->count--;
            NextState = AActiveBInactive;
        }
        else if ((FSM->CurrentInputA == 1) && (FSM->CurrentInputB == 1)){
            NextState = AActiveBActive;
        }
        else{
            NextState = ResetState;
        }



        break;
    }
    return NextState;
}

void InitializeFSM(RotaryDecoderFSMType *FSM)
{

    FSM->CurrentState = ResetState;

}




void OutputFunction(RotaryDecoderFSMType *FSM)
{
    switch (FSM->CurrentState) {
    case ResetState:
        if (FSM->count == 4){
            TOGGLE_RBG_B;
            FSM->count = 0;
            if (FSM->dash < 19){
                FSM->dash++;
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->count == -4){
            TOGGLE_RED_LED;
            FSM->count = 0;
            if (FSM->dash > 1){
                if (FSM->dash > 0){
                    FSM->dash--;
                }
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->ResetButton == 0){
            InitializeLCD();
            FSM->dash = 0;
        }



        break;
    case AInactiveBInactive:
        if (FSM->count == 4){
            TOGGLE_RBG_B;
            FSM->count = 0;
            if (FSM->dash < 19){
                FSM->dash++;
            }
            AddDashes(FSM->dash);
        }
        else if (FSM->count == -4){
            TOGGLE_RED_LED;
            FSM->count = 0;
            if (FSM->dash > 1){
                if (FSM->dash > 0){
                    FSM->dash--;
                }
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->ResetButton == 0){
            InitializeLCD();
            FSM->dash = 0;
        }

        break;
    case AInactiveBActive:
        if (FSM->count == 4){
            TOGGLE_RBG_B;
            FSM->count = 0;
            if (FSM->dash < 19){
                FSM->dash++;
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->count == -4){
            TOGGLE_RED_LED;
            FSM->count = 0;
            if (FSM->dash > 1){
                if (FSM->dash > 0){
                    FSM->dash--;
                }
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->ResetButton == 0){
            InitializeLCD();
            FSM->dash = 0;
        }




        break;
    case AActiveBInactive:
        if (FSM->count == 4){
            TOGGLE_RBG_B;
            FSM->count = 0;
            if (FSM->dash < 19){
                FSM->dash++;
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->count == -4){
            TOGGLE_RED_LED;
            FSM->count = 0;
            if (FSM->dash > 1){
                if (FSM->dash > 0){
                    FSM->dash--;
                }
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->ResetButton == 0){
            InitializeLCD();
            FSM->dash = 0;
        }



        break;
    case AActiveBActive:
        if (FSM->count == 4){
            TOGGLE_RBG_B;
            FSM->count = 0;
            if (FSM->dash < 19){
                FSM->dash++;
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->count == -4){
            TOGGLE_RED_LED;
            FSM->count = 0;
            if (FSM->dash > 1){
                if (FSM->dash > 0){
                    FSM->dash--;
                }
                AddDashes(FSM->dash);
            }

        }
        else if (FSM->ResetButton == 0){
            InitializeLCD();
            FSM->dash = 0;
        }



        break;


    }
}




