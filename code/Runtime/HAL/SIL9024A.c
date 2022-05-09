#include <stdio.h>
#include "Runtime/HAL/I2C.h"
#include "Runtime/HAL/SIL9024A.h"

#define TPI_ADDRESS 0x72
#define CEC_ADDRESS 0xc0
#define REGS1_ADDRESS 0x72
#define REGS2_ADDRESS 0x7a

static int sil9024a_reset()
{
	uint8_t deviceId;
	uint8_t deviceProdRevId;
	uint8_t tpiScheme;

	i2c_write(TPI_ADDRESS, 0xc7, 0x00);

	// Read device identification.
	i2c_read(TPI_ADDRESS, 0x1b, &deviceId);
	i2c_read(TPI_ADDRESS, 0x1c, &deviceProdRevId);
	i2c_read(TPI_ADDRESS, 0x1d, &tpiScheme);

	if (deviceId != 0xb0)
	{
		printf("Unknown SIL9024 device; device id %02x is unknown.\n", deviceId);
		return 1;
	}

	return 0;
}

int sil9024a_init()
{
	sil9024a_reset();
	return 0;
}
