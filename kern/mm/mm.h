#ifndef __MM_H__
#define __MM_H__
#include <elf.h>

/* All physical memory mapped at this address */
#define KERNBASE            0xC0000000
#define KMEMSIZE            0x38000000                  // the maximum amount of physical memory
#define KERNTOP             (KERNBASE + KMEMSIZE)


// some constants for bios interrupt 15h AX = 0xE820
#define E820MAX             20      // number of entries in E820MAP
#define E820_ARM            1       // address range memory
#define E820_ARR            2       // address range reserved

struct e820map {
    int n_map;
    struct {
        uint32_t addrLow;
        uint32_t addrHigh;
        uint32_t sizeLow;
        uint32_t sizeHigh;
        uint32_t type;
    } __attribute__((packed)) map[E820MAX];
};


void Page_Init(void);
uint32_t CalMemSize(void);


#endif /* !__MM_H__ */



