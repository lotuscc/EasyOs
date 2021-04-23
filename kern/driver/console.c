#include "console.h"
#include "sem.h"
#include "vga.h"

static struct lock console_lock;    // 控制台锁

// 初始化终端
void eos_console_init() {
    lock_init(&console_lock); 
}

// 获取终端
static void eos_console_acquire() {
    lock_acquire(&console_lock);
}

// 释放终端
static void eos_console_release() {
    lock_release(&console_lock);
}

// 终端中输出字符串
void eos_console_putStr(char* str) {
    eos_console_acquire(); 
    vga->putStr(str);
    eos_console_release();
}

// 终端中输出字符
void eos_console_putChar(uint8_t ch) {
    eos_console_acquire(); 
    vga->putChar(ch); 
    eos_console_release();
}

// 终端中输出16进制整数
void eos_console_putInt(uint32_t num) {
    eos_console_acquire(); 
    vga->putInt(num);
    eos_console_release();
}
