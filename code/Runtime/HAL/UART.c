#include "Runtime/HAL/UART.h"

void uart_tx_u8(uint8_t data)
{
	*UART_BASE = (uint32_t)data;
}

uint8_t uart_rx_u8()
{
	return (uint8_t)*(UART_BASE);
}
