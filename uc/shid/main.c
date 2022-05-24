#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

#include "pico/stdlib.h"
#include "hardware/uart.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
void led_blinking_task(void);

#if CFG_TUH_CDC
extern void cdc_task(void);
#endif

extern void hid_app_task(void);

/*------------- MAIN -------------*/
int main(void)
{
	board_init();
	tusb_init();

	uart_init(uart0, 115200);

	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);

	uart_set_hw_flow(uart0, false, false);
	uart_set_format(uart0, 8, 1, UART_PARITY_NONE);

	while (1)
	{
		tuh_task();
		led_blinking_task();

#if CFG_TUH_CDC
		cdc_task();
#endif

		hid_app_task();
	}

	return 0;
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
#if CFG_TUH_CDC

CFG_TUSB_MEM_SECTION static char serial_in_buffer[64] = { 0 };

void tuh_mount_cb(uint8_t dev_addr)
{
	tuh_cdc_receive(dev_addr, serial_in_buffer, sizeof(serial_in_buffer), true); // schedule first transfer
}

void tuh_umount_cb(uint8_t dev_addr)
{
}

// invoked ISR context
void tuh_cdc_xfer_isr(uint8_t dev_addr, xfer_result_t event, cdc_pipeid_t pipe_id, uint32_t xferred_bytes)
{
	//printf(serial_in_buffer);
	tu_memclr(serial_in_buffer, sizeof(serial_in_buffer));
	tuh_cdc_receive(dev_addr, serial_in_buffer, sizeof(serial_in_buffer), true); // waiting for next data
}

void cdc_task(void)
{
}

#endif

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
	const uint32_t interval_ms = 200;
	static uint32_t start_ms = 0;
	static bool led_state = false;

	if (board_millis() - start_ms < interval_ms)
		return;

	start_ms += interval_ms;

	board_led_write(led_state);
	led_state = 1 - led_state;
}
