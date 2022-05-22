#include <stdio.h>
#include "Runtime/HAL/I2C.h"
#include "Runtime/HAL/SIL9024A.h"
#include "Runtime/HAL/Timer.h"

#define TPI_ADDRESS 0x39

// registers
#define TPI_DEVICE_POWER_STATE_CTRL_REG	0x1e
#define TPI_DEVICE_ID					0x1b
#define TPI_INTERRUPT_ENABLE_REG		0x3c
#define TPI_INTERRUPT_STATUS_REG		0x3d
#define TPI_ENABLE						0xc7
#define TPI_INPUT_BUS_AND_PR_REG		0x08

// register values
#define TX_POWER_STATE_MASK				0x03
#define TX_POWER_STATE_D0				0x00
#define TX_POWER_STATE_D1				0x01
#define TX_POWER_STATE_D2				0x02
#define TX_POWER_STATE_D3				0x03
#define SiI9022_DEVICE_ID				0xb0
#define HOT_PLUG_EVENT					0x01


static void modify_register_bits(uint8_t reg, uint8_t mask, uint8_t set)
{
	uint8_t tmp;
	i2c_read(TPI_ADDRESS, reg, &tmp);
	tmp = (tmp & ~mask) | (set & mask);
	i2c_write(TPI_ADDRESS, reg, tmp);
}

static int sil9024a_reset()
{
	uint8_t deviceId;
	uint8_t deviceProdRevId;
	uint8_t tpiScheme;
	uint8_t tmp;

	// Reset SIL9022 device.
	for (;;)
	{
		volatile uint32_t* led = (volatile uint32_t*)LED_BASE;
		*led = 0;
		timer_wait_ms(250);
		*led = 1;
		timer_wait_ms(150);

		// Set terminations.
		i2c_write(TPI_ADDRESS, 0x82, 0x25);

		// HW debounce to 64 ms.
		i2c_write(TPI_ADDRESS, 0x7c, 0x14);

		timer_wait_ms(250);

		// Enable TPI mode.
		i2c_write(TPI_ADDRESS, 0xc7, 0x00);

		timer_wait_ms(250);

		// // Read device identification.
		i2c_read(TPI_ADDRESS, TPI_DEVICE_ID, &deviceId);
		i2c_read(TPI_ADDRESS, 0x1c, &deviceProdRevId);
		i2c_read(TPI_ADDRESS, 0x1d, &tpiScheme);

		if (deviceId == 0xb0)
			break;

		timer_wait_ms(500);
	}

	printf("SIL90242 device found; id %02x, %02x, %02x.\n", deviceId, deviceProdRevId, tpiScheme);

	
	// enable active mode.
	// i2c_read(TPI_ADDRESS, TPI_DEVICE_POWER_STATE_CTRL_REG, &tmp);
	// printf("power state ctrl reg %02x\n", tmp);
	// i2c_write(TPI_ADDRESS, TPI_DEVICE_POWER_STATE_CTRL_REG, tmp & (~0x03));



	// select input bus characteristics.

	const uint16_t clk = 2500;
	const uint16_t vfreq = 60;
	const uint16_t pixels = 800;
	const uint16_t lines = 525;

	// pixel clock 25 MHz
	i2c_write(TPI_ADDRESS, 0x00, clk & 255); i2c_write(TPI_ADDRESS, 0x01, clk >> 8);

	// vfreq 60hz
	i2c_write(TPI_ADDRESS, 0x02, vfreq & 255); i2c_write(TPI_ADDRESS, 0x03, vfreq >> 8);

	// pixels 800
	i2c_write(TPI_ADDRESS, 0x04, pixels & 255); i2c_write(TPI_ADDRESS, 0x05, pixels >> 8);

	// lines 525
	i2c_write(TPI_ADDRESS, 0x06, lines & 255); i2c_write(TPI_ADDRESS, 0x07, lines >> 8);

	i2c_write(TPI_ADDRESS, 0x08, 0b01110000);



	modify_register_bits(0x60, 0b10000000, 0b00000000);	// external sync
	modify_register_bits(0x61, 0b00000111, 0b00000000);	// non-interlaced, vsync active high, hsync active high
	modify_register_bits(0x63, 0b01000000, 0b00000000);	// disable DE generator.


	/*
	// setup sync decoding.
	modify_register_bits(0x60, 0b10000000, 0b00000000);	// external sync
	modify_register_bits(0x61, 0b00000111, 0b00000000);	// non-interlaced, vsync active high, hsync active high
	
	i2c_write(TPI_ADDRESS, 0x62, 16);	// h delay

	modify_register_bits(0x63, 0b01110011, 0b01000000);
	i2c_write(TPI_ADDRESS, 0x64, 10);	// v front porch
	i2c_write(TPI_ADDRESS, 0x66, 640 & 255);	// active pixels
	i2c_write(TPI_ADDRESS, 0x67, (640 >> 8) & 255);
	i2c_write(TPI_ADDRESS, 0x68, 480 & 255);	// active lines
	i2c_write(TPI_ADDRESS, 0x69, (480 >> 8) & 255);	
	*/


	// disable TMDS
	i2c_write(TPI_ADDRESS, 0x1a, 0x11);

	// enter D0 state
	i2c_write(TPI_ADDRESS, 0x1e, 0x00);

	// enable TMDS
	i2c_write(TPI_ADDRESS, 0x1a, 0x01);



	// sil9024a_read_edid();


	printf("SIL90242 device initialized.\n");
	return 0;
}

int sil9024a_init()
{
	sil9024a_reset();
	return 0;
}
