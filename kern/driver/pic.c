#include "pic.h"
#include "vga.h"

// 这里用的可编程中断控制器是8259A
#define PIC_M_CTRL 0x20	       // 主片的控制端口是0x20
#define PIC_M_DATA 0x21	       // 主片的数据端口是0x21

#define PIC_S_CTRL 0xa0	       // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1	       // 从片的数据端口是0xa1


// M 主8259A芯片端口
// S 从8259A芯片端口
#define M_8259A_ICW1 PIC_M_CTRL   // 
#define M_8259A_ICW2 PIC_M_DATA
#define M_8259A_ICW3 PIC_M_DATA
#define M_8259A_ICW4 PIC_M_DATA

#define M_8259A_OCW1 PIC_M_DATA
#define M_8259A_OCW2 PIC_M_CTRL
#define M_8259A_OCW3 PIC_M_CTRL


#define S_8259A_ICW1 PIC_S_CTRL
#define S_8259A_ICW2 PIC_S_DATA
#define S_8259A_ICW3 PIC_S_DATA
#define S_8259A_ICW4 PIC_S_DATA

#define S_8259A_OCW1 PIC_S_DATA
#define S_8259A_OCW2 PIC_S_CTRL
#define S_8259A_OCW3 PIC_S_CTRL

static void pic_setmask(uint16_t mask) {
    outb(M_8259A_OCW1, mask);
    outb(S_8259A_OCW1, mask >> 8);
}

/* 初始化可编程中断控制器8259A */
void pic_init(void) {

    /* 初始化主片 */
    outb (M_8259A_ICW1, 0x11);   // ICW1: 边沿触发,级联8259, 需要ICW4.
    outb (M_8259A_ICW2, 0x20);   // ICW2: 起始中断向量号为0x20,也就是IR[0-7] 为 0x20 ~ 0x27.
    outb (M_8259A_ICW3, 0x04);   // ICW3: IR2接从片. 
    outb (M_8259A_ICW4, 0x03);   // ICW4: x86模式, 自动EOI

    /* 初始化从片 */
    outb (S_8259A_ICW1, 0x11);    // ICW1: 边沿触发,级联8259, 需要ICW4.
    outb (S_8259A_ICW2, 0x28);    // ICW2: 起始中断向量号为0x28,也就是IR[8-15] 为 0x28 ~ 0x2F.
    outb (S_8259A_ICW3, 0x02);    // ICW3: 设置从片连接到主片的IR2引脚
    outb (S_8259A_ICW4, 0x03);    // ICW4: x86模式, 自动EOI

    /* 打开主片上IR0,也就是目前只接受时钟产生的中断 */
    // pic_setmask(0xfffe);
    pic_setmask(0xfffD);
}


