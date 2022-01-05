#include "Print.h"
#include "UART.h"

void print(const char* str)
{
	for (const char* c = str; *c; ++c)
		uart_tx_u8(*c);
}

void printLn(const char* str)
{
	print(str);
	print("\r\n");
}

void printHex(uint8_t v)
{
	const char hex[] = { "0123456789abcdef" };
	uart_tx_u8(hex[v >> 4]);
	uart_tx_u8(hex[v & 15]);
}
