/***************************************************************************
| Project           : steering_control
|
| Program name      : main.c
|
| Author            : qrogers
|
| Date created      : 08 MAR 2019
|
| Purpose           : Set up ePWM and to get standard servo to sweep between
|                     0 to 180 degreees
|
| Revision History  :
|
| Date          Author      Ref     Revision
| 08 MAR 2019   qrogers      1      Initial Release
|
***************************************************************************/
#include "driverlib.h"
#include "device.h"

// configure ePWM1
void init_ePWM1()
{
    // Set-up TBCLK
    //(200 MHz / 64) / 50 Hz = 62500 = TBPRD
    EPWM_setTimeBasePeriod(EPWM1_BASE, 62500U);
    EPWM_setPhaseShift(EPWM1_BASE, 0U);
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0U);

    // Set Compare values
    EPWM_setCounterCompareValue(EPWM1_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                850);

    // Set up counter mode
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_disablePhaseShiftLoad(EPWM1_BASE);
    EPWM_setClockPrescaler(EPWM1_BASE,
                           EPWM_CLOCK_DIVIDER_64,
                           EPWM_HSCLOCK_DIVIDER_1);

    // Set up shadowing
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    // Set actions
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
}

// remap a number from one range to another
uint32_t map(uint32_t x,
             uint32_t in_min , uint32_t in_max,
             uint32_t out_min, uint32_t out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// writes a value in microseconds (uS) to the servo
uint32_t write_microseconds(uint32_t _us)
{
   return ((200 * _us) / 128);
}

// writes a value to the servo, controlling the shaft accordingly (0 to 180 only)
void servo_write(uint32_t pos)
{
   uint32_t value = 0;

   value = map(pos, 0, 180, 600, 2400);
   value = write_microseconds(value);

   EPWM_setCounterCompareValue(EPWM1_BASE,
                              EPWM_COUNTER_COMPARE_A,
                              value);
}

void main(void)
{
   Device_init();
   Device_initGPIO();

   GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);
   GPIO_setPinConfig(GPIO_0_EPWM1A);

   init_ePWM1();

   uint16_t pos = 0;

   while(1)
   {
      for(pos = 0; pos <= 180; pos++)
      {
        servo_write(pos);
        DEVICE_DELAY_US(15000);
      }

      for(pos = 180; pos > 0; pos--)
      {
        servo_write(pos);
        DEVICE_DELAY_US(15000);
      }
   }
}
