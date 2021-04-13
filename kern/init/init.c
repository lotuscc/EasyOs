#include "vga.h"
#include "vmm.h"
#include "pic.h"
#include "timer.h"

void dowhile(){
        /* do nothing */
    while (1);
}

int kern_init(void) {

    

    extern char etext[], edata[], end[];

    vga->clearScreen();

    vga->putStr("etext: ");
    vga->putInt(etext);
    vga->putChar('\n');

    vga->putStr("edata: ");
    vga->putInt(edata);
    vga->putChar('\n');

    vga->putStr("edata: ");
    vga->putInt(end);
    vga->putChar('\n');

    vga->putStr("Page init now\n");
    
    
    
    vmm_init();
    
    // 中断控制器初始化
    pic_init();

    //中断向量以及中断处理函数初始化 
    idt_init();

    // 时钟初始化
    timer_init();
    
    asm volatile("sti");	     // 为演示中断处理,在此临时开中断

    dowhile();
}

