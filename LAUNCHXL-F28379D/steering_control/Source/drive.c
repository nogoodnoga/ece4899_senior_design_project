#include <stdint.h>
#include "driverlib.h"
#include "device.h"
#include "drive.h"

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
   return ((200 * _us) / 64);
}

// writes a value to the servo, controlling the shaft accordingly (0 to 180 only)
void servo_write(uint32_t                  pwm_address,
                 EPWM_CounterCompareModule compare_module,
                 uint32_t                  pos)
{
   uint32_t value = 0;

   value = map(pos, 0, 180, 600, 2400);
   value = write_microseconds(value);

   EPWM_setCounterCompareValue(pwm_address,
                               compare_module,
                               value);
}

// turn vehicle straight
void turn_straight()
{
   servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, STRAIGHT);
   DEVICE_DELAY_US(1000000);
}

// turn vehicle right
void turn_right()
{
   servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, RIGHT);
   DEVICE_DELAY_US(1000000);
}

// turn vehicle left
void turn_left()
{
   servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, LEFT);
   DEVICE_DELAY_US(1000000);
}

// TODO: NEEDS TO BE TESTED
// turn vehicle based on the error between the heading and bearing
// error = heading - bearing
void turn_servo(float error)
{
   // constrain to -180 to 180 degree range
   if (error > 180)
   {
      error -= 360;
   }
   if (error < -180)
   {
      error += 360;
   }

   // The divisor is to reduce the reaction speed and avoid oscillations
   error /= 3;

   servo_write(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, STRAIGHT + error);
}
