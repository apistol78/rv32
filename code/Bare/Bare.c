
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

// I2C pins
#define I2C_RD_SDA() \
	((*(volatile uint32_t*)0x50000030 & 1) == 1)
#define I2C_WR_SDA_LOW() \
	{ *(volatile uint32_t*)0x50000030 = 0x0010; }
#define I2C_WR_SDA_HIGH() \
	{ *(volatile uint32_t*)0x50000030 = 0x0011; }

#define I2C_RD_SCL() \
	((*(volatile uint32_t*)0x50000030 & 2) == 2)
#define I2C_WR_SCL_LOW() \
	{ *(volatile uint32_t*)0x50000030 = 0x0020; }
#define I2C_WR_SCL_HIGH() \
	{ *(volatile uint32_t*)0x50000030 = 0x0022; }

/*
// GPIO pins
#define I2C_RD_SDA() \
	((*(volatile uint32_t*)0x50000024 & 1) == 1)
#define I2C_WR_SDA_LOW() \
	{ *(volatile uint32_t*)0x50000024 = 0x00010000; }
#define I2C_WR_SDA_HIGH() \
	{ *(volatile uint32_t*)0x50000024 = 0x00010001; }

#define I2C_RD_SCL() \
	((*(volatile uint32_t*)0x50000024 & 2) == 2)
#define I2C_WR_SCL_LOW() \
	{ *(volatile uint32_t*)0x50000024 = 0x00020000; }
#define I2C_WR_SCL_HIGH() \
	{ *(volatile uint32_t*)0x50000024 = 0x00020002; }
*/

/*
void i2c_dly()
{
	for (volatile uint32_t i = 0; i < 10; ++i)
		;
}

void i2c_start(void)
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

void i2c_stop(void)
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

uint8_t i2c_tx(uint8_t d)
{
	uint8_t x;
	uint8_t b;

	for (x = 8; x; x--)
	{
		if (d & 0x80)
			{ I2C_WR_SDA_HIGH(); }
		else
			{ I2C_WR_SDA_LOW(); }

		I2C_WR_SCL_HIGH();
		d <<= 1;
		i2c_dly();
		I2C_WR_SCL_LOW();
	}

	I2C_WR_SDA_HIGH();
	I2C_WR_SCL_HIGH();
	i2c_dly();
	b = I2C_RD_SDA();	// possible ACK bit
	I2C_WR_SCL_LOW();

	return b;
}


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
	{ 0xfa, 0x7d },
	{ 0x00, 0x00 }	// end
};

void adv7513_power_up()
{
	for (int i = 0; defaultConfig[i].reg != 0; ++i)
	{
		i2c_start();
		i2c_tx(0x72);
		i2c_tx(defaultConfig[i].reg);
		i2c_tx(defaultConfig[i].val);
		i2c_stop();
	}
}
*/


void main()
{
	volatile uint32_t* video = (volatile uint32_t*)0x40000000;
	volatile uint32_t* vctrl = (volatile uint32_t*)0x4ffffff0;

	//volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	// *leds = 0x00;

	// adv7513_power_up();


	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;


	for (;;)
	{
		for (int y = 0; y < 240; ++y)
		{
			for (int x = 0; x < 320; ++x)
			{
				video[x + y * 320] = (r << 16) | (g << 8) | b;
				r++;
			}
			g++;
		}
		b++;

		*vctrl = 1;
	}

}