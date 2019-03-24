/***************************************************************************
| Project           : motor_control
|
| Program name      : main.c
|
| Author            : qrogers
|
| Date created      : 24 MAR 2019
|
| Purpose           : Simple motor control - forward and backward
|
| Revision History  :
|
| Date          Author      Ref     Revision
| 24 MAR 2019   qrogers      1      Initial Release
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

   while(1)
   {
      drive_forward(2000);
      drive_stop(1000);
      drive_backward(2000);
      drive_stop(1000);
   }
}
