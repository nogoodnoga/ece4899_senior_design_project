#include "driverlib.h"
#include "device.h"

void main(void)
{
    /* *************************************************************************************
     * The delay function is supposed to be run from RAM. If you're loading code into flash,
     * somewhere in your code you'll need to copy SysCtl_delay() from flash to RAM.
     * If you're using Device_init(), it will do the memcpy for you as long as you
     * have _FLASH in your predefined symbols.
     **************************************************************************************/

    Device_init();

    Device_initGPIO();
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

    while(1)
    {
        // Turn on LED
        GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);

        // Delay for half a second
        DEVICE_DELAY_US(500000);

        // Turn off LED
        GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);

        // Delay for half a second
        DEVICE_DELAY_US(500000);
    }
}
