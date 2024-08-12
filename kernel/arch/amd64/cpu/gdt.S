#include <asm/macro.h>
#include <asm/regs.h>

.section .text

.function LoadGdt, scope=global, align=64
    lgdtq (%rdi)
    movw $KERNEL_DATA_SELECTOR, %ax
    movl %eax, %ds
    movl %eax, %fs
    movl %eax, %gs
    movl %eax, %es
    movl %eax, %ss
    pushq $KERNEL_CODE_SELECTOR
    lea .gdt_trampoline(%rip), %rax
    pushq %rax
    lretq
.gdt_trampoline:
    retq
.end_function

.function LoadTss, scope=global
    movw $TSS_SELECTOR, %ax
    ltrw %ax
    retq
.end_function