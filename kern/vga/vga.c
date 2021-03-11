#include "vga.h"

struct VGA_Color_Struct
{
    char Red; // 0x0c

};

#define VGA_X_Max 80
#define VGA_Y_Max 25


char *addr = (int *)0x0B8000;

int CurPos = 0;

struct VGA_Color_Struct VGA_Color = {0x0c};

void EasyOS_PutChar(char ch, int pos){  

    *(addr + pos * 2) = ch;
    *(addr + pos * 2 + 1 ) = VGA_Color.Red;

}

int EasyOS_ClearScreen(){
    for (int i = 0; i < 80 ; i++){
        for (int j = 0; j < 25; j++){
            EasyOS_PutChar_XY(' ', i, j);
        }
    }
    return 0;
}

int EasyOS_PutChar_XY(char ch, int posx, int posy){  
    if(posx > VGA_X_Max || posy > VGA_Y_Max)
        return -1;
   
    *(addr + (posy * 80 + posx) * 2) = ch;
    *(addr + (posy * 80 + posx) * 2 + 1 ) = VGA_Color.Red;

    return 0;
}

int EasyOS_PutStr(char* str, int posx, int posy){
    int pos = posy * 80 + posx;

    while (*str != '\0'){

        if(*str == '\n'){
            int x = pos / 80;
            pos = 80 * (x + 1);
            str++;

            continue;
        }

        EasyOS_PutChar( *(str++),pos++);
    }

    return 0;
}