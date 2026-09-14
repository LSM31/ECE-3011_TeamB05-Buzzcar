/*
    drv.h - Header file for the DRV8833 motor driver functions.
    Driver functions for controlling the DRV8833 motor driver.
*/

// the following functions are defined in drv.c and are used to control the motors of the BuzzCar
void drv_init(void);
void drv_sleep(void);
void drv_wake(void);
void drv_set_motor_a(int direction, int speed);
void drv_set_motor_b(int direction, int speed);