#include "vmm.h"
#include "memory.h"

struct PageItem{
    unsigned int addrAttributes;
};


#define DEFINE_SetPageItem(PageItemIt, addr, attributes) {               \
        (PageItemIt)->addrAttributes = (addr)&(0xFFFFF000) | (attributes)&(0xFFF);        \
    }


// 页目录表首地址,物理地址,此时还未开启分页机制
// #define PageCataTableBase 0x00010000



static void vmmPageInit(void){
    struct PageItem* PageSetIt = (struct PageItem*)(PageCataTableBase);
    // // 0x0010 1000

    // 设置2个页目录项
    
    // 第一个页目录项
    // 将第1个4MB地址(0-4MB) 映射到第1个页表
    DEFINE_SetPageItem(PageSetIt+0, PageCataTableBase + 0x1000, 0x7);
    
    // 3GB以上
    // 将第x个4MB地址(3GB+0 - 3GB+4MB) 映射到第1个页表
    DEFINE_SetPageItem(PageSetIt+0x300, PageCataTableBase + 0x1000, 0x7);

    // 自映射,将最后一个目录项设置为自己,用于更新页目录项
    DEFINE_SetPageItem(PageSetIt+0x3FF, PageCataTableBase, 0x7);

    // 设置第一个页表
    // 映射到 物理空间0-4MB 
    PageSetIt = (struct PageItem*)(PageCataTableBase + 0x1000);
    // 设置1024个页表项
    for (int i = 0; i < 1024; i++){
        DEFINE_SetPageItem(PageSetIt+i, 0x00000000+i*0x1000, 0x7);
    }

}

static void vmmOpenPage(void){
    
    // 把页目录地址赋给cr3
    lcr3(PageCataTableBase);

    // 打开cr0的pg位(第31位)
    unsigned int cronum = rcr0();
    cronum |= 0x80000000;
    lcr0(cronum);
}


void vmm_init(void){

    // 设置页目录表以及页表
    vmmPageInit();
    
    // 开启分页
    vmmOpenPage();
}
