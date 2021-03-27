#ifndef __VGA_H__
#define __VGA_H__

struct VGA_Color_Struct
{
    char Red; // 0x0c

};

#define VGA_X_Max 80
#define VGA_Y_Max 25


void EasyOS_PutChar(char ch, int pos);
int EasyOS_ClearScreen();
int EasyOS_PutChar_XY(char ch, int posx, int posy);
int EasyOS_PutStr(char* str, int posx, int posy);


#endif /* !__VGA_H__ */



