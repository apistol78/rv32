//#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
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

/*

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

*/


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


// CRC


/* Table for CRC-7 (polynomial x^7 + x^3 + 1) */
static const uint8_t crc7_syndrome_table[256] = {
	  0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	  0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	  0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	  0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	  0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	  0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	  0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	  0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	  0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	  0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	  0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	  0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	  0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	  0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	  0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	  0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	  0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	  0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	  0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	  0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	  0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	  0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	  0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	  0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	  0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	  0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	  0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	  0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	  0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	  0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	  0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	  0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

uint8_t crc7_byte(uint8_t crc, uint8_t data)
{
		return crc7_syndrome_table[(crc << 1) ^ data];
}

uint8_t crc7(uint8_t crc, const uint8_t *buffer, int len)
{
	  while (len--)
			crc = crc7_byte(crc, *buffer++);
	  return crc;
}



// SD
/*
|x|x|x|x|x|x|x|x|dat|dat|dat|dat|cmd|dat dir|cmd dir|clk|
*/

#define SD_VHS_2V7_3V6				0x01
#define CMD8_DEFAULT_TEST_PATTERN	0xaa


#define SD_WR_CLK_LOW() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000100; }
#define SD_WR_CLK_HIGH() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000101; }

#define SD_WR_CMD_DIR_IN() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000200; }
#define SD_WR_CMD_DIR_OUT() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000202; }

#define SD_WR_CMD_LOW() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000800; }
#define SD_WR_CMD_HIGH() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000808; }
#define SD_RD_CMD() \
	( (*(volatile uint32_t*)0x50000040 & 0x00000008) == 0x00000008 )

#define SD_WR_DAT_DIR_IN() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000400; }
#define SD_WR_DAT_DIR_OUT() \
	{ *(volatile uint32_t*)0x50000040 = 0x00000404; }
#define SD_WR_DAT(d4) \
	{ *(volatile uint32_t*)0x50000040 = 0x0000f000 | ((d4 & 15) << 4); }
#define SD_RD_DAT() \
	( (*(volatile uint32_t*)0x50000040 & 0x000000f0) >> 4 )

void sd_init()
{
	*(volatile uint32_t*)0x50000040 = 0x0000ff00;

	SD_WR_CMD_DIR_OUT();
	SD_WR_DAT_DIR_OUT();
	SD_WR_CLK_HIGH();
	SD_WR_CMD_HIGH();
	SD_WR_DAT(0x0);
}

void sd_dummy_clock(int clockCnt)
{
	for (int i = 0; i < clockCnt; ++i)
	{
        SD_WR_CLK_LOW();
		i2c_dly();
        SD_WR_CLK_HIGH();
		i2c_dly();
    }
}

void sd_send_cmd(uint8_t cmd[6], int cmdLen)
{
	uint8_t mask, data;
	
	SD_WR_CMD_DIR_OUT();
	for (int i = 0; i < cmdLen; i++)
	{
		mask = 0x80;
		data = cmd[i];

		for (int k = 0; k < 8; k++)
		{
			SD_WR_CLK_LOW();
			
			if (data & mask)
				{ SD_WR_CMD_HIGH(); }
			else
				{ SD_WR_CMD_LOW(); }

			i2c_dly();

			SD_WR_CLK_HIGH();  

			mask >>= 1;
			i2c_dly();
		}
	}
}

int sd_get_response(uint8_t* outResponse, int responseLen)
{
	const int maxCnt = 40; // !!!! Note. the value should be large than 8

	SD_WR_CMD_DIR_IN();
	
	//===== check start bit == 0
	for (int cnt = 0;;)
	{
		SD_WR_CLK_LOW();
		SD_WR_CLK_HIGH();
		if (!SD_RD_CMD())
			break;
		else if (cnt++ > maxCnt)
		{
			printLn("SD response timeout");
			return 0;
		}
		i2c_dly();
	}
  
	//===== check transmitter bit == 0
	SD_WR_CLK_LOW();
	SD_WR_CLK_HIGH();
	if (SD_RD_CMD())
	{
		printLn("SD invalid transmitter bit");
		return 0;
	}

	//===== read content + CRC + end-bits ======
	int value = 0;
	int bit = 2;
	for (int index = 0; index < responseLen; )
	{
		SD_WR_CLK_LOW();
		SD_WR_CLK_HIGH();

		if (SD_RD_CMD())
		{
			value |= 0x80 >> bit;
		}

		if (bit >= 7)
		{
			outResponse[index] = value;
			index++;
			value = 0;
			bit = 0;
		}
		else
			bit++;
	} 
	
	// A command with response. 8 clocks after the card response end bit.
	sd_dummy_clock(8);
	return 1;	
}

int sd_cmd0()
{
	uint8_t cmd[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[1];
	uint8_t crc;

	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD0: No response from SD card");
		return 0;
	}

	printHex(response[0]);
	printLn("");

	printLn("sd_cmd0 OK");
	return 1;
}

int sd_cmd8(uint8_t voltId, uint8_t testPattern)
{
	const uint8_t c_cmd = 8;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[3] |= (voltId & 0x0f);
	cmd[4] = testPattern;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));
	
	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD8: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("CMD8: Invalid command");
		return 0;
	}
	if (response[3] != voltId)
	{
		printLn("CMD8: Invalid voltage id");
		return 0;
	}        
	if (response[4] != testPattern)
	{
		printLn("CMD8: Invalid test pattern");
		return 0;
	}
	if (crc7(0, response, 5) != (response[5] >> 1))
	{
		printLn("CMD8: Invalid checksum");
		return 0;
	}
	if ((response[5] & 0x01) != 0x01)
	{
		printLn("CMD8: Invalid end bits");
		return 0;
	}

	printLn("sd_cmd8 OK");
	return 1;
}

int sd_cmd55(uint16_t rca16)
{
	const uint8_t c_cmd = 55;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] = rca16 >> 8;
	cmd[2] = rca16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD55: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("CMD55: Invalid command");
		return 0;
	}
	// \todo check card status
	if (crc7(0, response, 5) != (response[5] >> 1))
	{
		printLn("CMD55: Invalid checksum");
		return 0;
	}
	if ((response[5] & 0x01) != 0x01)
	{
		printLn("CMD55: Invalid end bits");
		return 0;
	}

	printLn("sd_cmd55 OK");
	return 1;
}

int sd_acmd41(uint32_t hostOCR32, uint32_t* outOCR)
{
	const uint8_t c_cmd = 41;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] |= (hostOCR32 >> 24) & 0x40;
	cmd[2] = (hostOCR32 >> 16) & 0xff;
	cmd[3] = (hostOCR32 >> 8) & 0xff;
	cmd[4] = hostOCR32 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("ACMD41: No response from SD card");
		return 0;
	}

	// \todo verify response

	uint32_t OCR = 0;
	for(int i = 0; i < 4; i++)
	{
		OCR <<= 8;
		OCR |= response[i + 1];
	}
	*outOCR = OCR;

	printLn("sd_acmd41 OK");
	return 1;
}

int sd_cmd2(uint8_t* cid, int cidLen)
{
	const uint8_t c_cmd = 2;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[17];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD2: No response from SD card");
		return 0;
	}

	// \todo verify response

	int copyLen = cidLen;
	if (copyLen > 16)
		copyLen = 16;

	for (int i = 0; i < copyLen; ++i)
		cid[i] = response[i + 1];

	printLn("sd_cmd2 OK");
	return 1;
}

int sd_cmd3(uint16_t* outRCA16)
{
	const uint8_t c_cmd = 3;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD3: No response from SD card");
		return 0;
	}

	*outRCA16 = (response[1] << 8) | response[2];

	printLn("sd_cmd3 OK");
	return 1;
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

	printLn("Initialize SD card...");
	
	sd_init();

	for (;;)
	{
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
		
		sd_dummy_clock(100);
		if (sd_cmd0())
			break;
	}

	for (uint32_t i = 0; i < 1000000; ++i)
		__asm__ volatile ("nop");

	sd_cmd8(SD_VHS_2V7_3V6, CMD8_DEFAULT_TEST_PATTERN);

	uint32_t hostOCR32 = 0x40FF8000;
	uint32_t OCR = 0;
	uint16_t RCA16 = 0;

	for (;;)
	{
		if (!sd_cmd55(0x0000))
		{
			for (uint32_t i = 0; i < 1000000; ++i)
				__asm__ volatile ("nop");
			continue;
		}

		if (!sd_acmd41(hostOCR32, &OCR))
		{
			for (uint32_t i = 0; i < 1000000; ++i)
				__asm__ volatile ("nop");
			continue;
		}

		// success
		break;
	}

	/*
	uint8_t cid[16];
	sd_cmd2(cid, sizeof(cid));

	print("SD CID: ");
	for (int i = 0; i < 16; ++i)
		printHex(cid[i]);
	printLn("");
	*/

	sd_cmd3(&RCA16);
	print("RCA ");
	printHex(RCA16 >> 16);
	printHex(RCA16);
	printLn("");

	printLn("Initialization done, executing demo...");

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
