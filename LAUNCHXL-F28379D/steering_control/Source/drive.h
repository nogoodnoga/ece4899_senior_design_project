#ifndef drive_h
#define drive_h

// remap a number from one range to another
uint32_t map(uint32_t x,
             uint32_t in_min , uint32_t in_max,
             uint32_t out_min, uint32_t out_max);

// writes a value in microseconds (uS) to the servo
uint32_t write_microseconds(uint32_t _us);

// writes a value to the servo, controlling the shaft accordingly (0 to 180 only)
void servo_write(uint32_t pwm_address,
                 EPWM_CounterCompareModule compare_module,
                 uint32_t pos);

#endif
