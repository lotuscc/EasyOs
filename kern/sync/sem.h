#ifndef __KERN_SYNC_SEM_H__
#define __KERN_SYNC_SEM_H__

#include "defs.h"
#include "list.h"

// 信号量结构
struct semaphore {
   uint8_t  value;
   struct list_entry_t waiters;
};


// 锁结构
struct lock {
   struct proc_struct* holder;	        // 锁的持有者
   struct semaphore semaphore;	        // 用二元信号量实现锁
   uint32_t holder_repeat_nr;		    // 锁的持有者重复申请锁的次数
};

void sema_init(struct semaphore* psema, uint8_t value); 
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_init(struct lock* plock);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);

#endif /* !__KERN_SYNC_SEM_H__ */

