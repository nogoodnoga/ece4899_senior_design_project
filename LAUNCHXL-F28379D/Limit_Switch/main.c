#include "driverlib.h"
#include "device.h"

uint32_t XINT1COUNT;

//The ISR for the interrupt. Just increments a counter, turns off the LED for a few milliseconds.
interrupt void xint1_isr(void)
  {
      XINT1COUNT++;

      GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);

      DEVICE_DELAY_US(50000);

      Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
  }

void init_Interrupt(void)
  {
    //Configuring that LED
         GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
         GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

         //By default turn on the LED
         GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);

        //Initialize device clock and peripherals
         Device_init();

         //Disable pin locks and enable internal pullups
         Device_initGPIO();

         //Initialize PIE and clear PIE registers, Disables CPU interrupts
         Interrupt_initModule();

         //Initialize PIE vector table with pointers to shell interrupts
         Interrupt_initVectorTable();

         //Interrupts used are remapped to their ISR functions
         Interrupt_register(INT_XINT1, &xint1_isr);

         //Initialize count to 0
         XINT1COUNT = 0;

         //Enable XINT1 in the PIE
         Interrupt_enable(INT_XINT1);

         //Enable Global Interrupts
         EINT;

         //GPIO 0 in an Input (This is labeled PIN40 on the front of the board FYI)
         GPIO_setDirectionMode(0,GPIO_DIR_MODE_IN);

         //Setting the Interrupt pin to pin 0
         GPIO_setInterruptPin(0,GPIO_INT_XINT1);

         //Setting interrupt to fire on rising edge
         GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_RISING_EDGE);

         //Enable Xint 1
         GPIO_enableInterrupt(GPIO_INT_XINT1);
  }

void main(void)
  {
     init_Interrupt();
     while(1)
     {
         GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
     }
  }




