#ifndef drive_h
#define drive_h

#define RIGHT    30
#define STRAIGHT 90
#define LEFT    150

#define GO   	 1680
#define BACK 	 1150
#define STOP 	 1500

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

// turn vehicle straight
void turn_straight();

// turn vehicle left
void turn_left();

// turn vehicle right
void turn_right();

// TODO: NEEDS TO BE TESTED
// turn vehicle based on the error between the heading and bearing
void turn_servo(float error);

// drive vehicle forward
void drive_forward(uint32_t delay_time);

// stop vehicle
void drive_stop(uint32_t delay_time);

// drive vehicle backward
void drive_backward(uint32_t delay_time);

#endif
