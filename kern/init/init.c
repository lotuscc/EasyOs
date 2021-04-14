#include "vga.h"
#include "vmm.h"
#include "pic.h"
#include "timer.h"
#include "thread.h"


void dowhile(){
        /* do nothing */
    while (1);
}

void hello1(void* arg);
void hello2(void* arg);

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
    

    eos_proc_init();

    eos_proc_start("th1", 20, hello1, 0);
    eos_proc_start("th1", 20, hello1, 0);

    asm volatile("sti");	     // 为演示中断处理,在此临时开中断

    dowhile();
}



void hello1(void* arg){
    while(1){
        vga->putStr("1111");
    }
}

void hello2(void* arg){
    while(2){
        vga->putStr("2222");
    }
}