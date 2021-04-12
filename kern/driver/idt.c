#include "idt.h"
#include "vga.h"
#include "timer.h"

/* Gate descriptors for interrupts and traps */
struct gatedesc {
    unsigned gd_off_15_0 : 16;      // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;           // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;           // reserved(should be zero I guess)
    unsigned gd_type : 4;           // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;              // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;              // Present
    unsigned gd_off_31_16 : 16;     // high bits of offset in segment
};

/* *
 * Set up a normal interrupt/trap gate descriptor
 *   - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate
 *   - sel: Code segment selector for interrupt/trap handler
 *   - off: Offset in code segment for interrupt/trap handler
 *   - dpl: Descriptor Privilege Level - the privilege level required
 *          for software to invoke this interrupt/trap gate explicitly
 *          using an int instruction.
 * */
#define SETGATE(gate, istrap, sel, off, dpl) {               \
        (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;      \
        (gate).gd_ss = (sel);                                \
        (gate).gd_args = 0;                                 \
        (gate).gd_rsv1 = 0;                                 \
        (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
        (gate).gd_s = 0;                                    \
        (gate).gd_dpl = (dpl);                              \
        (gate).gd_p = 1;                                    \
        (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
    }

/* Set up a call gate descriptor */
#define SETCALLGATE(gate, ss, off, dpl) {                   \
        (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;      \
        (gate).gd_ss = (ss);                                \
        (gate).gd_args = 0;                                 \
        (gate).gd_rsv1 = 0;                                 \
        (gate).gd_type = STS_CG32;                          \
        (gate).gd_s = 0;                                    \
        (gate).gd_dpl = (dpl);                              \
        (gate).gd_p = 1;                                    \
        (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
    }



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
        break;

        case T_SYSCALL:    // system call 系统调用
        /* do nothing */
        break;
        
        case IRQ_OFFSET + IRQ_TIMER: // IRQ0 时钟中断
        /* do nothing */
        ticks++;
        vga->putInt(tf->tf_trapno);
        vga->putStr(" timer trap! ");
        vga->putInt(ticks);
        vga->putChar(' ');

        break;
        
        case IRQ_OFFSET + IRQ_KBD:   // IRQ1 键盘 
        /* do nothing */
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
