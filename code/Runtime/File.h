#pragma once

#include "Common.h"

int32_t file_init();

int32_t file_open(const char* name);

void file_close(int32_t fd);