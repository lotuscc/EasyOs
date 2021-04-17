#include "thread.h"
#include "string.h"
#include "memory.h"
#include "vga.h"
#include "pid.h"
#include "x86.h"

// struct task_struct* main_thread;       // 主线程PCB
// struct list thread_ready_list;	      // 就绪队列
// struct list thread_all_list;	         // 所有任务队列
// static struct list_elem* thread_tag;   // 用于保存队列中的线程结点

extern void switch_to(struct context *from, struct context *to);
extern void run_to(struct context *to);
extern void exit_entey(void);

// 获取当前线程pcb指针
// struct task_struct* running_thread() {
//    uint32_t esp; 
//    asm ("mov %%esp, %0" : "=g" (esp));
//    // 取esp整数部分即pcb起始地址
//    return (struct task_struct*)(esp & 0xfffff000);
// }

// /* 由kernel_thread去执行function(func_arg) */
// static void kernel_thread(thread_func* function, void* func_arg) {
// /* 执行function前要开中断,避免后面的时钟中断被屏蔽,而无法调度其它线程 */
// //    intr_enable();
//    function(func_arg); 
// }

// // 初始化线程栈thread_stack,将待执行的函数和参数放到thread_stack中相应的位置
// void thread_create(struct task_struct* pthread, thread_func function, void* func_arg) {
//    // 先预留中断使用栈的空间,可见thread.h中定义的结构
//    pthread->self_kstack -= sizeof(struct intr_stack);

//    // 再留出线程栈空间,可见thread.h中定义
//    pthread->self_kstack -= sizeof(struct thread_stack);
//    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
//    kthread_stack->eip = kernel_thread;
//    kthread_stack->function = function;
//    kthread_stack->func_arg = func_arg;
//    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->esi = kthread_stack->edi = 0;
// }

// /* 初始化线程基本信息 */
// void init_thread(struct task_struct* pthread, char* name, int prio) {
//    memset(pthread, 0, sizeof(*pthread));
//    strcpy(pthread->name, name);

//    if (pthread == main_thread) {
// // 由于把main函数也封装成一个线程,并且它一直是运行的,故将其直接设为TASK_RUNNING */
//       pthread->status = TASK_RUNNING;
//    } else {
//       pthread->status = TASK_READY;
//    }

// // self_kstack是线程自己在内核态下使用的栈顶地址 */
//    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
//    pthread->priority = prio;
//    pthread->ticks = prio;
//    pthread->elapsed_ticks = 0;
//    pthread->pgdir = NULL;
//    pthread->stack_magic = 0x19870916;	  // 自定义的魔数
// }

// // 创建一优先级为prio的线程,线程名为name,线程所执行的函数是function(func_arg) */
// struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg) {
// // pcb都位于内核空间,包括用户进程的pcb也是在内核空间 */
//    struct task_struct* thread = get_kernel_pages(1);

//    init_thread(thread, name, prio);
//    thread_create(thread, function, func_arg);

//    /* 确保之前不在队列中 */
// //    ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
//    /* 加入就绪线程队列 */
// //    list_append(&thread_ready_list, &thread->general_tag);

//    /* 确保之前不在队列中 */
// //    ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
//    /* 加入全部线程队列 */
// //    list_append(&thread_all_list, &thread->all_list_tag);

//    return thread;
// }

// // 将kernel中的main函数完善为主线程
// static void make_main_thread(void) {
// // 因为main线程早已运行,咱们在loader.S中进入内核时的mov esp,0xc009f000,
// // 就是为其预留了tcb,地址为0xc009e000,因此不需要通过get_kernel_page另分配一页*/
//    main_thread = running_thread();
//    init_thread(main_thread, "main", 31);

// // main函数是当前线程,当前线程不在thread_ready_list中,
// // 所以只将其加在thread_all_list中. */
// //    ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
// //    list_append(&thread_all_list, &main_thread->all_list_tag);
// }

// /* 实现任务调度 */
// void schedule() {

//    // ASSERT(intr_get_status() == INTR_OFF);

//    struct task_struct* cur = running_thread(); 
//    if (cur->status == TASK_RUNNING) { // 若此线程只是cpu时间片到了,将其加入到就绪队列尾
//       ASSERT(!elem_find(&thread_ready_list, &cur->general_tag));
//       list_append(&thread_ready_list, &cur->general_tag);
//       cur->ticks = cur->priority;     // 重新将当前线程的ticks再重置为其priority;
//       cur->status = TASK_READY;
//    } else { 
//       /* 若此线程需要某事件发生后才能继续上cpu运行,
//       不需要将其加入队列,因为当前线程不在就绪队列中。*/
//    }

// //    ASSERT(!list_empty(&thread_ready_list));
//    thread_tag = NULL;	  // thread_tag清空
// /* 将thread_ready_list队列中的第一个就绪线程弹出,准备将其调度上cpu. */
//    thread_tag = list_pop(&thread_ready_list);   
//    struct task_struct* next = elem2entry(struct task_struct, general_tag, thread_tag);
//    next->status = TASK_RUNNING;

//    /* 击活任务页表等 */
//    process_activate(next);

//    switch_to(cur, next);
// }

// /* 当前线程将自己阻塞,标志其状态为stat. */
// void thread_block(enum task_status stat) {
// /* stat取值为TASK_BLOCKED,TASK_WAITING,TASK_HANGING,也就是只有这三种状态才不会被调度*/
//    ASSERT(((stat == TASK_BLOCKED) || (stat == TASK_WAITING) || (stat == TASK_HANGING)));
//    enum intr_status old_status = intr_disable();
//    struct task_struct* cur_thread = running_thread();
//    cur_thread->status = stat; // 置其状态为stat 
//    schedule();		      // 将当前线程换下处理器
// /* 待当前线程被解除阻塞后才继续运行下面的intr_set_status */
//    intr_set_status(old_status);
// }

// /* 将线程pthread解除阻塞 */
// void thread_unblock(struct task_struct* pthread) {
// //    enum intr_status old_status = intr_disable();
// //    ASSERT(((pthread->status == TASK_BLOCKED) || (pthread->status == TASK_WAITING) || (pthread->status == TASK_HANGING)));
//     if (pthread->status != TASK_READY) {
//     //   ASSERT(!elem_find(&thread_ready_list, &pthread->general_tag));
//         if (elem_find(&thread_ready_list, &pthread->general_tag)) {
        
//         }
//         list_push(&thread_ready_list, &pthread->general_tag);    // 放到队列的最前面,使其尽快得到调度
//         pthread->status = TASK_READY;
//     } 
   
// //    intr_set_status(old_status);
// }

// /* 初始化线程环境 */
// void thread_init(void) {
    
//     list_init(&thread_ready_list);
//     list_init(&thread_all_list);
//     // 将当前main函数创建为线程
//     make_main_thread();
// }


// void eos_proc_create(struct proc_struct* proc, thread_func func, void* func_arg){

// }


struct proc_struct* eos_proc_start(char* name, int prio, thread_func function, void* func_arg){
   // PCB 使用一页大小,并且位于页低端
   struct proc_struct* proc = get_kernel_pages(1);

   memset(proc, 0, sizeof(struct proc_struct));

   // eos_proc_create(proc, function, func_arg);
   memset(proc, 0, sizeof(struct proc_struct));
   
   proc->context.eip = function;



   strcpy(proc->name, name);
   proc->priority = prio;

   proc->state = TASK_READY;

   proc->pid = eos_getPID();

   // proc->kstack = (uint32_t)proc + 4096 - 4;

   // 
   proc->context.esp = (uint32_t)proc + PG_SIZE;

   proc->context.esp -= 4;

   (*(uint32_t*)proc->context.esp) = func_arg;

   proc->context.esp -= 4;

   (*(uint32_t*)proc->context.esp) = exit_entey;
 
   ++proc_all_listHead.proc_nums;

   list_add(&proc_all_listHead.entry, &proc->all_link);
   
   ++proc_ready_listHead.proc_nums;
   list_add(&proc_ready_listHead.entry, &proc->ready_link);

   return proc;
}


// exit为系统调用
// 线程退出,将该线程从链表中去除,并且调度
void eos_do_exit(void* error_code){

   struct proc_struct* cur = eos_running_proc(); 
   
   vga->putStr("\nid: ");
   vga->putInt(cur->pid);
   vga->putStr("return: ");
   vga->putInt(error_code);

   list_del(&cur->ready_link);
   --proc_ready_listHead.proc_nums;

   list_del(&cur->all_link);
   --proc_all_listHead.proc_nums;

   //释放PCB空间,本进程的资源应该由父进程释放
   // 此时释放会出现问题,下面的程序将无法运行
   // free_kernel_pages((void*)cur);
   cur->state = TASK_DIED;

   //调度直接使用此调度会出现bug
   // eos_schedule();
   struct list_entry_t* t = proc_ready_listHead.entry.next;

   struct proc_struct* next = le2proc(t, ready_link);

   eos_proc_run(next);
}


// 直接运行下一个线程
void eos_proc_run(struct proc_struct* next){
   run_to(&next->context);
}

// 替换执行程序
// 将线程升级为进程,需要改变页表,进程镜像等
int eos_do_execve(const char *name, int argc, const char **argv){
   
}

// 

// 复制父进程来创建一个子进程,页表,内存完全复制为父进程的
// 如果fork设置为系统调用的话返回时需要返回到 forkrets
int eos_do_fork(char* name, int prio, eos_func func, void* arg){
      // PCB 使用一页大小,并且位于页低端
   struct proc_struct* proc = get_kernel_pages(1);

   memset(proc, 0, sizeof(struct proc_struct));
   
   strcpy(proc->name, name);
   
   proc->priority = prio;

   proc->state = TASK_READY;

   // 设置函数位置
   proc->context.eip = func;
   
   // 设置线程栈顶 
   proc->context.esp = (uint32_t)proc + PG_SIZE;

   proc->context.esp -= 4;

   // 设置函数参数
   (*(uint32_t*)proc->context.esp) = arg;

   proc->context.esp -= 4;

   // 设置函数返回地址
   (*(uint32_t*)proc->context.esp) = exit_entey;
 
   // 将
   ++proc_all_listHead.proc_nums;

   list_add(&proc_all_listHead.entry, &proc->all_link);
   
   ++proc_ready_listHead.proc_nums;
   list_add(&proc_ready_listHead.entry, &proc->ready_link);

   // 返回pid
   return proc->pid;
}



struct  proc_struct* eos_running_proc(void){
   
   uint32_t esp = read_esp();
   // 取esp整数部分即pcb起始地址

   return (struct proc_struct*)(esp & 0xfffff000);
}

void eos_schedule(void){
   struct proc_struct* cur = eos_running_proc(); 
   if (cur->state == TASK_RUNNING) { // 若此线程只是cpu时间片到了,将其加入到就绪队列尾
      
      // 如果已经在read链表中
      if(cur->ready_link.next != NULL){
         list_del(&cur->ready_link);  
         --proc_ready_listHead.proc_nums;
      }

      // 重新加入到ready链表尾部
      proc_ready_listHead.proc_nums++;
      
      cur->ticks = cur->priority;     // 重新将当前线程的ticks再重置为其priority;
      cur->state = TASK_READY;

      list_add(proc_ready_listHead.entry.prev, &cur->ready_link);

   } else { 
      /* 若此线程需要某事件发生后才能继续上cpu运行,
      不需要将其加入队列,因为当前线程不在就绪队列中。*/
   }

   // ready链表头部寻找线程上CPU运行
   if (proc_ready_listHead.proc_nums != 0){
      struct list_entry_t* t = proc_ready_listHead.entry.next;


      struct proc_struct* next = le2proc(t, ready_link);
      next->state = TASK_RUNNING;
      next->ticks = next->priority;

      


      switch_to(&cur->context, &next->context);
   }
   

//    ASSERT(!list_empty(&thread_ready_list));
   // thread_tag = NULL;	  // thread_tag清空
/* 将thread_ready_list队列中的第一个就绪线程弹出,准备将其调度上cpu. */
   // thread_tag = list_pop(&thread_ready_list);   
   // struct task_struct* next = elem2entry(struct task_struct, general_tag, thread_tag);
   // next->status = TASK_RUNNING;

   // /* 击活任务页表等 */
   // process_activate(next);

   // switch_to(cur, next);
}


// 将kernel中的main函数完善为主线程
static void make_main_thread(void) {
// 因为main线程早已运行,咱们在loader.S中进入内核时的mov esp,0xc009f000,
// 就是为其预留了tcb,地址为0xc009e000,因此不需要通过get_kernel_page另分配一页*/
   struct proc_struct* main_thread = eos_running_proc();
   strcpy(main_thread->name, "main");
   main_thread->priority = 2;

   main_thread->state = TASK_RUNNING;

   main_thread->pid = eos_getPID();


// main函数是当前线程,当前线程不在thread_ready_list中,
// 所以只将其加在thread_all_list中. */l

   proc_all_listHead.proc_nums++;
   list_add(&proc_all_listHead.entry, &main_thread->all_link);

}


void eos_proc_init(void){
   list_init(&proc_ready_listHead.entry);
   list_init(&proc_all_listHead.entry);

   proc_all_listHead.proc_nums = 0;
   proc_ready_listHead.proc_nums = 0;

   eos_PIDinit();

   make_main_thread();
}

void debug_all_list(void){
   struct list_entry_t* head = &proc_all_listHead.entry;
   struct list_entry_t* it = head->next;
   while(it  != head){
      vga->putStr("proc: ");

      struct proc_struct* t =  le2proc(it, all_link);
      vga->putStr(t->name);
      vga->putChar(' ');

      it = it->next;
   }
}


void debug_ready_list(void){
   struct list_entry_t* head = &proc_ready_listHead.entry;
   struct list_entry_t* it = head->next;
   while(it  != head){
      vga->putStr("proc: ");

      struct proc_struct* t =  le2proc(it, ready_link);
      vga->putStr(t->name);
      vga->putChar(' ');

      it = it->next;
   }
}