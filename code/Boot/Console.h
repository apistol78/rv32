#pragma once

void fb_init();

void fb_shutdown();

void fb_clear();

void fb_putc(char c);

void fb_print(const char* str);

void fb_printf(const char* str, ...);
