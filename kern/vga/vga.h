#ifndef __VGA_H__
#define __VGA_H__

#include <elf.h>
struct VGA_Color_Struct
{
    char Red; // 0x0c

};

// 80 * 25
//////////////////////////////////////////////////  PosY
//P                                              //
//O                                             //
//S                                             //
//X                                             //
//////////////////////////////////////////////////



#define VGA_X_Max 25
#define VGA_Y_Max 80


int EasyOS_PutChar(char ch, int pos);
int EasyOS_ClearScreen();
int EasyOS_FillScreen(char ch);

int EasyOS_PutChar_XY(char ch, int posx, int posy);
int EasyOS_PutStr(char* str, int posx, int posy);
int EasyOS_PutIntX(uint32_t data, int PosX, int PosY);
int EasyOS_PutIntD(uint32_t data, int PosX, int PosY);

#endif /* !__VGA_H__ */



