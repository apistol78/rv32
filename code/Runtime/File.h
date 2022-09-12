#pragma once

#include "Runtime/HAL/Common.h"

#define FILE_MODE_READ  0
#define FILE_MODE_WRITE 1

typedef void (*fn_enum_t)(void* user, const char* filename, uint32_t size, uint8_t directory);

EXTERN_C int32_t file_init();

EXTERN_C int32_t file_open(const char* name, int32_t mode);

EXTERN_C void file_close(int32_t fd);

EXTERN_C int32_t file_size(int32_t fd);

EXTERN_C int32_t file_seek(int32_t file, int32_t offset, int32_t from);

EXTERN_C int32_t file_write(int32_t file, const uint8_t* ptr, int32_t len);

EXTERN_C int32_t file_read(int32_t file, uint8_t* ptr, int32_t len);

EXTERN_C int32_t file_enumerate(const char* path, void* user, fn_enum_t fnen);

EXTERN_C int32_t file_remove(const char* filename);