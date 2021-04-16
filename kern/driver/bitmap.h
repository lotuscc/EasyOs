#ifndef __BITMAP_H__
#define __BITMAP_H__
#include "elf.h"
#include "x86.h"
#include "string.h"

struct bitmap{
    uint32_t len;   // 位图长度,单位字节 
    uint8_t* bits;  // 位图起始地址
};

// struct bitmapdevice{
//     struct bitmap btmp;
    
//     void (*init)(struct bitmap* btmp);
//     bool (*test)(struct bitmap* btmp, uint32_t bit_idx);
//     int (*scan)(struct bitmap* btmp, uint32_t cnt);
//     void (*set)(struct bitmap* btmp, uint32_t bit_idx, int8_t value);
// };

void bitmap_init(struct bitmap* btmp);
bool bitmap_test(struct bitmap* btmp, uint32_t bit_idx);
int bitmap_scan(struct bitmap* btmp, uint32_t cnt);
void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value);


#endif /* !__BITMAP_H__ */



