#include <stdio.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/HAL/Common.h"
#include "Runtime/HAL/CRC.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"

#define SD_CTRL (volatile uint32_t*)(SD_BASE)

/*
|x|x|x|x|x|x|x|x|dat|dat|dat|dat|cmd|dat dir|cmd dir|clk|
*/
#define SD_WR_CLK_LOW() \
	{ *SD_CTRL = 0x00000100; }
#define SD_WR_CLK_HIGH() \
	{ *SD_CTRL = 0x00000101; }

#define SD_WR_CMD_DIR_IN() \
	{ *SD_CTRL = 0x00000200; }
#define SD_WR_CMD_DIR_OUT() \
	{ *SD_CTRL = 0x00000202; }

#define SD_WR_CMD_LOW() \
	{ *SD_CTRL = 0x00000800; }
#define SD_WR_CMD_HIGH() \
	{ *SD_CTRL = 0x00000808; }
#define SD_RD_CMD() \
	( (*SD_CTRL & 0x00000008) == 0x00000008 )

#define SD_WR_DAT_DIR_IN() \
	{ *SD_CTRL = 0x00000400; }
#define SD_WR_DAT_DIR_OUT() \
	{ *SD_CTRL = 0x00000404; }
#define SD_WR_DAT(d4) \
	{ *SD_CTRL = 0x0000f000 | ((d4 & 15) << 4); }
#define SD_WR_DAT_HIGH() \
	SD_WR_DAT(0x0f)
#define SD_WR_DAT_LOW() \
	SD_WR_DAT(0x00)
#define SD_RD_DAT() \
	( (*SD_CTRL & 0x000000f0) >> 4 )

#define SD_VHS_2V7_3V6				0x01
#define CMD8_DEFAULT_TEST_PATTERN	0xaa

#define SD_SHORT_DELAY()

typedef enum
{
    SD_STATE_IDLE = 0,
    SD_STATE_READY = 1,    
    SD_STATE_IDENT = 2,
    SD_STATE_STBY = 3,
    SD_STATE_TRAN    
} SD_CURRENT_STATE;

static int32_t s_dataBits = 1;

static void sd_dummy_clock(uint32_t clockCnt)
{
	for (uint32_t i = 0; i < clockCnt; ++i)
	{
        SD_WR_CLK_LOW();
		SD_SHORT_DELAY();
        SD_WR_CLK_HIGH();
		SD_SHORT_DELAY();
    }
}

static void sd_send_cmd(uint8_t cmd[6], int32_t cmdLen)
{
	uint8_t mask, data;
	SD_WR_CMD_DIR_OUT();
	for (int32_t i = 0; i < cmdLen; i++)
	{
		mask = 0x80;
		data = cmd[i];
		for (int32_t k = 0; k < 8; k++)
		{
			SD_WR_CLK_LOW();
			if (data & mask)
				{ SD_WR_CMD_HIGH(); }
			else
				{ SD_WR_CMD_LOW(); }
			SD_SHORT_DELAY();
			SD_WR_CLK_HIGH();  
			mask >>= 1;
			SD_SHORT_DELAY();
		}
	}
}

static int32_t sd_get_response(uint8_t* outResponse, int32_t responseLen)
{
	const int32_t maxCnt = 40;

	SD_WR_CMD_LOW();
	SD_WR_CMD_DIR_IN();
	
	for (int32_t cnt = 0;;)
	{
		SD_WR_CLK_LOW();
		SD_SHORT_DELAY();
		SD_WR_CLK_HIGH();
		SD_SHORT_DELAY();
		if (!SD_RD_CMD())
			break;
		else if (cnt++ > maxCnt)
			return 0;
	}
  
	SD_WR_CLK_LOW();
	SD_SHORT_DELAY();
	SD_WR_CLK_HIGH();
	SD_SHORT_DELAY();

	if (SD_RD_CMD())
		return 0;

	int32_t value = 0;
	int32_t bit = 2;
	for (int32_t index = 0; index < responseLen; )
	{
		SD_WR_CLK_LOW();
		SD_SHORT_DELAY();
		SD_WR_CLK_HIGH();
		SD_SHORT_DELAY();
		if (SD_RD_CMD())
			value |= 0x80 >> bit;
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

	sd_dummy_clock(8);
	return 1;	
}

static int32_t sd_cmd0()
{
	uint8_t cmd[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[1];
	uint8_t crc;

	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	sd_dummy_clock(100);
	return 1;
}

static int32_t sd_cmd8(uint8_t voltId, uint8_t testPattern)
{
	const uint8_t c_cmd = 8;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[3] |= (voltId & 0x0f);
	cmd[4] = testPattern;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));
	
	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;
	if (response[3] != voltId)
		return 0;
	if (response[4] != testPattern)
		return 0;
	if (crc7(0, response, 5) != (response[5] >> 1))
		return 0;
	if ((response[5] & 0x01) != 0x01)
		return 0;

	return 1;
}

static int32_t sd_cmd55(uint16_t rca16)
{
	const uint8_t c_cmd = 55;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] = rca16 >> 8;
	cmd[2] = rca16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;

	// \todo check card status

	if (crc7(0, response, 5) != (response[5] >> 1))
		return 0;
	if ((response[5] & 0x01) != 0x01)
		return 0;

	return 1;
}

static int32_t sd_acmd41(uint32_t hostOCR32, uint32_t* outOCR)
{
	const uint8_t c_cmd = 41;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= (hostOCR32 >> 24) & 0x40;
	cmd[2] = (hostOCR32 >> 16) & 0xff;
	cmd[3] = (hostOCR32 >> 8) & 0xff;
	cmd[4] = hostOCR32 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != 0x3f)
		return 0;		

	uint32_t OCR = 0;
	for(int i = 0; i < 4; i++)
	{
		OCR <<= 8;
		OCR |= response[i + 1];
	}

	// Check if card is busy.
	if ((OCR & 0x80000000) != 0x80000000)
		return 0;

	if (response[5] != 0xff)
		return 0;		

	*outOCR = OCR;
	return 1;
}

static int32_t sd_cmd2(uint8_t* cid, int32_t cidLen)
{
	const uint8_t c_cmd = 2;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[17];
	uint8_t crc;

	cmd[0] |= c_cmd;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	// \todo verify response

	int32_t copyLen = cidLen;
	if (copyLen > 16)
		copyLen = 16;

	for (int32_t i = 0; i < copyLen; ++i)
		cid[i] = response[i + 1];

	return 1;
}

static int32_t sd_cmd3(uint16_t* outRCA16)
{
	const uint8_t c_cmd = 3;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	*outRCA16 = (response[1] << 8) | response[2];
	return 1;
}

static int32_t sd_cmd9(uint16_t RCA16, uint8_t* outCSD, int32_t CSDLen)
{
	const uint8_t c_cmd = 9;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[17];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= RCA16 >> 8;
	cmd[2] |= RCA16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != 0x3f)
		return 0;		

	// \todo verify response

	int32_t copyLen = CSDLen;
	if (copyLen > 16)
		copyLen = 16;

	for (int32_t i = 0; i < copyLen; ++i)
		outCSD[i] = response[i + 1];

	return 1;
}

static int32_t sd_cmd7(uint16_t RCA16)
{
	const uint8_t c_cmd = 7;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= RCA16 >> 8;
	cmd[2] |= RCA16 & 0xff;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;		

	return 1;
}

static int32_t sd_cmd16(uint32_t blockLength)
{
	const uint8_t c_cmd = 16;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= blockLength >> 24;
	cmd[2] |= blockLength >> 16;
	cmd[3] |= blockLength >> 8;
	cmd[4] |= blockLength;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;		

	return 1;
}

static int32_t sd_cmd17(uint32_t addr)
{
	const uint8_t c_cmd = 17;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= addr >> 24;
	cmd[2] |= addr >> 16;
	cmd[3] |= addr >> 8;
	cmd[4] |= addr;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	return 1;
}

static int32_t sd_cmd24(uint32_t addr)
{
	const uint8_t c_cmd = 24;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[1] |= addr >> 24;
	cmd[2] |= addr >> 16;
	cmd[3] |= addr >> 8;
	cmd[4] |= addr;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;
	if (crc7(0, response, 5) != (response[5] >> 1))
		return 0;
	if ((response[5] & 0x01) != 0x01)
		return 0;

	return 1;
}

static int32_t sd_acmd6(int32_t bus4)
{
	const uint8_t c_cmd = 6;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[4] |= bus4 ? 2 : 0;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;		

	return 1;
}

static int32_t sd_acmd42(int32_t bus4)
{
	const uint8_t c_cmd = 42;

	uint8_t cmd[6] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t response[6];
	uint8_t crc;

	cmd[0] |= c_cmd;
	cmd[4] |= bus4 ? 2 : 0;
	crc = crc7(0, cmd, 5);
	cmd[5] = (crc << 1) | 0x01;
	sd_send_cmd(cmd, sizeof(cmd));

	if (!sd_get_response(response, sizeof(response)))
		return 0;

	if (response[0] != c_cmd)
		return 0;		

	return 1;
}

int32_t sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen)
{
	uint32_t addr = block;	// SDHC take block number.
	// \todo support non SDHC

	kernel_enter_critical();

	if (!sd_cmd17(addr)) // , SD_STATE_TRAN))
	{
		kernel_leave_critical();
		return 0;
	}

	SD_WR_DAT_DIR_IN();

	// Wait on start bits.
	int32_t try = 0;
	while(1)
	{
        SD_WR_CLK_LOW();
		SD_SHORT_DELAY();
        SD_WR_CLK_HIGH();
		SD_SHORT_DELAY();

		// Check start bits (zero is expected).
		if (s_dataBits == 4)
		{
			if((SD_RD_DAT() & 0x0f) == 0x00) 
				break;
		}
		else if (s_dataBits == 1)
		{
			if((SD_RD_DAT() & 0x01) == 0x00)
				break;
		}

		// Not ready; yield this thread.
		kernel_leave_critical();
		kernel_yield();
		kernel_enter_critical();

		if (try++ > 4000)
		{
			kernel_leave_critical();
			return 0;
		}     
	}

    // Read data (512byte = 1 block)
	if (s_dataBits == 4)
	{
		for(uint32_t i = 0; i < bufferLen; i++)
		{
			uint8_t data8 = 0;
			for(int32_t j = 0; j < 2; j++)
			{
				SD_WR_CLK_LOW();
				SD_SHORT_DELAY();
				SD_WR_CLK_HIGH();
				SD_SHORT_DELAY();
				data8 <<= 4; 
				data8 |= (SD_RD_DAT() & 0x0f);
			}
			buffer[i] = data8;
		}
	}
	else if (s_dataBits == 1)
	{
		for(uint32_t i = 0; i < bufferLen; i++)
		{
			uint8_t data8 = 0;
			for(int32_t j = 0; j < 8; j++)
			{
				SD_WR_CLK_LOW();
				SD_SHORT_DELAY();
				SD_WR_CLK_HIGH();
				SD_SHORT_DELAY();
				data8 <<= 1; 
				data8 |= (SD_RD_DAT() & 0x01);
			}
			buffer[i] = data8;
		}
	}

	kernel_leave_critical();
	return bufferLen;
}

int32_t sd_write_block512(uint32_t block, const uint8_t* buffer, uint32_t bufferLen)
{
	uint32_t addr = block;	// SDHC take block number.
	// \todo support non SDHC

	kernel_enter_critical();

	if (!sd_cmd24(addr))
	{
		kernel_leave_critical();
		return 0;
	}

	SD_WR_DAT_DIR_OUT();
	SD_WR_CLK_LOW();
	SD_WR_DAT(0x00);  
	SD_WR_CLK_HIGH();

	for (uint32_t i = 0; i < bufferLen; i++)
	{
		uint8_t data8 = buffer[i];
		if (s_dataBits == 4)
		{
			for (uint32_t j = 0; j < 2; j++)
			{
				SD_WR_CLK_LOW();
				SD_WR_DAT((data8 >> 4) & 0x0f);
				SD_WR_CLK_HIGH();
				data8 <<= 4; 
			}
		}
		else if (s_dataBits == 1)
		{  
			for (uint32_t j = 0; j < 8; j++)
			{
				SD_WR_CLK_LOW();
				if (data8 & 0x80) {
					SD_WR_DAT_HIGH();
				} else {
					SD_WR_DAT_LOW();
				}
				SD_WR_CLK_HIGH();
				data8 <<= 1; 
			} 
		}
	}

	// Send CRC.
	if (s_dataBits == 4)
	{
		uint8_t p[4][128];
		memset(p, 0, sizeof(p));

		uint32_t jj = 0;
		for (uint32_t i = 0; i < bufferLen; i++)
		{
			uint8_t data8 = buffer[i];
			for (uint32_t j = 0; j < 2; j++)
			{
				const uint8_t nibble = (data8 >> 4) & 0x0f;

				uint8_t k = 7 - (jj & 7);
				p[0][jj >> 3] |= ((nibble & 1) ? 1 : 0) << k;		// DAT0
				p[1][jj >> 3] |= ((nibble & 2) ? 1 : 0) << k;		// DAT1
				p[2][jj >> 3] |= ((nibble & 4) ? 1 : 0) << k;		// DAT2
				p[3][jj >> 3] |= ((nibble & 8) ? 1 : 0) << k;		// DAT3
				++jj;

				data8 <<= 4;
			}
		}

		uint16_t dataCrc16[] =
		{
			crc16(p[0], 128),
			crc16(p[1], 128),
			crc16(p[2], 128),
			crc16(p[3], 128)
		};

		for (uint32_t i = 0; i < 16; i++)
		{
			uint8_t c = 0;
			c |= (dataCrc16[0] & 0x8000) ? 1 : 0;
			c |= (dataCrc16[1] & 0x8000) ? 2 : 0;
			c |= (dataCrc16[2] & 0x8000) ? 4 : 0;
			c |= (dataCrc16[3] & 0x8000) ? 8 : 0;

			SD_WR_CLK_LOW();
			SD_WR_DAT(c);
			SD_WR_CLK_HIGH();

			dataCrc16[0] <<= 1;         
			dataCrc16[1] <<= 1;         
			dataCrc16[2] <<= 1;         
			dataCrc16[3] <<= 1;         
		}

	}
	else if (s_dataBits == 1)
	{
		uint16_t dataCrc16 = crc16(buffer, bufferLen);
		for (uint32_t i = 0; i < 16; i++)
		{
			SD_WR_CLK_LOW();
			if (dataCrc16 & 0x8000) {
				SD_WR_DAT_HIGH();
			} else {
				SD_WR_DAT_LOW();
			}
			SD_WR_CLK_HIGH();
			dataCrc16 <<= 1;         
		}
	}

	// Stop bits (value 'one').
	SD_WR_CLK_LOW();
	if (s_dataBits == 4) {
		SD_WR_DAT(0x0f);
	} else if (s_dataBits == 1) {
		SD_WR_DAT_HIGH();
	}
	SD_WR_CLK_HIGH();

	// Check busy bits (data0 only).
	SD_WR_DAT_DIR_IN();
	uint8_t writeSuccess = 0;    
	for (uint32_t i = 0; i < 32; i++)
	{
		SD_WR_CLK_LOW();
		SD_WR_CLK_HIGH();
		if ((SD_RD_DAT() & 0x01) == 0x01)
		{
			writeSuccess = 1;
			break;
		}
	}
	if (!writeSuccess)
	{
		kernel_leave_critical();
		return 0;
	}

	sd_dummy_clock(100000);

	kernel_leave_critical();
	return bufferLen;
}

int32_t sd_init()
{
	*SD_CTRL = 0x0000ff00;

	// const uint32_t deviceId = sysreg_read(SR_REG_DEVICE_ID);
	// if (deviceId == SR_DEVICE_ID_RV32T || deviceId == SR_DEVICE_ID_RV32)
		s_dataBits = 4;
	// else
	// {
	// 	// \todo Cannot write in 4 bit mode atm, something wrong...
	// 	s_dataBits = 1;
	// }

	SD_WR_CMD_DIR_OUT();
	SD_WR_DAT_DIR_IN();
	SD_WR_CLK_HIGH();
	SD_SHORT_DELAY();
	SD_WR_CMD_HIGH();
	SD_SHORT_DELAY();
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

	for (int32_t count = 0;; ++count)
	{
		if (!sd_cmd55(0x0000))
			return 1;

		if (!sd_acmd41(hostOCR32, &OCR))
		{
			if (count > 100)
				return 1;
			timer_wait_ms(2);
		}
		else
			break;
	}

	// Get card CID.
	uint8_t cid[16];
	sd_cmd2(cid, sizeof(cid));

	// Get card RCA identifier.
	sd_cmd3(&RCA16);

	// Enter data transfer mode, standby state.
	uint8_t csd[17];
	sd_cmd9(RCA16, csd, sizeof(csd));

	// cmd10?

	// Enter transfer state.
	sd_cmd7(RCA16);

	// Select block length.
	sd_cmd16(512);

	// If 4 bit, set bus width.
	if (s_dataBits == 4)
	{
		sd_cmd55(RCA16);
		sd_acmd6(1);
		sd_cmd55(RCA16);
		sd_acmd42(1);
	}
	return 0;
}
