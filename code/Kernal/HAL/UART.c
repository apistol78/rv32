#include "Kernal/HAL/UART.h"

static volatile uint32_t* c_base[] =
{
	(volatile uint32_t*)0x50000000,
	(volatile uint32_t*)0x51000000
};

static volatile uint32_t* c_status[] =
{
	(volatile uint32_t*)0x50000004,
	(volatile uint32_t*)0x51000004
};

void uart_tx_u8(uint8_t port, uint8_t data)
{
	*c_base[port] = (uint32_t)data;
}

uint32_t uart_rx_full(uint8_t port)
{
	return (*c_status[port] & 0x00000001) ? 1 : 0;
}

uint32_t uart_rx_empty(uint8_t port)
{
	return (*c_status[port] & 0x00000002) ? 1 : 0;
}

uint8_t uart_rx_u8(uint8_t port)
{
	return (uint8_t)*c_base[port];
}
