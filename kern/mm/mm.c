
#include "mm.h"
#include "vga.h"
#include "elf.h"
#include "defs.h"
#include "list.h"




struct PageType{
    int count;
    uint32_t property;
    uint32_t begin_addr;
    uint32_t end_addr;
    struct list_entry_t list_entry;
};



#define ARDS_ADDRESS 0x8000                     // ARDS结构的物理地址
#define PGSIZE          4096                    // bytes mapped by a page


#define Page_Property_Allow  0
#define Page_Property_Ban    1

// 64位运算会出错


// 检测物理内存的大小，最大只能检测出4Gb
uint32_t CalMemSize(){
    struct e820map *memmap = (struct e820map *)(ARDS_ADDRESS);
    
    uint32_t TolMemSize = 0;

    uint32_t AddBegin = 0;
    uint32_t MemSize = 0;

    for (int i = 0; i < memmap->n_map; i ++) {
        AddBegin = memmap->map[i].addrLow;
        MemSize = memmap->map[i].sizeLow;
        
        MemSize = MemSize + AddBegin; 

        if (TolMemSize < MemSize) {
            TolMemSize = MemSize;
        }

        if(memmap->map[i].type == E820_ARM){
            EasyOS_PutStr("Yes\n", 0, 0);
        }else{
            EasyOS_PutStr("No\n", 0, 0);
        }
    }
    return TolMemSize;
}

uint32_t PageSizeRoundUp(uint32_t addr){
    uint32_t n_Size = addr / PGSIZE;
    if(addr % PGSIZE == 0){
        return addr;
    }

    return (n_Size + 1) * PGSIZE;
}

uint32_t PageSizeRoundDown(uint32_t addr){
    uint32_t n_Size = addr / PGSIZE;
    if(addr % PGSIZE == 0){
        return addr;
    }

    return n_Size * PGSIZE;
}


/* pmm_init - initialize the physical memory management */
void Page_Init(void) {

    uint32_t size = CalMemSize();

    int N_Page = size / PGSIZE;

    extern char end[];


    // 从内核结束末地址开始
    struct PageType* pageHead = (struct PageType*)(end);

    // 依次将 N_Page 个Page的count置零
    for (int i = 0; i < N_Page; i++){
        struct PageType* pageIt = pageHead + i;
        pageIt->count = 0;
    }

    struct PageType* pageIt = pageHead + 1;

    struct e820map *memmap = (struct e820map *)(ARDS_ADDRESS);
    
    uint32_t AddBegin = 0;
    uint32_t MemSize = 0;

    for (int i = 0; i < memmap->n_map; i ++) {
        AddBegin = memmap->map[i].addrLow;
        MemSize = memmap->map[i].sizeLow;
        
        MemSize = MemSize + AddBegin; 


        if(memmap->map[i].type == E820_ARM){
            
            AddBegin = PageSizeRoundUp(AddBegin);                   
            MemSize = PageSizeRoundDown(MemSize);

            while(AddBegin < MemSize){
                
                pageIt->begin_addr = AddBegin;
                pageIt->end_addr = AddBegin + PGSIZE;
                pageIt->count = 0;
                
                pageIt->property = Page_Property_Allow;

                AddBegin += PGSIZE;
                // 链接到链表上
                list_add(pageIt-1, pageIt);
                
                // 指向下一个节点
                pageIt = pageIt + 1;
            } 
        }else{

            AddBegin = PageSizeRoundDown(AddBegin);                   
            MemSize = PageSizeRoundUp(MemSize);

            while(AddBegin < MemSize){
                
                pageIt->begin_addr = AddBegin;
                pageIt->end_addr = AddBegin + PGSIZE;
                pageIt->count = 0;
                
                pageIt->property = Page_Property_Ban;

                AddBegin += PGSIZE;
                // 链接到链表上
                list_add(pageIt-1, pageIt);
                
                // 指向下一个节点
                pageIt = pageIt + 1;

            } 
        }
        


    }

}
















