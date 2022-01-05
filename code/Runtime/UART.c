#include "UART.h"

void uart_tx_u8(uint8_t data)
{
	*UART_BASE = (uint32_t)data;
}
