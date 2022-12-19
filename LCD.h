#ifndef LCD_H_
#define LCD_H_

#include "msp.h"
#include "uscib.h"

#if (I2C_MODE == I2C_HW_MODE)
#define LCD_I2C_ADDR 0x28 // when hardware I2C
#else
#define LCD_I2C_ADDR (0x28 << 1) // when software I2C = 0x50
#endif

void InitializeLCD(void);

void AddDashes(int dash);

#endif /* LCD_H_ */
