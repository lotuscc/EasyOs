#ifndef __PIC_H__
#define __PIC_H__
#include "elf.h"
#include "x86.h"

void pic_init(void);

static void pic_setmask(uint16_t mask);

#endif /* !__PIC_H__ */



