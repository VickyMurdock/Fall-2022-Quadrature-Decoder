#include "uscib.h"

// Function prototypes for local static functions
// In software mode, function to create I2C start condition
static void I2C_Start(void);

// In software mode, function to create I2C stop condition
static void I2C_Stop(void);

// In software mode, function to send a single byte
static void I2C_SendByte(unsigned char SendValue);

static void DataRead(void);
static void DataHigh(void);
static void DataLow(void);
static void ClockHigh(void);
static void ClockLow(void);

unsigned char *LocalSendBuffer;
unsigned char TXByteCtr;

// Uses 12 MHz SMCLK clock
// prescale sets bit rate = 12MHz/prescale
// for example prescale=30 means bit rate =12MHz/30 = 400 kHz
void InitializeUSCIB0(uint32_t Prescale)
{
    // Enable for I2C level shifter.
    ENABLE_SET_AS_OUTPUT;
    ENABLE_HIGH;

#if (I2C_MODE == I2C_HW_MODE)
    // Configure USCIB0 SDA, SCL
    SELECT_SDA;
    SELECT_SCL;

    // initialize eUSCI
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // put eUSCI_B in reset state
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST | // Remain eUSCI_B in reset state
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // I2C master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK

    EUSCI_B0->CTLW1 = (EUSCI_B_CTLW1_CLTO_3 |   // timeout clock low after 165,000 SYSCLK cycles
            EUSCI_B_CTLW1_ASTP_2);              // generate stop condition automatically after TBCNT bytes

    // Set the baud rate for the eUSCI which gets its clock from SMCLK.
    EUSCI_B0->BRW = Prescale;                   // baudrate = SMCLK /Prescale

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    // enable eUSCI
    EUSCI_B0->IE = 0;
#endif
}

#if (I2C_MODE == I2C_HW_MODE)
uint16_t I2C_Send(unsigned char I2CAddress, unsigned char byteCount, unsigned char *SendBuffer)
{
    uint32_t i;
    uint8_t *local_SendBuffer = SendBuffer;
    uint8_t local_byteCount = byteCount;
    uint16_t status;                   // save status register here in case of error

//    while (EUSCI_B0->STATW & EUSCI_B_STATW_BBUSY){};    // wait for I2C ready
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;             // disable eUSCI_B (to modify TBCNT value)
    EUSCI_B0->TBCNT = byteCount;                        // generate stop condition after this many bytes
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;            // enable eUSCI_B
    UCB0I2CSA = I2CAddress;                             // I2CCSA[6:0] is slave address

    // Clear UCTXSTP for no transmit stop condition, and transmit start condition in transmit mode.
    EUSCI_B0->CTLW0 = (EUSCI_B0->CTLW0 & ~EUSCI_B_CTLW0_TXSTP) |
            (EUSCI_B_CTLW0_TXSTT | EUSCI_B_CTLW0_TR);

    // Wait for slave address to be sent (which clears the start transmission bit)
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT){};

    for (i = 0; i < local_byteCount-1; i++) {
        EUSCI_B0->TXBUF = *local_SendBuffer;
        local_SendBuffer++;
        while ((EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) == 0){      // wait for data to be sent

            // if not-acknowledge or arbitration lost, re-initialize
            if (EUSCI_B0->IFG & (EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_ALIFG)){
                status = EUSCI_B0->IFG;              // snapshot flag register for calling program
                InitializeUSCIB0(I2C_PRESCALE);      // reset to known state
                return status;
            }
        }
    }

    // After last byte, wait for the bus to become idle (after stop)
    EUSCI_B0->TXBUF = *local_SendBuffer;
    while (EUSCI_B0->STATW & EUSCI_B_STATW_BBUSY){           // wait for I2C idle

        // if not-acknowledge or arbitration lost, re-initialize
        if (EUSCI_B0->IFG & (EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_ALIFG)){
            status = EUSCI_B0->IFG;              // snapshot flag register for calling program
            InitializeUSCIB0(I2C_PRESCALE);      // reset to known state
            return status;
        }
    }
    return 0;
}
#else
uint16_t I2C_Send(unsigned char I2CAddress, unsigned char byteCount, unsigned char *SendBuffer)
{
    unsigned char *LocalSendBuffer = SendBuffer;

    I2C_Start();
    I2C_SendByte(I2CAddress);
    for (int i = 0; i < byteCount; i++) {
        I2C_SendByte(*LocalSendBuffer);
        LocalSendBuffer++;
    }
    I2C_Stop();

    return 0;
}
#endif

static void DataRead(void )
{
    DATA_READ; // float to get ready to read
}

static void DataHigh(void )
{
    DATA_HIGH; // float pin to go high
    TimeDelay( DATA_HIGH_DELAY, ONE_MICROSECOND_DELAY );
}

static void DataLow(void )
{
    DATA_LOW; // assert low
    TimeDelay( DATA_LOW_DELAY, ONE_MICROSECOND_DELAY );
}

static void ClockHigh(void)
{
    CLK_HIGH;  // float pin to go high
    TimeDelay( CLK_HIGH_DELAY, ONE_MICROSECOND_DELAY );
}

static void ClockLow(void)
{
    CLK_LOW;  // assert low
    TimeDelay( CLK_LOW_DELAY, ONE_MICROSECOND_DELAY );
}

// I2C communication starts when both the data and clock lines go low, in that order.
void I2C_Start(void)
{
    ClockHigh();
    DataHigh();
    DataLow();
    ClockLow();
}

// I2C communication stops with both the clock and data lines going high, in that order.
void I2C_Stop(void)
{
    DataLow();
    ClockLow();
    ClockHigh();
    DataHigh();
}

// Outputs 8-bit command or data via I2C lines.
void I2C_SendByte(unsigned char SendValue)
{
    volatile unsigned char local_SendValue = SendValue;

    for(int n = 0; n < 8; n++ ) {
        if( local_SendValue & 0x80 ) {
            DataHigh();
        } else {
            DataLow();
        }

        ClockHigh();
        ClockLow();

        local_SendValue <<= 1;        // Shift next bit into position.
    }

    DataRead();     // Set data line to receive.
    ClockHigh();    // Clock goes high to wait for acknowledge.

    // Slave will pull data line low to acknowledge.
    while( SDA_PORT->IN & SDA_MASK ) {

        // Else toggle the clock line and check again
        ClockLow();
        ClockHigh();
    }

    ClockLow();
}
