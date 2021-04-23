#ifndef __console_H__
#define __console_H__

#include "elf.h"

void eos_console_init(void);
static void eos_console_acquire(void);
static void eos_console_release(void);

void eos_console_putStr(char* str);
void eos_console_putChar(uint8_t ch);
void eos_console_putInt(uint32_t num);


#endif