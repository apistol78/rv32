
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;


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

void i2c_dly()
{
	for (uint32_t i = 0; i < 10000; ++i)
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

	for(x = 8; x; x--)
	{
		if (d & 0x80)
			{ I2C_WR_SDA_HIGH(); }
		else
			{ I2C_WR_SDA_LOW(); }
		I2C_WR_SCL_HIGH();
		d <<= 1;
		I2C_WR_SCL_LOW();
	}

	I2C_WR_SDA_HIGH();
	I2C_WR_SCL_HIGH();
	i2c_dly();
	b = I2C_RD_SDA();	// possible ACK bit
	I2C_WR_SCL_LOW();

	return b;
}

void main()
{
	volatile uint32_t* gpio = (volatile uint32_t*)0x50000020;
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	uint8_t cnt = 1;
	int i;

	for (;;)
	{
		*leds = (uint32_t)cnt;

		*gpio = 0xffffffff;

		for (i = 0; i < 100000; ++i)
			;

		*gpio = 0x00000000;

		for (i = 0; i < 100000; ++i)
			;

		++cnt;
	}
}