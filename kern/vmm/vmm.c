#include "vmm.h"
#include "x86.h"

#define DEFINE_SetPageItem(PageItemIt, addr, attributes) {               \
        (PageItemIt)->addrAttributes = (addr)&(0xFFFFF000) | (attributes)&(0xFFF);        \
    }

    
void Settest(void){
    struct PageItem* PageSetIt = (struct PageItem*)(0x00100000);
    // // 0x0010 1000

    // 设置4个页目录项
    
    // 第一个页目录项
    // 将第1个4MB地址(0-4MB) 映射到第1个页表
    DEFINE_SetPageItem(PageSetIt+0, 0x00101000, 0x7);
    
    // 将第2个4MB地址(4-8MB) 映射到第2个页表
    DEFINE_SetPageItem(PageSetIt+1, 0x00102000, 0x7);
    
    // 将第x个4MB地址(3GB+0 - 3GB+4MB) 映射到第1个页表
    DEFINE_SetPageItem(PageSetIt+0x300, 0x00101000, 0x7);

    // 将第x+1个4MB地址((3GB+4 - 3GB+8MB)) 映射到第2个页表
    DEFINE_SetPageItem(PageSetIt+0x301, 0x00102000, 0x7);

    // 设置第一个页表
    // 映射到 物理空间0-4MB 
    PageSetIt = (struct PageItem*)(0x00101000);
    // 设置1024个页表项
    for (int i = 0; i < 1024; i++){
        DEFINE_SetPageItem(PageSetIt+i, 0x00000000+i*0x1000, 0x7);
    }

    // 设置第二个页表
    // 映射到 物理空间4-8MB 
    PageSetIt = (struct PageItem*)(0x00102000);
    // 设置1024个页表项
    for (int i = 0; i < 1024; i++){
        DEFINE_SetPageItem(PageSetIt+i, 0x00400000+i*0x1000, 0x7);
    }
}

void OpenPageing(void){
    
    // 把页目录地址赋给cr3
    lcr3(0x00100000);

    // 打开cr0的pg位(第31位)
    unsigned int cronum = rcr0();
    cronum |= 0x80000000;
    lcr0(cronum);
}

