#pragma once

#include "Kernal/HAL/Common.h"

// Directions
#define GPIO_INPUT	0
#define GPIO_OUTPUT	1

// Levels
#define GPIO_LOW	0
#define GPIO_HIGH	1

EXTERN_C void gpio_set_direction(int pin, int direction);

EXTERN_C void gpio_set_pin(int pin, int level);
