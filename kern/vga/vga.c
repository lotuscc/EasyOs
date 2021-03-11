#include "vga.h"

struct VGA_Color_S
{
    char Red;        
};

struct VGA_Color_S VGA_Color = {0x0c};

void EasyOS_PutChar(char ch, int pos){

    char *addr = (int *)0x0B8000;

    *(addr + pos * 2) = ch;
    *(addr + pos * 2 + 1 ) = VGA_Color.Red;
}