#include "Kernal/HAL/GPIO.h"

void gpio_set_direction(int pin, int direction)
{
	uint32_t gc = *GPIO_CTRL;
	if (direction == GPIO_INPUT)
		gc &= ~(1 << pin);
	else if (direction == GPIO_OUTPUT)
		gc |= (1 << pin);
	*GPIO_CTRL = gc;		
}

void gpio_set_pin(int pin, int level)
{
	uint32_t gi = *GPIO_IO;
	if (level == GPIO_LOW)
		gi &= ~(1 << pin);
	else if (level == GPIO_HIGH)
		gi |= (1 << pin);
	*GPIO_IO = gi;
}
