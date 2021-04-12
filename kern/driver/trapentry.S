
# vectors.S sends all traps here.
.text
.globl __alltraps
__alltraps:
    # push_registers to build a trap frame
    # therefore make the stack look like a struct trapframe
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushal

    # load GD_KDATA into %ds and %es to set up data segments for kernel
    # movl $GD_KDATA, %eax
    # movw %ax, %ds
    # movw %ax, %es

    # push_%esp to pass a pointer to the trapframe as an argument to trap()
    pushl %esp

    # call trap(tf), where tf=%esp
    call trap

    # pop_the pushed stack pointer
    popl %esp

    # return falls through to trapret...
.globl __trapret
__trapret:
    # restore registers from stack
    popal

    # restore %ds, %es, %fs and %gs
    popl %gs
    popl %fs
    popl %es
    popl %ds

    # get rid of the trap number and error code
    # 对于有没有错误码的中断,我们在向量入口处多弹入了一个32位的0
    # 有错误码的中断(例如8号),只弹入了中断号,因此直接add 8 可以统一起来
    addl $0x8, %esp
    iret

.globl forkrets
forkrets:
    # set stack to this new process's trapframe
    movl 4(%esp), %esp
    jmp __trapret
