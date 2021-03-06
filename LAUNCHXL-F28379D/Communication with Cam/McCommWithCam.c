
//
// Included Files
//
#include "driverlib.h"
#include <stdio.h>
#include "device.h"

//
// Defines
//
#define SLAVE_ADDRESS               0x27
#define EEPROM_HIGH_ADDR            0x00
#define EEPROM_LOW_ADDR             0x30
#define NUM_BYTES                   1
#define MAX_BUFFER_SIZE             14      // Max is currently 14 because of
                                            // 2 address bytes and the 16-byte
                                            // FIFO

//
// I2C message states for I2CMsg struct
//
#define MSG_STATUS_INACTIVE         0x0000 // Message not in use, do not send
#define MSG_STATUS_SEND_WITHSTOP    0x0010 // Send message with stop bit
#define MSG_STATUS_WRITE_BUSY       0x0011 // Message sent, wait for stop
#define MSG_STATUS_SEND_NOSTOP      0x0020 // Send message without stop bit
#define MSG_STATUS_SEND_NOSTOP_BUSY 0x0021 // Message sent, wait for ARDY
#define MSG_STATUS_RESTART          0x0022 // Ready to become master-receiver
#define MSG_STATUS_READ_BUSY        0x0023 // Wait for stop before reading data

//
// Error messages for read and write functions
//
#define ERROR_BUS_BUSY              0x1000
#define ERROR_STOP_NOT_READY        0x5555
#define SUCCESS                     0x0000


//
// Typedefs
//
struct I2CMsg
{
    uint16_t msgStatus;                  // Word stating what state msg is in.
                                         // See MSG_STATUS_* defines above.
    uint16_t slaveAddr;                  // Slave address tied to the message.
    uint16_t numBytes;                   // Number of valid bytes in message.
    uint16_t memoryHighAddr;             // EEPROM address of data associated
                                         // with message (high byte).
    uint16_t memoryLowAddr;              // EEPROM address of data associated
                                         // with message (low byte).
    uint16_t msgBuffer[MAX_BUFFER_SIZE]; // Array holding message data.
};

//
// Globals
//
struct I2CMsg i2cMsgOut = {MSG_STATUS_SEND_WITHSTOP,
                           SLAVE_ADDRESS,
                           NUM_BYTES,
                           EEPROM_HIGH_ADDR,
                           EEPROM_LOW_ADDR,
                           0x01,                // Message bytes
                           0x23,
                           0x45,
                           0x67,
                           0x89,
                           0xAB,
                           0xCD,
                           0xEF};
struct I2CMsg i2cMsgIn  = {MSG_STATUS_SEND_NOSTOP,
                           SLAVE_ADDRESS,
                           NUM_BYTES,
                           EEPROM_HIGH_ADDR,
                           EEPROM_LOW_ADDR};

struct I2CMsg *currentMsgPtr;                   // Used in interrupt

uint16_t passCount = 0;
uint16_t failCount = 0;
unsigned int decimal;
unsigned int unidade;
long relogio = 0;
long relogio2 = 0;  // nao usa
long relogio3 = 0;  // nao usa

//
// Function Prototypes
//
void initI2C(void);
uint16_t I2CA_WriteData(struct I2CMsg *msg);


void LcdWriteLineOne(char *str);
void LcdWriteLineTwo(char *str);
void init_LCD(void);
void I2C_out(uint16_t data);
void LCD_command(unsigned int c);
void LCD_write(unsigned int val);
void convert(int c);
void dec_unid_convert(unsigned numero);
void intro(void);

//__interrupt void i2cAISR(void);

//
// Main
//
void main(void)
{
    //uint16_t error;
    //uint16_t i;
    //int connt = 35;
    uint16_t recieved_buff[22];
    char received_char;
    uint32_t gps_age;
    float    gps_latitude, gps_longitude;
    float    dest_latitude  =  39.7392;
    float dest_longitude = -104.9903;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize GPIOs 32 and 33 for use as SDA A and SCL A respectively
    //
    GPIO_setPinConfig(GPIO_104_SDAA);
    GPIO_setPadConfig(104, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(104, GPIO_QUAL_ASYNC);

    GPIO_setPinConfig(GPIO_105_SCLA);
    GPIO_setPadConfig(105, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(105, GPIO_QUAL_ASYNC);



    // GPIO19 is the SCI RX pin. (Pin 3)
    GPIO_setMasterCore(19, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_19_SCIRXDB);
    GPIO_setDirectionMode(19, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(19, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(19, GPIO_QUAL_ASYNC);

    // GPIO18 is the SCI TX pin. (Pin 4)
    GPIO_setMasterCore(18, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_18_SCITXDB);
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(18, GPIO_QUAL_ASYNC);

    //
    // Initialize PIE and clear PIE registers. Disable CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();



    SCI_performSoftwareReset(SCIB_BASE);

    SCI_setConfig(SCIB_BASE, DEVICE_LSPCLK_FREQ, 9600, (SCI_CONFIG_WLEN_8   |
                                                       SCI_CONFIG_STOP_ONE |
                                                       SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIB_BASE);
    SCI_resetRxFIFO(SCIB_BASE);
    SCI_resetTxFIFO(SCIB_BASE);
    SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIB_BASE);
    SCI_enableModule(SCIB_BASE);
    SCI_performSoftwareReset(SCIB_BASE);

    //
    // Interrupts that are used in this example are re-mapped to ISR functions
    // found within this file.
    //
    //Interrupt_register(INT_I2CA, &i2cAISR);

    //
    // Set I2C use, initializing it for FIFO mode
    //
    initI2C();


    //
    // Set message pointer used in interrupt to point to outgoing message
    //
    currentMsgPtr = &i2cMsgOut;

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(INT_I2CA);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    DINT;
    init_LCD();
    char str [18] = "Hello Team,\0";
    char str2 [18]= "enjoy the break \0";
    int j;
    for(j = 0; j<18;j++){
        str[j] = '\0';
        str2[j] = '\0';
    }
    //char str3 [18] = "MISO       \0";

    //LcdWriteLineOne(&(str[0]));
    //LcdWriteLineTwo(&(str2[0]));

    int i = 0;
    //sprintf(str, "%4d",i);
    while(1)
       {
         SCI_readCharArray(SCIB_BASE, recieved_buff, 19);

         for(i = 0; i < 19; i++){
             if(i<11)
                 str[i] = recieved_buff[i];
             else{
                 int k = i - 11;
                 str2[k] = recieved_buff[i];
             }
         }
          LcdWriteLineOne(&(str[0]));
          LcdWriteLineTwo(&(str2[0]));



       }

}

//
// initI2C - Function to configure I2C A in FIFO mode.
//
void
initI2C()
{
    //
    // Must put I2C into reset before configuring it
    //
    I2C_disableModule(I2CA_BASE);

    //
    // I2C configuration. Use a 400kHz I2CCLK with a 33% duty cycle.
    //
    I2C_initMaster(I2CA_BASE, DEVICE_SYSCLK_FREQ, 100000, I2C_DUTYCYCLE_50);
    I2C_setBitCount(I2CA_BASE, I2C_BITCOUNT_8);
    I2C_setSlaveAddress(I2CA_BASE, SLAVE_ADDRESS);
    I2C_setEmulationMode(I2CA_BASE, I2C_EMULATION_FREE_RUN);

    //
    // Enable stop condition and register-access-ready interrupts
    //
    I2C_enableInterrupt(I2CA_BASE, I2C_INT_STOP_CONDITION |
                        I2C_INT_REG_ACCESS_RDY);

    //
    // FIFO configuration
    //
    I2C_enableFIFO(I2CA_BASE);
    I2C_clearInterruptStatus(I2CA_BASE, I2C_INT_RXFF | I2C_INT_TXFF);

    //
    // Configuration complete. Enable the module.
    //
    I2C_enableModule(I2CA_BASE);
}

//
// writeData - Function to send the data that is to be written to the EEPROM
//
uint16_t
I2CA_WriteData(struct I2CMsg *msg)
{
    uint16_t i;

    //
    // Wait until the STP bit is cleared from any previous master
    // communication. Clearing of this bit by the module is delayed until after
    // the SCD bit is set. If this bit is not checked prior to initiating a new
    // message, the I2C could get confused.
    //
    if(I2C_getStopConditionStatus(I2CA_BASE))
    {
        return(ERROR_STOP_NOT_READY);
    }

    //
    // Setup slave address
    //
    I2C_setSlaveAddress(I2CA_BASE, SLAVE_ADDRESS);

    //
    // Check if bus busy
    //
    if(I2C_isBusBusy(I2CA_BASE))
    {
        return(ERROR_BUS_BUSY);
    }

    //
    // Setup number of bytes to send msgBuffer and address
    //
    I2C_setDataCount(I2CA_BASE, (msg->numBytes + 2));

    //
    // Setup data to send
    //
    I2C_putData(I2CA_BASE, msg->memoryHighAddr);
    I2C_putData(I2CA_BASE, msg->memoryLowAddr);

    for (i = 0; i < msg->numBytes; i++)
    {
        I2C_putData(I2CA_BASE, msg->msgBuffer[i]);
    }

    //
    // Send start as master transmitter
    //
    I2C_setConfig(I2CA_BASE, I2C_MASTER_SEND_MODE);
    I2C_sendStartCondition(I2CA_BASE);
    I2C_sendStopCondition(I2CA_BASE);

    return(SUCCESS);
}
void init_LCD(void) {

     DEVICE_DELAY_US(30000);
     LCD_command(0x02);
     DEVICE_DELAY_US(40);
     LCD_command(0x28);
     DEVICE_DELAY_US(40);
     LCD_command(0x0E);
     DEVICE_DELAY_US(40);
     LCD_command(0x06);
     DEVICE_DELAY_US(1500);
     LCD_command(0x01);

}

void I2C_out(uint16_t data){

     *currentMsgPtr->msgBuffer = data;  // 36  ASCII
     I2CA_WriteData(&i2cMsgOut);
}


void LCD_command(unsigned int c){

    unsigned int commandEo;
    unsigned int commandEi;
    commandEo = 0x08;       //LED = 1, E = 0, RW = 0, RS = 0
    commandEi = 0x0C;       //LED = 1, E = 1, RW = 0, RS = 0

    unsigned int result = 0;

    result = (c&0xf0) | commandEi;
    I2C_out(result);
    result = (c&0xf0) | commandEi;
    I2C_out(result);

    result = (c&0xf0) | commandEo;
    I2C_out(result);
    result = (c&0xf0) | commandEo;
    I2C_out(result);

    result = (c&0xf0) | commandEi;
    I2C_out(result);
    result = (c&0xf0) | commandEi;
    I2C_out(result);


    DEVICE_DELAY_US(4000);

    result = (c<<4) | commandEi;
    I2C_out(result);
    result = (c<<4) | commandEi;
    I2C_out(result);

    result = (c<<4) | commandEo;
    I2C_out(result);
    result = (c<<4) | commandEo;
    I2C_out(result);

    result = (c<<4) | commandEi;
    I2C_out(result);
    result = (c<<4) | commandEi;
    I2C_out(result);


    DEVICE_DELAY_US(4000);




}

void LCD_write(unsigned int c){

    unsigned int commandEo;
    unsigned int commandEi;

     commandEo = 0x09;       //LED = 1, E = 0, RW = 0, RS = 1
     commandEi = 0x0D;       //LED = 1, E = 1, RW = 0, RS = 1

     unsigned int result = 0;

     result = (c&0xf0) | commandEi;
     I2C_out(result);
     result = (c&0xf0) | commandEi;
     I2C_out(result);

     result = (c&0xf0) | commandEo;
     I2C_out(result);
     result = (c&0xf0) | commandEo;
     I2C_out(result);

     result = (c&0xf0) | commandEi;
     I2C_out(result);
     result = (c&0xf0) | commandEi;
     I2C_out(result);

     DEVICE_DELAY_US(4000);

     result = (c<<4) | commandEi;
     I2C_out(result);
     result = (c<<4) | commandEi;
     I2C_out(result);


     result = (c<<4) | commandEo;
     I2C_out(result);
     result = (c<<4) | commandEo;
     I2C_out(result);

     result = (c<<4) | commandEi;
     I2C_out(result);
     result = (c<<4) | commandEi;
     I2C_out(result);

     DEVICE_DELAY_US(4000);

}// end LCD

void convert(int c){
    LCD_write(c+48);
}

void dec_unid_convert(unsigned int numero){
   decimal = numero/10;
   unidade = numero - (decimal*10);
}

void LcdWriteLineOne(char *str){
    LCD_command(0x80);

    char thisChar = *str;
    int i;
    for(i = 0; (thisChar != '\x00' && i < 15); i++){
        LCD_write(thisChar);
        str++;
        thisChar = *str;
    }

}

void LcdWriteLineTwo(char *str){
    LCD_command(0xC0);

    char thisChar = *str;
    int i;
    for(i = 0; (thisChar != '\x00' && i < 17); i++){
        LCD_write(thisChar);
        str++;
        thisChar = *str;
    }
}

void intro(void){


      // WRITE ON THE LCD :  I - CARGA:
      LCD_write('H');
      LCD_write('E');
      LCD_write('L');
      LCD_write('L');
      LCD_write('O');LCD_write(' ');LCD_write('W');LCD_write('O');LCD_write('R');LCD_write('L');LCD_write('D');LCD_write(32);LCD_write(32);LCD_write(32);

      LCD_command(0xC0);    // LINE 2
      // WRITE ON THE : V - SAIDA
      LCD_write(32);
      LCD_write(86);
      LCD_write(45);
      LCD_write(83);
      LCD_write(65);LCD_write(73);LCD_write(68);LCD_write(65);LCD_write(58);LCD_write(32);LCD_write(32);LCD_write(32);LCD_write(32);LCD_write(86);

      LCD_command(0x80);    // LINE 1
}

