#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/UART.h"

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
	const static char hex[] = { "0123456789abcdef" };
	uart_tx_u8(hex[v >> 4]);
	uart_tx_u8(hex[v & 15]);
}

void printHexU32(uint32_t v)
{
	const uint8_t* p = (const uint8_t*)&v;
	printHex(p[3]);
	printHex(p[2]);
	printHex(p[1]);
	printHex(p[0]);
}