#include "Runtime/HAL/UART.h"

volatile uint32_t* const c_base[] __attribute__ ((section (".rodata"))) =
{
	(volatile uint32_t*)UART_0_BASE,
	(volatile uint32_t*)UART_1_BASE
};

volatile uint32_t* const c_status[] __attribute__ ((section (".rodata"))) =
{
	(volatile uint32_t*)(UART_0_BASE + 0x04),
	(volatile uint32_t*)(UART_1_BASE + 0x04)
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

uint32_t uart_rx_u32(uint8_t port)
{
	uint8_t tmp[4];
	tmp[0] = uart_rx_u8(port);
	tmp[1] = uart_rx_u8(port);
	tmp[2] = uart_rx_u8(port);
	tmp[3] = uart_rx_u8(port);
	return *(uint32_t*)tmp;
}
