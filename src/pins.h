// pragma once ensures that the header file is included only once in a single compilation,
// preventing duplicate definitions and potential conflicts.
#pragma once

#include "driver/gpio.h"

// defining the GPIO pins for the motor driver
#define DRV_A1 GPIO_NUM_16
#define DRV_A2 GPIO_NUM_17
#define DRV_B1 GPIO_NUM_23
#define DRV_B2 GPIO_NUM_22
#define DRV_nSLEEP GPIO_NUM_14