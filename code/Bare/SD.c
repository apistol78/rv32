#include "Common.h"
#include "CRC.h"
#include "Print.h"

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

void sd_dummy_clock(int clockCnt)
{
	for (int i = 0; i < clockCnt; ++i)
	{
        SD_WR_CLK_LOW();
        SD_WR_CLK_HIGH();
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

			SD_WR_CLK_HIGH();  

			mask >>= 1;
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

void sd_init()
{
	*SD_BASE = 0x0000ff00;

	SD_WR_CMD_DIR_OUT();
	SD_WR_DAT_DIR_OUT();
	SD_WR_CLK_HIGH();
	SD_WR_CMD_HIGH();
	SD_WR_DAT(0x0);

    // Set card to idle.
	sd_dummy_clock(100);
	sd_cmd0();

//	for (uint32_t i = 0; i < 1000000; ++i)
//		__asm__ volatile ("nop");

	sd_cmd8(SD_VHS_2V7_3V6, CMD8_DEFAULT_TEST_PATTERN);

	uint32_t hostOCR32 = 0x40FF8000;
	uint32_t OCR = 0;
	uint16_t RCA16 = 0;

	sd_cmd55(0x0000);
	sd_acmd41(hostOCR32, &OCR);

	uint8_t cid[16];
	sd_cmd2(cid, sizeof(cid));

	print("SD CID: ");
	for (int i = 0; i < 16; ++i)
		printHex(cid[i]);
	printLn("");

	sd_cmd3(&RCA16);
	print("RCA ");
	printHex(RCA16 >> 16);
	printHex(RCA16);
	printLn("");    
}
