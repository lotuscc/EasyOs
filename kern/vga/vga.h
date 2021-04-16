#ifndef __VGA_H__
#define __VGA_H__

#include <elf.h>
#include "x86.h"

// 80 * 25
//////////////////////////////////////////////////  PosY
//P                                              //
//O                                             //
//S                                             //
//X                                             //
//////////////////////////////////////////////////

// 字符缓冲区，在80*25模式下
struct VgaDeviceStruct{
    const char *name;   
    void (*init)(void);
    void (*refresh)(void);
    void (*putChar)(char ch);
    void (*putStr)(const char* str);
    void (*putInt)(int num);
    void (*clearScreen)(void);
    void (*fillScreen)(char ch);
};

extern const struct VgaDeviceStruct vgadevice;
extern struct VgaDeviceStruct* vga;

#endif /* !__VGA_H__ */



