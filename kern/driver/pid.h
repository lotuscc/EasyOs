#ifndef __KERNEL_PID_H__
#define __KERNEL_PID_H__
#include "bitmap.h"

void eos_PIDinit();

int eos_getPID(void);

void eos_clearPID(int pid);

#endif /* !__KERNEL_PID_H__ */



