#include "bsp/board.h"
#include "tusb.h"
#include "hardware/uart.h"

#define MAX_REPORT 16

// Each HID instance can has multiple reports
static struct
{
	uint8_t report_count;
	tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

static void process_kbd_report(hid_keyboard_report_t const *report);
static void process_mouse_report(hid_mouse_report_t const * report);
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len);

void hid_app_task(void)
{
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
	uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

	// By default host stack will use activate boot protocol on supported interface.
	// Therefore for this simple example, we only need to parse generic report descriptor (with built-in parser)
	if ( itf_protocol == HID_ITF_PROTOCOL_NONE )
	{
		hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
	}

	// request to receive report
	// tuh_hid_report_received_cb() will be invoked when report is available
	if ( !tuh_hid_receive_report(dev_addr, instance) )
	{
		//printf("Error: cannot request to receive report\r\n");
	}
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
	uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
	switch (itf_protocol)
	{
		case HID_ITF_PROTOCOL_KEYBOARD:
			process_kbd_report( (hid_keyboard_report_t const*) report );
			break;

		case HID_ITF_PROTOCOL_MOUSE:
			process_mouse_report( (hid_mouse_report_t const*) report );
			break;

		default:
			process_generic_report(dev_addr, instance, report, len);
			break;
	}

	// continue to request to receive report
	if ( !tuh_hid_receive_report(dev_addr, instance) )
	{
		//printf("Error: cannot request to receive report\r\n");
	}
}

//--------------------------------------------------------------------+
// Keyboard
//--------------------------------------------------------------------+

static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		if (report->keycode[i] == keycode)
			return true;
	}
	return false;
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
	static hid_keyboard_report_t last = { 0, 0, {0} };

	// Issue events for all newly pressed keys.
	for(uint8_t i = 0; i < 6; i++)
	{
		if (report->keycode[i])
		{
			if (!find_key_in_report(&last, report->keycode[i]))
			{
				uart_putc_raw(uart0, 'K');
				uart_putc_raw(uart0, report->keycode[i]);
				uart_putc_raw(uart0, report->modifier);
				uart_putc_raw(uart0, 'E');
			}
		}
	}

	// Issue events for all released keys.
	for(uint8_t i = 0; i < 6; i++)
	{
		if (last.keycode[i])
		{
			if (!find_key_in_report(report, last.keycode[i]))
			{
				uart_putc_raw(uart0, 'k');
				uart_putc_raw(uart0, last.keycode[i]);
				uart_putc_raw(uart0, last.modifier);
				uart_putc_raw(uart0, 'E');
			}
		}
	}

	last = *report;
}

//--------------------------------------------------------------------+
// Mouse
//--------------------------------------------------------------------+

static void process_mouse_report(hid_mouse_report_t const * report)
{
	uart_putc_raw(uart0, 'M');
	uart_putc_raw(uart0, report->x);
	uart_putc_raw(uart0, report->y);
	uart_putc_raw(uart0, report->wheel);
	uart_putc_raw(uart0, report->buttons);
	uart_putc_raw(uart0, 'E');
}

//--------------------------------------------------------------------+
// Generic Report
//--------------------------------------------------------------------+
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
	uint8_t const rpt_count = hid_info[instance].report_count;
	tuh_hid_report_info_t* rpt_info_arr = hid_info[instance].report_info;
	tuh_hid_report_info_t* rpt_info = NULL;

	if (rpt_count == 1 && rpt_info_arr[0].report_id == 0)
	{
		// Simple report without report ID as 1st byte
		rpt_info = &rpt_info_arr[0];
	}
	else
	{
		// Composite report, 1st byte is report ID, data starts from 2nd byte
		uint8_t const rpt_id = report[0];

		// Find report id in the arrray
		for(uint8_t i=0; i<rpt_count; i++)
		{
			if (rpt_id == rpt_info_arr[i].report_id )
			{
				rpt_info = &rpt_info_arr[i];
				break;
			}
		}

		report++;
		len--;
	}

	if (!rpt_info)
		return;

	// For complete list of Usage Page & Usage checkout src/class/hid/hid.h. For examples:
	// - Keyboard                     : Desktop, Keyboard
	// - Mouse                        : Desktop, Mouse
	// - Gamepad                      : Desktop, Gamepad
	// - Consumer Control (Media Key) : Consumer, Consumer Control
	// - System Control (Power key)   : Desktop, System Control
	// - Generic (vendor)             : 0xFFxx, xx
	if ( rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP )
	{
		switch (rpt_info->usage)
		{
			case HID_USAGE_DESKTOP_KEYBOARD:
				process_kbd_report( (hid_keyboard_report_t const*) report );
				break;

			case HID_USAGE_DESKTOP_MOUSE:
				process_mouse_report( (hid_mouse_report_t const*) report );
				break;

			default:
				break;
		}
	}
}
