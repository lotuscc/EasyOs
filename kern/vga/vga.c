#include "vga.h"

char *addr = (char *)0x0B8000;

int CurPosX = 0;
int CurPosY = 0;
int CurPos = 0;

struct VGA_Color_Struct VGA_Color = {0x0c};

char NumtoChar_Map[16] = {'0','1','2','3','4','5','6','7','8','9',\
                          'A','B','C','D','E','F'};

struct NumBufferType{
    char NumBuffer[64];
    int Size;
};

int XYtoPos(int PosX, int PosY){
    return PosX * 80 + PosY;
}


int EasyOS_PutChar(char ch, int pos){      

    *(addr + pos * 2) = ch;
    *(addr + pos * 2 + 1 ) = VGA_Color.Red;
}

int EasyOS_ClearScreen(){
    for (int i = 0; i < 25 ; i++){
        for (int j = 0; j < 80; j++){
            EasyOS_PutChar_XY(' ', i, j);
        }
    }
    return 0;
}

int EasyOS_FillScreen(char ch){
    for (int i = 0; i < 25 ; i++){
        for (int j = 0; j < 80; j++){
            EasyOS_PutChar_XY(ch, i, j);
        }
    }
    return 0;
}


int EasyOS_PutChar_XY(char ch, int PosX, int PosY){  
    if(PosX > VGA_X_Max || PosY > VGA_Y_Max)
        return -1;
   
    *(addr + (PosX * 80 + PosY) * 2) = ch;
    *(addr + (PosX * 80 + PosY) * 2 + 1 ) = VGA_Color.Red;

    return 0;
}

int EasyOS_PutStr(char* str, int PosX, int PosY){

    int pos = PosX * 80 + PosY;

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

int EasyOS_PutIntX(uint32_t data, int PosX, int PosY){
    
    struct NumBufferType NumBuffer;
    NumBuffer.Size = 0;
    char ch = 0;

    do{
        ch = data % 16;
        NumBuffer.NumBuffer[NumBuffer.Size++] = NumtoChar_Map[ch];
        data /= 16; 

    }while (data);

    int Pos = XYtoPos(PosX, PosY);

    while (NumBuffer.Size){
        EasyOS_PutChar(NumBuffer.NumBuffer[--NumBuffer.Size], Pos++);
    }
    
    return 0;
}

int EasyOS_PutIntD(uint32_t data, int PosX, int PosY){
    
    struct NumBufferType NumBuffer;
    NumBuffer.Size = 0;
    char ch = 0;

    do{
        ch = data % 10;
        NumBuffer.NumBuffer[NumBuffer.Size++] = NumtoChar_Map[ch];
        data /= 10; 

    }while (data);

    int Pos = XYtoPos(PosX, PosY);

    while (NumBuffer.Size){
        EasyOS_PutChar(NumBuffer.NumBuffer[--NumBuffer.Size], Pos++);
    }
    
    return 0;
}