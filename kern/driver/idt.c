#include "idt.h"
#include "vga.h"
#include "timer.h"
#include "thread.h"
#include "keyboard.h"




/* *
 * Interrupt descriptor table:
 *
 * Must be built at run time because shifted function addresses can't
 * be represented in relocation records.
 * */
static struct gatedesc idt[256] = {{0}};

static struct pseudodesc idt_pd = {
    sizeof(idt) - 1, (uintptr_t)idt
};


/*完成有关中断的所有初始化工作*/
void idt_init() {

   extern uintptr_t __vectors[];

   // 初始化为中断门描述符
   for (int i = 0; i < sizeof(idt) / sizeof(struct gatedesc); i ++) {
      SETGATE(idt[i], 0, GD_KTEXT, __vectors[i], DPL_KERNEL);
   }

   // 将T_SYSCALL(0x80)初始化为陷阱门
   SETGATE(idt[T_SYSCALL], 1, GD_KTEXT, __vectors[T_SYSCALL], DPL_USER);
   
   // 加载GDTR
   lidt(&idt_pd);

}


// 根据中断向量号分发中断服务函数
static void trap_dispatch(struct trapframe *tf) {

    switch (tf->tf_trapno) {
        case T_PGFLT:  //page fault
        /* do nothing */
            vga->putStr("Pagr error! \n");
        break;

        case T_SYSCALL:    // system call 系统调用
        /* do nothing */
        break;
        
        case IRQ_OFFSET + IRQ_TIMER: // IRQ0 时钟中断
        /* do nothing */
            ticks++;
            // vga->putInt(tf->tf_trapno);
            // vga->putStr(" timer trap! ");
            // vga->putInt(ticks);
            // vga->putChar(' ');

            struct proc_struct* cur = eos_running_proc();
            if (cur->ticks-- == 0){
                cur->tf = tf;
                eos_schedule();
            }
            
        break;
        
        case IRQ_OFFSET + IRQ_KBD:   // IRQ1 键盘 
            /* do nothing */
            // 
            intr_keyboard_handler();
        break;
        
        case IRQ_OFFSET + IRQ_COM1:  // IRQ4 串口1
            /* do nothing */
        break;

        case IRQ_OFFSET + IRQ_IDE1:
        case IRQ_OFFSET + IRQ_IDE2:
            /* do nothing */
        break;

        //LAB1 CHALLENGE 1 : YOUR CODE you should modify below codes.
        case T_SWITCH_TOU:
        case T_SWITCH_TOK:
            /* do nothing */
        break;

        default:
            vga->putStr("default trap! \n");
         
    }
}

void trap(struct trapframe *tf){

   trap_dispatch(tf);

}
