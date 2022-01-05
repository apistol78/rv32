#include "Runtime/HAL/I2C.h"
#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/Video.h"

#define SLAVE_ADDR 0x72

const struct {
	uint8_t reg;
	uint8_t val;
} const defaultConfig[] =
{
	{ 0x98, 0x03 },
	{ 0x01, 0x00 },
	{ 0x02, 0x18 },
	{ 0x03, 0x00 },
	{ 0x14, 0x70 },
	{ 0x15, 0x20 },
	{ 0x16, 0x30 },
	{ 0x18, 0x46 },
	{ 0x40, 0x80 },
	{ 0x41, 0x10 },
	{ 0x49, 0xa8 },
	{ 0x55, 0x10 },
	{ 0x56, 0x08 },
	{ 0x96, 0xf6 },
	{ 0x73, 0x07 },
	{ 0x76, 0x1f },
	{ 0x98, 0x03 },
	{ 0x99, 0x02 },
	{ 0x9a, 0xe0 },
	{ 0x9c, 0x30 },
	{ 0x9d, 0x61 },
	{ 0xa2, 0xa4 },
	{ 0xa3, 0xa4 },
	{ 0xa5, 0x04 },
	{ 0xab, 0x40 },
	{ 0xaf, 0x16 },
	{ 0xba, 0x60 },
	{ 0xd1, 0xff },
	{ 0xde, 0x10 },
	{ 0xe4, 0x60 },
	{ 0xfa, 0x7d }
};

void reg_update_bits(int reg, int mask, int data)
{
	uint8_t regv = 0;

	i2c_read(SLAVE_ADDR, reg, &regv);

	regv &= ~((uint8_t)mask);
	regv |= ((uint8_t)data & (uint8_t)mask);

	i2c_write(SLAVE_ADDR, reg, regv);
}

#define ADV7513_REG_CHIP_REVISION				0x00
#define ADV7513_REG_CHIP_ID_HIGH				0xf5
#define ADV7513_REG_CHIP_ID_LOW					0xf6

int adv7513_chip_identify()
{
	uint8_t rev = 0;
	uint8_t id0 = 0;
	uint8_t id1 = 0;

	i2c_read(SLAVE_ADDR, ADV7513_REG_CHIP_REVISION, &rev);
	i2c_read(SLAVE_ADDR, ADV7513_REG_CHIP_ID_LOW, &id0);
	i2c_read(SLAVE_ADDR, ADV7513_REG_CHIP_ID_HIGH, &id1);

	if (id0 == 0x11 && id1 == 0x75)
	{
		print("Identified ADV"); printHex(id1); printHex(id0); print(" rev: "); printHex(rev); printLn("");
        return 1;
	}
	else
	{
		printLn("Unknown HDMI encoder chip.");
		print("Chip revision: "); printHex(rev); printLn("");
		print("Chip ID 0: "); printHex(id0); printLn("");
		print("Chip ID 1: "); printHex(id1); printLn("");
        return 0;
	}
}

void adv7513_kick_up()
{
	// power up the encoder
	reg_update_bits(0x41, 0x40, 0);
}

void adv7513_power_up()
{
	for (int i = 0; i < sizeof(defaultConfig) / sizeof(defaultConfig[0]); ++i)
        i2c_write(0x72, defaultConfig[i].reg, defaultConfig[i].val);
}

int video_init()
{
	if (adv7513_chip_identify())
    {
    	adv7513_kick_up();
    	adv7513_power_up();
    }
    return 1;
}
