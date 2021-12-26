//#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;



// OUTPUT


void uart_tx_u8(uint8_t data)
{
	volatile uint32_t* uart = (volatile uint32_t*)0x50000010;
	*uart = (uint32_t)data;
}

void print(const char* str)
{
	for (const char* c = str; *c; ++c)
		uart_tx_u8(*c);
}

void printHex(uint8_t v)
{
	const char hex[] = { "0123456789abcdef" };
	uart_tx_u8(hex[v >> 4]);
	uart_tx_u8(hex[v & 15]);
}

void printLn(const char* str)
{
	print(str);
	print("\r\n");
}


// GPIO


#define GPIO_INPUT	0
#define GPIO_OUTPUT	1

#define GPIO_LOW	0
#define GPIO_HIGH	1

void gpio_set_direction(int pin, int direction)
{
	volatile uint32_t* gpio_ctrl = (volatile uint32_t*)0x50000020;
	uint32_t gc = *gpio_ctrl;
	if (direction == GPIO_INPUT)
		gc &= ~(1 << pin);
	else if (direction == GPIO_OUTPUT)
		gc |= (1 << pin);
	*gpio_ctrl = gc;		
}

void gpio_set_pin(int pin, int level)
{
	volatile uint32_t* gpio_io = (volatile uint32_t*)0x50000024;
	uint32_t gi = *gpio_io;
	if (level == GPIO_LOW)
		gi &= ~(1 << pin);
	else if (level == GPIO_HIGH)
		gi |= (1 << pin);
	*gpio_io = gi;
}



// I2C


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




void I2C_Write(uint8_t deviceAddr, uint8_t controlAddr, uint8_t controlData)
{
    i2c_start();
    i2c_tx(deviceAddr);
    i2c_tx(controlAddr);
    i2c_tx(controlData);
    i2c_stop();

	for (uint32_t i = 0; i < 1000000; ++i)
		__asm__ volatile ("nop");
}

void I2C_Read(uint8_t deviceAddr, uint8_t controlAddr, uint8_t* outControlData)
{
    i2c_start();
    i2c_tx(deviceAddr);
	i2c_tx(controlAddr);
    i2c_start();  // restart
    i2c_tx(deviceAddr | 1);
	*outControlData = i2c_rx(0);  // read
    i2c_stop();
}



// VIDEO



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

#define SLAVE_ADDR 0x72


void reg_update_bits(int reg, int mask, int data)
{
	uint8_t regv = 0;

	I2C_Read(SLAVE_ADDR, reg, &regv);

	regv &= ~((uint8_t)mask);
	regv |= ((uint8_t)data & (uint8_t)mask);

	I2C_Write(SLAVE_ADDR, reg, regv);
}

#define ADV7513_REG_CHIP_REVISION				0x00
#define ADV7513_REG_CHIP_ID_HIGH				0xf5
#define ADV7513_REG_CHIP_ID_LOW					0xf6

void adv7513_chip_identify()
{
	uint8_t rev = 0;
	uint8_t id0 = 0;
	uint8_t id1 = 0;

	I2C_Read(SLAVE_ADDR, ADV7513_REG_CHIP_REVISION, &rev);
	I2C_Read(SLAVE_ADDR, ADV7513_REG_CHIP_ID_LOW, &id0);
	I2C_Read(SLAVE_ADDR, ADV7513_REG_CHIP_ID_HIGH, &id1);

	if (id0 == 0x11 && id1 == 0x75)
	{
		print("Identified ADV"); printHex(id1); printHex(id0); print(" rev: "); printHex(rev); printLn("");
	}
	else
	{
		printLn("Unknown HDMI encoder chip.");
		print("Chip revision: "); printHex(rev); printLn("");
		print("Chip ID 0: "); printHex(id0); printLn("");
		print("Chip ID 1: "); printHex(id1); printLn("");
	}
}

int adv7513_kick_up()
{
	// power up the encoder
	reg_update_bits(0x41, 0x40, 0);
}

void adv7513_power_up()
{
	for (int i = 0; i < sizeof(defaultConfig) / sizeof(defaultConfig[0]); ++i)
	{
		i2c_start();
		i2c_tx(0x72);
		i2c_tx(defaultConfig[i].reg);
		i2c_tx(defaultConfig[i].val);
		i2c_stop();
	}
}


// USER

void draw_line(int x0, int y0, int x1, int y1)
{
	volatile uint32_t* video = (volatile uint32_t*)0x40000000;
    int dx, dy, p, x, y;
 
	dx = x1 - x0;
	dy = y1 - y0;
	 
	x = x0;
	y = y0;
	 
	p = 2 * dy - dx;
	 
	while (x < x1)
	{
		if (p >= 0)
		{
			video[x + y * 320] = 0x00ffffff;
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			video[x + y * 320] = 0x00ffffff;
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}


void main()
{
	volatile uint32_t* video = (volatile uint32_t*)0x40000000;
	volatile uint32_t* vctrl = (volatile uint32_t*)0x4ffffff0;
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;

	*leds = 0x00000001;

	printLn("Identifying HDMI encoder...");	
	adv7513_chip_identify();

	printLn("Kick up HDMI encoder...");
	adv7513_kick_up();

	printLn("Power up HDMI encoder...");
	adv7513_power_up();

	int x1 = 50;

	for (;;)
	{
		
		int y1 = 50;
		int x2 = 200;
		int y2 = 150;

		draw_line(
			x1, y1,
			x2, y2
		);

		*vctrl = 1;

		if (++x1 > 300)
			x1 = 50;
	}
}
