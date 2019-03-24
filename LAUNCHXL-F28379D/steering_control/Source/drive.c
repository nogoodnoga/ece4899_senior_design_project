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
void servo_write(uint32_t pwm_address,
                 EPWM_CounterCompareModule compare_module,
                 uint32_t pos)
{
   uint32_t value = 0;

   value = map(pos, 0, 180, 600, 2400);
   value = write_microseconds(value);

   EPWM_setCounterCompareValue(pwm_address,
                               compare_module,
                               value);
}
