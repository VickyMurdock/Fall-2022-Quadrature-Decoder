#include "LCD.h"
#include "Timer32.h"
#include "FSM.h"


unsigned char ClearBuffer[2] = {0xFE, 0x51};
unsigned char NumberBuffer[20] = {0x30, 0x10, 0x31, 0x10, 0x32, 0x10, 0x33, 0x10, 0x34, 0x10, 0x35, 0x10, 0x36, 0x10, 0x37, 0x10, 0x38, 0x10, 0x39, 0x10};
//unsigned char DashBuffer[20] = { 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0};
unsigned char StartBuffer[1] = { 0x7C};
unsigned char SetDownBuffer[3] = {0xFE, 0x45, 0x28};
unsigned char SetUpBuffer[3] = {0xFE, 0x45, 0x00};
unsigned char DashBuffer[1] = {0xB0};
unsigned char EmptyBuffer[3] = {0x10, 0x10, 0x10};


void InitializeLCD(void)
{

    I2C_Send(LCD_I2C_ADDR, 2, ClearBuffer); // Clears the buffer
    TimeDelay(1000, 100);
    I2C_Send(LCD_I2C_ADDR, 3, SetDownBuffer); // Set down buffer
    TimeDelay(1000, 100);
    I2C_Send(LCD_I2C_ADDR, 20, NumberBuffer); // Number the buffer
    TimeDelay(1000, 100);
    I2C_Send(LCD_I2C_ADDR, 3, SetUpBuffer); // Set up buffer
    TimeDelay(1000, 100);
    I2C_Send(LCD_I2C_ADDR, 1, StartBuffer); // Dash
    TimeDelay(1000, 100);
    I2C_Send(LCD_I2C_ADDR, 3, SetUpBuffer); // Set up buffer






}


void AddDashes(int dash){

        TimeDelay(1000, 100);
        unsigned char point = 0x00 + dash - 1;
        unsigned char PointBuffer[3] = {0xFE, 0x45, point };
        unsigned char PrePointBuffer[3] = {0xFE, 0x45, point + 1};
        I2C_Send(LCD_I2C_ADDR, 3, PrePointBuffer); // Point + 1 buffer
        TimeDelay(1000, 100);
        I2C_Send(LCD_I2C_ADDR, 3, EmptyBuffer); // Empty
        TimeDelay(1000, 100);
        I2C_Send(LCD_I2C_ADDR, 3, PointBuffer); // Point buffer
        TimeDelay(1000, 100);
        I2C_Send(LCD_I2C_ADDR, 1, DashBuffer); // Dash
        TimeDelay(1000, 100);
        I2C_Send(LCD_I2C_ADDR, 1, StartBuffer); // Dash



}





