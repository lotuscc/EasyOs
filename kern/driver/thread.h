#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H

#include "x86.h"
#include "list.h"
#include "bitmap.h"
#include "memory.h"
#include "list.h"


// 自定义通用函数类型,它将在很多线程函数中做为形参类型
typedef void thread_func(void*);


typedef void* eos_func(void*);

/* 进程或线程的状态 */
enum task_status {
   TASK_UNINIT,
   TASK_RUNNING,
   TASK_READY,
   TASK_BLOCKED,
   TASK_WAITING,
   TASK_HANGING,
   TASK_DIED
};


// Saved registers for kernel context switches.
// Don't need to save all the %fs etc. segment registers,
// because they are constant across kernel contexts.
// Save all the regular registers so we don't need to care
// which are caller save, but not the return register %eax.
// (Not saving %eax just simplifies the switching code.)
// The layout of context must match code in switch.S.


struct context {
    uint32_t eip;   // eip寄存器指向指令的地址,本身不能进行操作,这里通过特殊方式来保存
    uint32_t esp;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
};

#define PROC_NAME_LEN 16 

struct proc_struct {
   enum task_status state;                      // Process state
   int pid;                                    // Process ID
   int runs;                                   // the running times of Proces
   
   uintptr_t kstack;                           // Process kernel stack
   
   volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
   
   struct proc_struct *parent;                 // the parent process
   struct mm_struct *mm;                       // Process's memory management field
   struct context context;                     // Switch here to run process
   
   struct trapframe *tf;                       // Trap frame for current interrupt
   
   uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
   uint32_t flags;                             // Process flag
   char name[PROC_NAME_LEN];                   // Process name
   struct list_entry_t list_link;              // Process link list
   struct list_entry_t hash_link;              // Process hash list
   int exit_code;                              // exit code (be sent to parent proc)
   uint32_t wait_state;                        // waiting state
   struct proc_struct *cptr, *yptr, *optr;     // relations between processes
   struct run_queue *rq;                       // running queue contains Process
   struct list_entry_t run_link;                      // the entry linked in run queue
   int time_slice;                             // time slice for occupying the CPU
   // skew_heap_entry_t lab6_run_pool;            // FOR LAB6 ONLY: the entry in the run pool
   // uint32_t lab6_stride;                       // FOR LAB6 ONLY: the current stride of the process
   // uint32_t lab6_priority;                     // FOR LAB6 ONLY: the priority of process, set by lab6_set_priority(uint32_t)
   // struct files_struct *filesp;                // the file related info(pwd, files_count, files_array, fs_semaphore) of process

   uint32_t* pgdir;              

   // 用户进程的虚拟地址
   struct virtual_addr userprog_vaddr;              

   struct list_entry_t ready_link;
   struct list_entry_t all_link;

   // 优先级
   uint32_t priority;
   
   // 每次在处理器上执行的时间嘀嗒数
   uint32_t ticks;

   // 魔数用来判断栈内核栈是否溢出	  
   uint32_t stack_magic;	 
};

// 进程或者线程头结点
struct proc_head{
   int proc_nums;
   struct list_entry_t entry;
};

// 宏定义，将list_entry_t类型指针转化为特定类型指针，用于访问特定类型中的数据
#define le2proc(le, member)                 \
to_struct((le), struct proc_struct, member)

struct proc_head proc_ready_listHead;
struct proc_head proc_all_listHead;

void eos_proc_init(void);
void eos_proc_create(struct proc_struct* proc, thread_func func, void* func_arg);
struct proc_struct* eos_proc_start(char* name, int prio, thread_func function, void* func_arg);
struct  proc_struct* eos_running_proc(void);
void eos_schedule(void);
void eos_do_exit(void* arg);
int eos_do_execve(const char *name, int argc, const char **argv);

void thread_create(struct task_struct* pthread, thread_func function, void* func_arg);
void init_thread(struct task_struct* pthread, char* name, int prio);
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg);
struct task_struct* running_thread(void);
void schedule(void);
void thread_init(void);
void thread_unblock(struct task_struct* pthread);
void thread_block(enum task_status stat);


#endif
