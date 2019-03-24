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
| 24 MAR 2019   qrogers      2      Steering controls moved to library and
|                                   tested with RC - COMPLETE
|
***************************************************************************/
#include "driverlib.h"
#include "device.h"
#include "drive.h"

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
                               1500);

   // Set up counter mode
   EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
   EPWM_disablePhaseShiftLoad(EPWM1_BASE);
   EPWM_setClockPrescaler(EPWM1_BASE,
                          EPWM_CLOCK_DIVIDER_32,
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
      // PWM test
      for(pos = 30; pos <= 150; pos++)
      {
        servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, pos);
        DEVICE_DELAY_US(15000);
      }

      for(pos = 150; pos > 30; pos--)
      {
        servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, pos);
        DEVICE_DELAY_US(15000);
      }

      // Steering library test
      turn_straight();
      turn_left();
      turn_right();
   }
}
