#include "Common.h"
#include "CRC.h"
#include "Print.h"

#define SD_4BIT_MODE

/*
|x|x|x|x|x|x|x|x|dat|dat|dat|dat|cmd|dat dir|cmd dir|clk|
*/
#define SD_WR_CLK_LOW() \
	{ *SD_BASE = 0x00000100; }
#define SD_WR_CLK_HIGH() \
	{ *SD_BASE = 0x00000101; }

#define SD_WR_CMD_DIR_IN() \
	{ *SD_BASE = 0x00000200; }
#define SD_WR_CMD_DIR_OUT() \
	{ *SD_BASE = 0x00000202; }

#define SD_WR_CMD_LOW() \
	{ *SD_BASE = 0x00000800; }
#define SD_WR_CMD_HIGH() \
	{ *SD_BASE = 0x00000808; }
#define SD_RD_CMD() \
	( (*SD_BASE & 0x00000008) == 0x00000008 )

#define SD_WR_DAT_DIR_IN() \
	{ *SD_BASE = 0x00000400; }
#define SD_WR_DAT_DIR_OUT() \
	{ *SD_BASE = 0x00000404; }
#define SD_WR_DAT(d4) \
	{ *SD_BASE = 0x0000f000 | ((d4 & 15) << 4); }
#define SD_RD_DAT() \
	( (*SD_BASE & 0x000000f0) >> 4 )

#define SD_VHS_2V7_3V6				0x01
#define CMD8_DEFAULT_TEST_PATTERN	0xaa

typedef enum
{
    SD_STATE_IDLE = 0,
    SD_STATE_READY = 1,    
    SD_STATE_IDENT = 2,
    SD_STATE_STBY = 3,
    SD_STATE_TRAN    
} SD_CURRENT_STATE;

void __attribute__ ((noinline)) sd_half_cycle()
{
//	for (int i = 0; i < 5; ++i)
//		__asm__ volatile ("nop");
}

void sd_dummy_clock(uint32_t clockCnt)
{
	for (uint32_t i = 0; i < clockCnt; ++i)
	{
        SD_WR_CLK_LOW();
		sd_half_cycle();
        SD_WR_CLK_HIGH();
		sd_half_cycle();
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

			sd_half_cycle();

			SD_WR_CLK_HIGH();  

			mask >>= 1;

			sd_half_cycle();
		}
	}
}

int sd_get_response(uint8_t* outResponse, int responseLen)
{
	const int maxCnt = 40; // !!!! Note. the value should be large than 8

	SD_WR_CMD_LOW();
	SD_WR_CMD_DIR_IN();
	
	//===== check start bit == 0
	for (int cnt = 0;;)
	{
		SD_WR_CLK_LOW();
		sd_half_cycle();

		SD_WR_CLK_HIGH();
		sd_half_cycle();

		if (!SD_RD_CMD())
			break;
		else if (cnt++ > maxCnt)
		{
			printLn("SD response timeout");
			return 0;
		}
	}
  
	//===== check transmitter bit == 0
	SD_WR_CLK_LOW();
	sd_half_cycle();

	SD_WR_CLK_HIGH();
	sd_half_cycle();

	if (SD_RD_CMD())
	{
		printLn("SD invalid transmitter bit");
		return 0;
	}

	//===== read content + CRC + end-bits ======
	//printLn("Reading content...");

	int value = 0;
	int bit = 2;
	for (int index = 0; index < responseLen; )
	{
		SD_WR_CLK_LOW();
		sd_half_cycle();

		SD_WR_CLK_HIGH();
		sd_half_cycle();

		if (SD_RD_CMD())
		{
			value |= 0x80 >> bit;
			//print("H");
		}
		//else
			//print("L");

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
	//printLn("");

	// A command with response. 8 clocks after the card response end bit.
	sd_dummy_clock(8);

	//printLn("END OF GET RESP");
	return 1;	
}

int sd_cmd0()
{
	static uint8_t cmd[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static uint8_t response[1];
	uint8_t crc;

	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	sd_dummy_clock(100);
	return 1;
}

int sd_cmd8(uint8_t voltId, uint8_t testPattern)
{
	const uint8_t c_cmd = 8;

	static uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static uint8_t response[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
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
		//return 0;
	}
	if (response[3] != voltId)
	{
		printLn("CMD8: Invalid voltage id");
		//return 0;
	}        
	if (response[4] != testPattern)
	{
		printLn("CMD8: Invalid test pattern");
		//return 0;
	}
	if (crc7(0, response, 5) != (response[5] >> 1))
	{
		printLn("CMD8: Invalid checksum");
		//return 0;
	}
	if ((response[5] & 0x01) != 0x01)
	{
		printLn("CMD8: Invalid end bits");
		//return 0;
	}

	printLn("sd_cmd8 OK");
	return 1;
}

int sd_cmd55(uint16_t rca16)
{
	const uint8_t c_cmd = 55;

	static uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static uint8_t response[6];
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

	static uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static uint8_t response[6];
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

	if (response[0] != 0x3f)
	{
		printLn("ACMD41: Invalid response from SD card");
		return 0;		
	}

	uint32_t OCR = 0;
	for(int i = 0; i < 4; i++)
	{
		OCR <<= 8;
		OCR |= response[i + 1];
	}

	if ((OCR & 0x80000000) != 0x80000000)
	{
		printLn("ACMD41: SD card is busy");
		return 0;
	}

	if (response[5] != 0xff)
	{
		printLn("ACMD41: Invalid reserved & end-bits");
		return 0;		
	}

	*outOCR = OCR;

	printLn("sd_acmd41 OK");
	return 1;
}

int sd_cmd2(uint8_t* cid, int cidLen)
{
	const uint8_t c_cmd = 2;

	static uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static uint8_t response[17];
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

int sd_cmd9(uint16_t RCA16, uint8_t* outCSD, int CSDLen)
{
	const uint8_t c_cmd = 9;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[17];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] |= RCA16 >> 8;
	cmd[2] |= RCA16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD9: No response from SD card");
		return 0;
	}

	if (response[0] != 0x3f)
	{
		printLn("CMD9: Invalid response from SD card");
		return 0;		
	}

	// \todo verify response

	int copyLen = CSDLen;
	if (copyLen > 16)
		copyLen = 16;

	for (int i = 0; i < copyLen; ++i)
		outCSD[i] = response[i + 1];

	printLn("sd_cmd9 OK");
	return 1;
}

int sd_cmd7(uint16_t RCA16)
{
	const uint8_t c_cmd = 7;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] |= RCA16 >> 8;
	cmd[2] |= RCA16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD7: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("CMD9: Invalid response from SD card");
		return 0;		
	}

	printLn("sd_cmd7 OK");
	return 1;
}

int sd_cmd16(uint32_t blockLength)
{
	const uint8_t c_cmd = 16;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] |= blockLength >> 24;
	cmd[2] |= blockLength >> 16;
	cmd[3] |= blockLength >> 8;
	cmd[4] |= blockLength;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD16: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("CMD16: Invalid response from SD card");
		return 0;		
	}

	printLn("sd_cmd16 OK");
	return 1;
}

int sd_cmd17(uint32_t addr)
{
	const uint8_t c_cmd = 17;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[1] |= addr >> 24;
	cmd[2] |= addr >> 16;
	cmd[3] |= addr >> 8;
	cmd[4] |= addr;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("CMD17: No response from SD card");
		return 0;
	}

	printLn("sd_cmd17 OK");
	return 1;
}

int sd_acmd6(int bus4)
{
	const uint8_t c_cmd = 6;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[4] |= bus4 ? 2 : 0;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("ACMD6: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("ACMD6: Invalid response from SD card");
		return 0;		
	}

	printLn("sd_acmd6 OK");
	return 1;
}

int sd_acmd42(int bus4)
{
	const uint8_t c_cmd = 42;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	// build packet    
	cmd[0] |= c_cmd;
	cmd[4] |= bus4 ? 2 : 0;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
	{
		printLn("ACMD42: No response from SD card");
		return 0;
	}

	if (response[0] != c_cmd)
	{
		printLn("ACMD42: Invalid response from SD card");
		return 0;		
	}

	printLn("sd_acmd42 OK");
	return 1;
}

int sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen)
{
	uint32_t addr = block;	// SDHC take block number.
	// \todo support non SDHC

	if (!sd_cmd17(addr)) // , SD_STATE_TRAN))
		return 0;

	SD_WR_DAT_DIR_IN();

	// Wait on start bits.
	int try = 0;
	while(1)
	{
        SD_WR_CLK_LOW();
		sd_half_cycle();
        SD_WR_CLK_HIGH();
		sd_half_cycle();
#ifdef SD_4BIT_MODE
		if((SD_RD_DAT() & 0x0F) == 0x00) // check start bits (zero is expected)
#else      
		if((SD_RD_DAT() & 0x01) == 0x00) // check start bits (zero is expected)
#endif      
			break;
		if (try++ > 9000)
			return 0;        
	}

    // Read data (512byte = 1 block)
	for(uint32_t i = 0; i < bufferLen; i++)
	{
		uint8_t Data8 = 0;
#ifdef SD_4BIT_MODE
		for(int j = 0; j < 2; j++)
		{
			SD_WR_CLK_LOW();
			sd_half_cycle();
			SD_WR_CLK_HIGH();
			sd_half_cycle();
			Data8 <<= 4; 
			Data8 |= (SD_RD_DAT() & 0x0F);
		}
#else      
		for(int j = 0; j < 8; j++)
		{
			SD_WR_CLK_LOW();
			sd_half_cycle();
			SD_WR_CLK_HIGH();
			sd_half_cycle();
			Data8 <<= 1; 
			if (SD_RD_DAT() & 0x01)  // check bit0
				Data8 |= 0x01;
		} 
#endif  

		buffer[i] = Data8;
	}

	return 1;
}

void sd_init()
{
	*SD_BASE = 0x0000ff00;

	SD_WR_CMD_DIR_OUT();
	SD_WR_DAT_DIR_IN();
	SD_WR_CLK_HIGH();
	sd_half_cycle();
	SD_WR_CMD_HIGH();
	sd_half_cycle();
	SD_WR_DAT(0x0);

	sd_dummy_clock(100);

    // Set card to idle.
	sd_cmd0();

	// Negotiate voltage.
	sd_cmd8(SD_VHS_2V7_3V6, CMD8_DEFAULT_TEST_PATTERN);

	// Determine capacity.
	uint32_t hostOCR32 = 0x40FF8000;
	uint32_t OCR = 0;
	uint16_t RCA16 = 0;

	for (int count = 0;; ++count)
	{
		if (!sd_cmd55(0x0000))
			return;

		if (!sd_acmd41(hostOCR32, &OCR))
		{
			if (count > 10)
				return;

			for (int i = 0; i < 100000; ++i)
				__asm__ volatile ("nop");
		}
		else
			break;
	}

	// Get card CID.
	uint8_t cid[16];
	sd_cmd2(cid, sizeof(cid));

	print("SD CID: ");
	for (int i = 0; i < 16; ++i)
		printHex(cid[i]);
	printLn("");

	// Get card RCA identifier.
	sd_cmd3(&RCA16);
	print("RCA ");
	printHex(RCA16 >> 16);
	printHex(RCA16);
	printLn("");

	// Enter data transfer mode, standby state.
	uint8_t csd[17];
	sd_cmd9(RCA16, csd, sizeof(csd));

	print("SD CSD: ");
	for (int i = 0; i < 16; ++i)
		printHex(csd[i]);
	printLn("");

	// cmd10?

	// Enter transfer state.
	sd_cmd7(RCA16);

	// Select block length.
	sd_cmd16(512);

	// If (4bit)
	// Set bus width.
	sd_cmd55(RCA16);
	sd_acmd6(1);
	sd_cmd55(RCA16);
	sd_acmd42(1);

	// Test read first block.
	static uint8_t block[512];
	if (sd_read_block512(0, block, 512))
	{
		printLn("Got block!");
		for (uint32_t i = 0; i < 512; )
		{
			printHex(i >> 8);
			printHex(i);
			print(": ");

			for (uint32_t j = 0; j < 16; ++j)
				printHex(block[i++]);

			printLn("");
		}
	}
	else
		printLn("Unable to read block!");
}
