#include <ff.h>
#include <diskio.h>

#include "Runtime/HAL/File.h"
#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/SD.h"

// FatFs hooks

DSTATUS disk_initialize (BYTE pdrv)
{
    printLn("disk_initialize");
    return 0;
}

DSTATUS disk_status (BYTE pdrv)
{
    printLn("disk_status");
    return 0;
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
    printLn("disk_read");
    sd_read_block512(sector, buff, 512);
    return RES_OK;
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
    printLn("disk_write");
    return RES_OK;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    printLn("disk_ioctl");
    return RES_OK;
}

// HAL

FATFS fs;
FIL fp;

int32_t file_init()
{
    f_mount(&fs, "", 0);
    return 1;
}

int32_t file_open(const char* name)
{
    if (f_open(&fp, name, FA_READ) == FR_OK)
        return 1;
    else
        return 0;
}

void file_close(int32_t fd)
{
    f_close(&fp);
}

int32_t file_seek(int32_t file, int32_t offset, int32_t from)
{
    FRESULT result = FR_INVALID_PARAMETER;
    
    if (from == 0)
        result = f_lseek(&fp, offset);
    else if (from == 1)
        result = f_lseek(&fp, f_tell(&fp) + offset);
    else if (from == 2)
        result = f_lseek(&fp, f_size(&fp) + offset);

    if (result != FR_OK)
        return -1;

    return f_tell(&fp);

	// if (__file_ptr == 0)
	// 	return -1;

	// if (dir == 0)
	// 	__file_ptr = (char*)0x00700000 + ptr;
	// else if (dir == 1)
	// 	__file_ptr += ptr;
	// else if (dir == 2)
	// 	__file_ptr = (char*)0x00700000 + __file_size + ptr;

	// return (int)(__file_ptr - (char*)0x00700000);
}

int32_t file_write(int32_t file, const uint8_t* ptr, int32_t len)
{
    UINT bw;
    if (f_write(&fp, ptr, len, &bw) == FR_OK)
        return (int32_t)bw;
    else
        return -1;
}

int32_t file_read(int32_t file, uint8_t* ptr, int32_t len)
{
    UINT br;
    if (f_read(&fp, ptr, len, &br) == FR_OK)
        return (int32_t)br;
    else
        return -1;
}
