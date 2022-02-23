#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C int32_t file_init();

EXTERN_C int32_t file_open(const char* name);

EXTERN_C void file_close(int32_t fd);

EXTERN_C int32_t file_size(int32_t fd);

EXTERN_C int32_t file_seek(int32_t file, int32_t offset, int32_t from);

EXTERN_C int32_t file_write(int32_t file, const uint8_t* ptr, int32_t len);

EXTERN_C int32_t file_read(int32_t file, uint8_t* ptr, int32_t len);
