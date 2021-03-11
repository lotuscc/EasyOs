#ifndef __VGA_H__
#define __VGA_H__

void EasyOS_PutChar(char ch, int pos);
int EasyOS_ClearScreen();
int EasyOS_PutChar_XY(char ch, int posx, int posy);
int EasyOS_PutStr(char* str, int posx, int posy);
#endif /* !__VGA_H__ */



