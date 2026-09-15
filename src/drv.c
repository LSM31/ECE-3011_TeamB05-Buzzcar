/*
    drv.c - Driver functions for controlling the DRV8833 motor driver.

    HOW DOES THIS FILE WORK?

    In this file, we define functions needed to control the DRV8833 motor driver.

    The drv has 5 pins we care about:
        1. DRV_A1: controls the direction of motor A
        2. DRV_A2: controls the direction of motor A
        3. DRV_B1: controls the direction of motor B
        4. DRV_B2: controls the direction of motor B
        5. DRV_nSLEEP: controls whether the drv is asleep or awake

    To provide proper PWM control, we configure an LEDC channel for each of the 4 direction pins.
    The speed of the PWM is controlled by an int from 0 to 255.
        0 is off and 255 is full speed.
    The direction of the motors is controlled by the direction parameter, which can be -1, 0, or 1.
        -1 is backward, 0 is stop, and 1 is forward.

    The enable/sleep functionality is pretty simple, high = on and low = off.
*/

// include the necessary header files
#include "drv.h"
#include "pins.h"

// include the necessary driver libraries
#include "driver/gpio.h"
#include "driver/ledc.h"

// define the LEDC channels for the motor driver, easier to use when naming them later
static const ledc_channel_t DRV_A1_CHANNEL = LEDC_CHANNEL_0;
static const ledc_channel_t DRV_A2_CHANNEL = LEDC_CHANNEL_1;
static const ledc_channel_t DRV_B1_CHANNEL = LEDC_CHANNEL_2;
static const ledc_channel_t DRV_B2_CHANNEL = LEDC_CHANNEL_3;

// a repeatable function to configure the LEDC channel for the motor driver
static void drv_channel_config(ledc_channel_t channel, const gpio_num_t gpio)
{
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = gpio,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
}

// The drv_init function initializes the drv by configuring the LEDC channels and setting the nSLEEP pin low to disable the drv.
void drv_init(void)
{
    // Configure the LEDC timer for the motor driver, fixed frequency of 20kHz and 8-bit resolution
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 20000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure the LEDC channels for the motor driver
    drv_channel_config(DRV_A1_CHANNEL, DRV_A1);
    drv_channel_config(DRV_A2_CHANNEL, DRV_A2);
    drv_channel_config(DRV_B1_CHANNEL, DRV_B1);
    drv_channel_config(DRV_B2_CHANNEL, DRV_B2);
    gpio_set_direction(DRV_nSLEEP, GPIO_MODE_OUTPUT);

    // Set the nSLEEP pin low to disable the driver
    gpio_set_level(DRV_nSLEEP, 0);
}

void drv_sleep(void)
{
    // Set the nSLEEP pin low to put the driver into sleep mode
    gpio_set_level(DRV_nSLEEP, 0);
}

void drv_wake(void)
{
    // Set the nSLEEP pin high to wake up the driver
    gpio_set_level(DRV_nSLEEP, 1);
}

/*
for motor control, the general idea is:
    if direction is forward:
        set A1 to speed
        set A2 to 0
    if direction is backward:
        set A1 to 0
        set A2 to speed
    if direction is stop:
        set A1 to 0
        set A2 to 0

    the same logic applies for motor B, just with B1 and B2 instead of A1 and A2
*/

// this function does the dirty work of setting the duty cycle and updating the LEDC channel for the motor driver
static void ledc_set_and_update(ledc_channel_t channel, int speed)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

// The drv_set_motor_a function sets the direction and speed of motor A.
void drv_set_motor_a(int direction, int speed)
{
    if (direction > 0) {
        ledc_set_and_update(DRV_A1_CHANNEL, speed);
        ledc_set_and_update(DRV_A2_CHANNEL, 0);
    } else if (direction < 0) {
        ledc_set_and_update(DRV_A1_CHANNEL, 0);
        ledc_set_and_update(DRV_A2_CHANNEL, speed);
    } else {
        ledc_set_and_update(DRV_A1_CHANNEL, 0);
        ledc_set_and_update(DRV_A2_CHANNEL, 0);
    }
}

// The drv_set_motor_b function sets the direction and speed of motor B.
void drv_set_motor_b(int direction, int speed)
{
    if (direction > 0) {
        ledc_set_and_update(DRV_B1_CHANNEL, speed);
        ledc_set_and_update(DRV_B2_CHANNEL, 0);
    } else if (direction < 0) {
        ledc_set_and_update(DRV_B1_CHANNEL, 0);
        ledc_set_and_update(DRV_B2_CHANNEL, speed);
    } else {
        ledc_set_and_update(DRV_B1_CHANNEL, 0);
        ledc_set_and_update(DRV_B2_CHANNEL, 0);
    }
}