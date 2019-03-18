#include "driverlib.h"
#include "device.h"
#define SLAVE_ADDRESS   0x3C


__interrupt void heartBeat(void);
__interrupt void i2cFIFOISR(void);
__interrupt void i2cAISR(void);

void Heart_Beat_Init(void);
int hb_toggle = 0;

void writeData(int msg);
uint16_t readData();

uint16_t data;
uint16_t sData[2];                  // Send data buffer
uint16_t rData[2];                  // Receive data buffer
uint16_t rDataPoint = 0;            // To keep track of where we are in the
uint16_t send_data = 12;                                    // data stream to check received data

void main(void)
{

    uint16_t data;
    Device_init();

    //-------Initialize-GPIO---------------
    Device_initGPIO();
    GPIO_setPinConfig(GPIO_31_GPIO31);
    GPIO_setPadConfig(31, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(31, GPIO_DIR_MODE_OUT);

    GPIO_setPinConfig(GPIO_105_SCLA);
    GPIO_setPadConfig(105, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(105, GPIO_QUAL_ASYNC);
    //GPIO_setDirectionMode(105, GPIO_DIR_MODE_OUT);

    GPIO_setPinConfig(GPIO_104_SDAA);
    GPIO_setPadConfig(104, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(104, GPIO_QUAL_ASYNC);

    //--------Initialize-Interrupt---------
    Interrupt_initModule();
    Interrupt_initVectorTable();


    //-------------------------------------

    //-------Initialize-HeartBeat----------

    //-------------------------------------
    //--------Initiialize_I2C--------------
    I2C_disableModule(I2CA_BASE);
    I2C_initMaster(I2CA_BASE,DEVICE_SYSCLK_FREQ,400000,I2C_DUTYCYCLE_33);
    I2C_setBitCount(I2CA_BASE, I2C_BITCOUNT_8);
    I2C_setSlaveAddress(I2CA_BASE,0x1E);
    I2C_setEmulationMode(I2CA_BASE, I2C_EMULATION_FREE_RUN);

    I2C_enableInterrupt(I2CA_BASE,I2C_INT_STOP_CONDITION |I2C_INT_REG_ACCESS_RDY);

    I2C_enableFIFO(I2CA_BASE);
    I2C_clearInterruptStatus(I2CA_BASE,I2C_INT_RXFF | I2C_INT_TXFF);
    I2C_enableModule (I2CA_BASE);

    /*I2C_setSlaveAddress(I2CA_BASE,0xAA);
    I2C_setDataCount(I2CA_BASE,2);
    I2C_putData(I2CA_BASE,0xAA);
    I2C_setConfig(I2CA_BASE,I2C_MASTER_SEND_MODE);
    I2C_sendStartCondition(I2CA_BASE);
    I2C_sendStopCondition(I2CA_BASE);*/

    //I2C_setConfig(I2CA_BASE, I2C_MASTER_SEND_MODE);
    //I2C_setSlaveAddress(I2CA_BASE,0xAA);

    //I2C_putData(I2CA_BASE,0x3C);
    //I2C_sendStartCondition(I2CA_BASE);
    //-------------------------------------

    Heart_Beat_Init();
    writeData(0x02);
    writeData(0x00);

    //data = readData();




    while(1){

        //I2C_putData(I2CA_BASE,0xAA);
        //I2C_sendStartCondition(I2CA_BASE);
        //data = I2C_getData(I2CA_BASE);
        //I2C_sendStartCondition(I2CA_BASE);
      }


}


void
writeData(int msg)
{


    I2C_putData(I2CA_BASE,msg);

    I2C_setConfig(I2CA_BASE, I2C_MASTER_SEND_MODE);
    I2C_sendStartCondition(I2CA_BASE);
    I2C_sendStopCondition(I2CA_BASE);

}

uint16_t readData(){
    I2C_setConfig(I2CA_BASE, I2C_MASTER_RECEIVE_MODE);
    return I2C_getData(I2CA_BASE);
}


void heartBeat(){
          hb_toggle = ~hb_toggle;
          // Turn on LED
          GPIO_writePin(31, hb_toggle);
      }

void Heart_Beat_Init(){
        uint32_t temp;
        Interrupt_register(INT_TIMER1, &heartBeat);

        temp = (uint32_t)(DEVICE_SYSCLK_FREQ);
        CPUTimer_setPeriod(CPUTIMER1_BASE, temp/2);
        Interrupt_enable(INT_TIMER1);

        CPUTimer_startTimer (CPUTIMER1_BASE);
        CPUTimer_enableInterrupt(CPUTIMER1_BASE);

        EINT;
        ERTM;
}
