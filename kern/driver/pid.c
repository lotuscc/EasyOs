#include "pid.h"
#include "memory.h"

static struct bitmap pid_pool;

void eos_PIDinit(){
    // 从内核空间中分配一页,并且关联到物理地址
    pid_pool.bits = get_kernel_pages(1);

    // 清空一页,总共可以分配的pid个数为PG_SIZE*8
    pid_pool.len = PG_SIZE;

    bitmap_init(& pid_pool);
}

int eos_getPID(void){
    int pid = bitmap_scan(&pid_pool, 1);
    bitmap_set(&pid_pool, pid, 1);

    return pid;
}


void eos_clearPID(int pid){
    bitmap_set(&pid_pool, pid, 0);
}