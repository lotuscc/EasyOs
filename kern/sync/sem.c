#include "defs.h"
#include "sem.h"
#include "proc.h"
#include "sync.h"
#include "thread.h"

// 初始化信号量
void sema_init(struct semaphore* psema, uint8_t value) {
   psema->value = value;       // 为信号量赋初值
   list_init(&psema->waiters); //初始化信号量的等待队列
}

// 初始化锁plock
void lock_init(struct lock* plock) {
   plock->holder = NULL;
   plock->holder_repeat_nr = 0;
   sema_init(&plock->semaphore, 1);  // 信号量初值为1
}

// 信号量down操作
void sema_down(struct semaphore* psema) {
    // 关中断来保证原子操作
    bool intr_flag;
    local_intr_save(intr_flag);
    {

        // 若value为0,表示已经被别人持有
        while(psema->value == 0) {	

            // 若信号量的值等于0,则当前线程把自己加入该锁的等待队列,然后阻塞自己
            list_add(&psema->waiters, &eos_running_proc()->ready_link); 
        
            // 阻塞线程,直到被唤醒
            eos_thread_block(TASK_BLOCKED);    
        }
        // 若value为1或被唤醒后,会执行下面的代码,也就是获得了锁
        psema->value--;
    
    }
    // 恢复之前的中断状态
    local_intr_save(intr_flag);
}

// 信号量的up操作
void sema_up(struct semaphore* psema) {
    
    // 关中断,保证原子操作
    bool intr_flag;
    local_intr_save(intr_flag);
    {    
        if (!list_empty(&psema->waiters)) {
            // struct task_struct* thread_blocked = elem2entry(struct task_struct, general_tag, list_pop(&psema->waiters));
            struct list_entry_t* t = psema->waiters.next;

            // 从ready链表中删除
            list_del(t);  

            struct proc_struct* thread_blocked = le2proc(t, ready_link);
            
            eos_thread_unblock(thread_blocked);
        }
        psema->value++;    
    }
    // 恢复之前的中断状态
    local_intr_save(intr_flag);
}

// 获取锁plock
void lock_acquire(struct lock* plock) {
    // 排除曾经自己已经持有锁但还未将其释放的情况
    if (plock->holder != eos_running_proc()) { 
        
        // 对信号量P操作,原子操作
        sema_down(&plock->semaphore);    
        
        plock->holder = eos_running_proc();
        
        plock->holder_repeat_nr = 1;
    } else {
        plock->holder_repeat_nr++;
    }
}

// 释放锁plock
void lock_release(struct lock* plock) {

   if (plock->holder_repeat_nr > 1) {
      plock->holder_repeat_nr--;
      return;
   }
   // 把锁的持有者置空放在V操作之前
   plock->holder = NULL;	        
   plock->holder_repeat_nr = 0;
    // 信号量的V操作,也是原子操作
   sema_up(&plock->semaphore);	  
}

