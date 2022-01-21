#include "Runtime/HAL/UART.h"

void uart_tx_u8(uint8_t data)
{
	*UART_BASE = (uint32_t)data;
}

uint32_t uart_rx_full()
{
	return (*(UART_STATUS) & 0x00000001) ? 1 : 0;
}

uint32_t uart_rx_empty()
{
	return (*(UART_STATUS) & 0x00000002) ? 1 : 0;
}

uint8_t uart_rx_u8()
{
	return (uint8_t)*(UART_BASE);
}
