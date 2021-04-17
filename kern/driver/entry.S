.text
.globl kernel_thread_entry
.globl exit_entey
kernel_thread_entry:        # void kernel_thread(void)

    pushl %edx              # push arg
    call *%ebx              # call fn

exit_entey:
    pushl %eax              # save the return value of fn(arg)
    call eos_do_exit            # call do_exit to terminate current thread

