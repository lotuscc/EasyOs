.text
.globl switch_to
.globl run_to
switch_to:                      # switch_to(from, to)

    # save from's registers
    movl 4(%esp), %eax          # eax points to from
    
    popl 0(%eax)                # save eip !popl,注意这里是pop而不是push,主要将返回地址保存到from的context的eip中
    
    movl %esp, 4(%eax)
    movl %ebx, 8(%eax)
    movl %ecx, 12(%eax)
    movl %edx, 16(%eax)
    movl %esi, 20(%eax)
    movl %edi, 24(%eax)
    movl %ebp, 28(%eax)

run_to:
    # restore to's registers
    movl 4(%esp), %eax          # not 8(%esp): popped return address already
                                # eax now points to to
    movl 28(%eax), %ebp
    movl 24(%eax), %edi
    movl 20(%eax), %esi
    movl 16(%eax), %edx
    movl 12(%eax), %ecx
    movl 8(%eax), %ebx
    movl 4(%eax), %esp          # 这里改变了esp的值,所以下面的push以及ret操作的是to的栈,与前面操作的不是同一个栈

    pushl 0(%eax)               # push eip,将返回地址压入栈中

    ret

