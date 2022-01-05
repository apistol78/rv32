#include "I2C.h"

#define I2C_RD_SDA() \
	((*I2C_BASE & 1) == 1)
#define I2C_WR_SDA_LOW() \
	{ *I2C_BASE = 0x0010; }
#define I2C_WR_SDA_HIGH() \
	{ *I2C_BASE = 0x0011; }

#define I2C_RD_SCL() \
	((*I2C_BASE & 2) == 2)
#define I2C_WR_SCL_LOW() \
	{ *I2C_BASE = 0x0020; }
#define I2C_WR_SCL_HIGH() \
	{ *I2C_BASE = 0x0022; }

void i2c_dly()
{
	for (uint32_t i = 0; i < 10; ++i)
		__asm__ volatile ("nop");
}

void i2c_start()
{
	I2C_WR_SDA_HIGH();	// i2c start bit sequence
	i2c_dly();
	I2C_WR_SCL_HIGH();
	i2c_dly();
	I2C_WR_SDA_LOW();
	i2c_dly();
	I2C_WR_SCL_LOW();
	i2c_dly();
}

void i2c_stop()
{
	I2C_WR_SDA_LOW();	// i2c stop bit sequence
	i2c_dly();
	I2C_WR_SCL_HIGH();
	i2c_dly();
	I2C_WR_SDA_HIGH();
	i2c_dly();
}

uint8_t i2c_rx(uint8_t ack)
{
	uint8_t x, d = 0;

	I2C_WR_SDA_HIGH(); 
	for (x = 0; x < 8; x++)
	{
		d <<= 1;
		do
		{
			I2C_WR_SCL_HIGH();
		}
		while (I2C_RD_SCL() == 0);    // wait for any SCL clock stretching
		i2c_dly();
		if (I2C_RD_SDA())
			d |= 1;
		I2C_WR_SCL_LOW();
	} 

	if (ack)
		{ I2C_WR_SDA_LOW(); }
	else
		{ I2C_WR_SDA_HIGH(); }

	I2C_WR_SCL_HIGH();
	i2c_dly();	// send (N)ACK bit
	I2C_WR_SCL_LOW();
	I2C_WR_SDA_HIGH();
	return d;
}

uint8_t NO_OPTIMIZE i2c_tx(uint8_t d)
{
	for (uint32_t i = 0; i < 8; ++i)
	{
		if (d & 0x80)
			{ I2C_WR_SDA_HIGH(); }
		else
			{ I2C_WR_SDA_LOW(); }

		i2c_dly();

		I2C_WR_SCL_HIGH();
		i2c_dly();
		I2C_WR_SCL_LOW();
		i2c_dly();

		d <<= 1;
	}

	I2C_WR_SDA_HIGH();
	i2c_dly();
	I2C_WR_SCL_HIGH();
	i2c_dly();
	uint8_t b = I2C_RD_SDA();	// possible ACK bit
	I2C_WR_SCL_LOW();
	i2c_dly();

	return b;
}

void i2c_write(uint8_t deviceAddr, uint8_t controlAddr, uint8_t controlData)
{
	i2c_start();
	i2c_tx(deviceAddr);
	i2c_tx(controlAddr);
	i2c_tx(controlData);
	i2c_stop();
}

void i2c_read(uint8_t deviceAddr, uint8_t controlAddr, uint8_t* outControlData)
{
	i2c_start();
	i2c_tx(deviceAddr);
	i2c_tx(controlAddr);
	i2c_start();  // restart
	i2c_tx(deviceAddr | 1);
	*outControlData = i2c_rx(0);  // read
	i2c_stop();
}
