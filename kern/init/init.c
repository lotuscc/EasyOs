#include "vga.h"
#include "vmm.h"
#include "pic.h"
#include "timer.h"
#include "thread.h"
#include "console.h"

void dowhile(){
        /* do nothing */
    while (1){
        // vga->putStr("main ");
    }
}

int hello1(void* arg);
int hello2(void* arg);

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
    
    // 物理内存初始化
    mem_init();

    // 虚拟内存初始化
    vmm_init();
    
    // 中断控制器初始化
    pic_init();

    //中断向量以及中断处理函数初始化 
    idt_init();

    // 时钟初始化
    timer_init();

    // 终端初始化
    // eos_console_init();

    // eos_proc_init();

    // eos_proc_start("th1", 2, hello1, 0x22);
    // eos_proc_start("th2", 2, hello2, 0x55);

    // debug_all_list();
    
    // debug_ready_list();

    keyboard_init();
    
    asm volatile("sti");	     // 为演示中断处理,在此临时开中断

    dowhile();
}

int hello1(void* arg){
    asm volatile("sti");

    for(int i = 0; i < 4; ++i){
        vga->putStr("\nthread1 ");
        vga->putInt(arg);        
    }
    // while(1){
    //     vga->putStr("thread1 ");
    //     vga->putInt(arg);
    // }

    return 0x111;
}

int hello2(void* arg){
    asm volatile("sti");

    for(int i = 0; i < 4; ++i){
        vga->putStr("\nthread2 ");
        vga->putInt(arg);        
    }
    // while(2){
    //     vga->putStr("thread2 ");
    //     vga->putInt(arg);
    // }

    return 0x222;
}