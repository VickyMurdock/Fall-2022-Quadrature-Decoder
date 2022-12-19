#ifndef USCIB_H_
#define USCIB_H_

#include "msp.h"
#include "Defines.h"
#include "Timer32.h"

#define I2C_HW_MODE 1
#define I2C_SW_MODE 0
#define I2C_MODE I2C_SW_MODE

#define DATA_HIGH_DELAY 5
#define DATA_LOW_DELAY  5
#define CLK_HIGH_DELAY  5
#define CLK_LOW_DELAY   5

#define SDA_BIT     6
#define SDA_MASK    (0x01 << SDA_BIT)
#define SDA_PORT    P1
#define SDA_DIR     SDA_PORT->DIR
#define SDA_OUT     SDA_PORT->OUT
#define SDA_SEL0    SDA_PORT->SEL0
#define SDA_SEL1    SDA_PORT->SEL1
#define SELECT_SDA  SDA_SEL0 |= SDA_MASK; SDA_SEL1 &= ~SDA_MASK
#define DATA_READ   SDA_DIR &= ~SDA_MASK
#define DATA_HIGH   SDA_DIR &= ~SDA_MASK
#define DATA_LOW    SDA_OUT &= ~SDA_MASK; SDA_DIR |= SDA_MASK

#define SCL_BIT     7
#define SCL_MASK    (0x01 << SCL_BIT)
#define SCL_PORT    P1
#define SCL_DIR     SCL_PORT->DIR
#define SCL_OUT     SCL_PORT->OUT
#define SCL_SEL0    SCL_PORT->SEL0
#define SCL_SEL1    SCL_PORT->SEL1
#define SELECT_SCL  SCL_SEL0 |= SCL_MASK; SCL_SEL1 &= ~SCL_MASK
#define CLK_HIGH    SCL_DIR &= ~SCL_MASK
#define CLK_LOW     SCL_OUT &= ~SCL_MASK; SCL_DIR |= SCL_MASK

#define ENABLE_BIT              0
#define ENABLE_MASK             (0x01 << ENABLE_BIT)
#define ENABLE_PORT             P5
#define ENABLE_DIR              ENABLE_PORT->DIR
#define ENABLE_OUT              ENABLE_PORT->OUT
#define ENABLE_SET_AS_OUTPUT    ENABLE_DIR |= ENABLE_MASK
#define ENABLE_HIGH             ENABLE_OUT |= ENABLE_MASK

// Function to initialize eUSCI for I2C in hardware mode.
void InitializeUSCIB0(uint32_t Prescale);

/*
 * This function is used to send information using I2C, whether in hardware mode or software mode.
 *
 * I2CAddress: This byte is the I2C device address.
 * byteCount: The number of bytes to send.
 * SendBuffer: An array containing the bytes to send.
 */
uint16_t I2C_Send(unsigned char I2CAddress, unsigned char byteCount, unsigned char *SendBuffer);

void I2C_SendByte(unsigned char SendValue);

#endif /* USCIB_H_ */
