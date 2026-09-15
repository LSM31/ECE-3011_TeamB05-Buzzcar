// adding required libraries
#include <stdio.h>
// adding ability to do math
#include <math.h>
// adding ability to use strings
#include <string.h>

// adding ability to use FreeRTOS (mainly for timers for now)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// adding ability to use the driver functions
#include "drv.h"
#include "pins.h"

// function prototype for the pause_two_seconds function
static void pause_two_seconds(void);

// speed < 192 won't start the wheel
int speed = 255;

void app_main(void)
{
    drv_init(); // initializing the driver
    drv_wake(); // waking up the driver
    while (1)
    {
        pause_two_seconds(); // 2 sec delay
        drv_set_motor_a(1, speed); // motor A forward at speed
        pause_two_seconds(); // 2 sec delay
        drv_set_motor_a(0, 0); // motor A stop
        pause_two_seconds(); // 2 sec delay
        drv_set_motor_a(-1, speed); // motor A backward at speed
        pause_two_seconds(); // 2 sec delay
        drv_set_motor_a(0, 0); // motor A stop
        pause_two_seconds(); // 2 sec delay
    }
}

// function to pause for 2 seconds
static void pause_two_seconds(void)
{
    // delaying for the specified number of milliseconds
    vTaskDelay(pdMS_TO_TICKS(2000));
}