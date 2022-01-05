#pragma once

#include "Runtime/HAL/Common.h"

int32_t file_init();

int32_t file_open(const char* name);

void file_close(int32_t fd);

int32_t file_seek(int32_t file, int32_t offset, int32_t from);

int32_t file_write(int32_t file, const uint8_t* ptr, int32_t len);

int32_t file_read(int32_t file, uint8_t* ptr, int32_t len);