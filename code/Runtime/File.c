#include <ff.h>
#include <diskio.h>

#include "File.h"
#include "Print.h"
#include "SD.h"

// 

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

//

FATFS fs;

int32_t file_init()
{

    f_mount(&fs, "", 0);

    FIL fp;
    f_open(&fp, "readme.txt", FA_READ);


    uint8_t buf[64];
    UINT br;
    f_read(&fp, buf, sizeof(buf), &br);
    print("Read "); printHex(br); printLn(" bytes!");
    buf[63] = 0;
    printLn(buf);


    f_close(&fp);


    return 1;
}

int32_t file_open(const char* name)
{
    return 0;
}

void file_close(int32_t fd)
{
}