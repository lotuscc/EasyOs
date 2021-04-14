#include "vga.h"

#define VGA_X_Max 25
#define VGA_Y_Max 80


#define CRT_AddrReg 0x03D4
#define CRT_DataReg 0x03D5

#define index_CurHighReg 0x0E
#define index_CurLowReg  0x0F


uint16_t* videoAddr = (uint16_t*)0x0B8000;

// 将对应的数字转换字符
const char NumtoChar_Map[16] = {'0','1','2','3','4','5','6','7','8','9',\
                          'A','B','C','D','E','F'};
// 黑底白字
const uint8_t defaultColor = 0x07;



// 得到光标位置
static uint32_t getCursorSite(void){
    // Extract cursor location
    uint32_t pos = 0;
    
    // 高8位
    outb(CRT_AddrReg, index_CurHighReg);
    pos = inb(CRT_DataReg) << 8;
    
    // 低8位
    outb(CRT_AddrReg, index_CurLowReg);
    pos |= inb(CRT_DataReg);

    return pos;
}

// 设置光标位置
static void setCursorSite(uint32_t pos){

    uint8_t low = pos & 0xFF;
    uint8_t high = (pos & 0xFF00) >> 8;
    // 高8位
    outb(CRT_AddrReg, index_CurHighReg);
    outb(CRT_DataReg, high);

    // 低8位
    outb(CRT_AddrReg, index_CurLowReg);
    outb(CRT_DataReg, low);
}

// 将前面n-1行向上移动一行
// 同时最后一行填为空白
static void scrollScreen(void){
    
}

// 不考虑不可打印字符,只考虑可打印字符
static void base_putchar(char ch, int pos){
    
    if(pos >= 80 * 25){   
        // scrollScreen();
        // pos -= pos % 80;
        // pos -= 80;

        pos = 0;
    }
    

    uint16_t t = (ch & 0xFF) | ( defaultColor << 8);
    *(videoAddr + pos) = t;
    ++pos;  
}


// 默认初始化为80*25文本模式
static void default_init(void){

}

// 默认刷新函数
static void default_refresh(void){

}

static void default_putChar(char ch){
    
    uint32_t pos = getCursorSite();

    if(ch == '\n' || ch == '\r'){
        pos -= pos % 80;
        pos += 80;
    }else if (ch == '\b'){
        --pos;
    }else{
        base_putchar(ch, pos);
        ++pos;
    }

    setCursorSite(pos);
}

static void default_putStr(const char* str){

    uint32_t pos = getCursorSite();

    int i = 0;
    while(str[i] != '\0'){
        if(str[i] == '\n' || str[i] == '\r'){
            pos -= pos % 80;
            pos += 80;
        }else if (str[i] == '\b'){
            --pos;
        }else{
            base_putchar(str[i], pos);
            ++pos;
        }
        ++i;
    }

    setCursorSite(pos);
}

// 清屏
static void default_clearScreen(void){
    setCursorSite(0);
    for (int i = 0; i < 80*25; i++){
        base_putchar(' ', i);
    }
}

// 用一个字符填满屏幕
static void default_fillScreen(char ch){
    setCursorSite(0);
    for (int i = 0; i < 80*25; i++){
        base_putchar(ch, i);
    }
}
    
// 以十六进制打印数字,前面带0x
static void default_putInt(int num){
    uint32_t mask = 0xF0000000;
    
    default_putStr("0x");
    for (int i = 0; i < 8; i++){
        char ch = (num & mask) >> ((7 - i) * 4);

        if(ch >= 10){
            default_putChar(ch - 10 + 'A');
        }else{
            default_putChar(ch + '0');
        }

        mask >>= 4;
    }
}


const struct VgaDeviceStruct vgadevice = {
    .name = "default_vga_device",
    .init = default_init,
    .refresh = default_refresh,
    .putChar = default_putChar,
    .putStr = default_putStr,
    .putInt = default_putInt,
    .clearScreen = default_clearScreen,
    .fillScreen = default_fillScreen,
};

struct VgaDeviceStruct* vga = &vgadevice;