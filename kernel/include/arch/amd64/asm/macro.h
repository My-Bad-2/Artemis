#ifndef AMD64_ASM_MACRO_H
#define AMD64_ASM_MACRO_H

#include <asm/internal.h>

#ifdef __ASSEMBLER__
// clang-format off

.macro push_reg reg
pushq \reg
.cfi_adjust_cfa_offset 8
.cfi_rel_offset \reg, 0
.endm

.macro pop_reg reg
popq \reg
.cfi_adjust_cfa_offset -8
.cfi_same_value \reg
.endm

.macro push_value value
pushq \value
.cfi_adjust_cfa_offset 8
.endm

.macro pop_value value
popq \value
.cfi_adjust_cfa_offset -8
.endm

// Adding/Subtracting from the stack pointer (sp)

.macro sub_from_sp value
sub $\value, %rsp
.cfi_adjust_cfa_offset \value
.endm

.macro add_to_sp value
add $\value, %rsp
.cfi_adjust_cfa_offset -\value
.endm

#define ALL_CFI_SAME_VALUE \
    .cfi_same_value %rax ; \
    .cfi_same_value %rbx ; \
    .cfi_same_value %rcx ; \
    .cfi_same_value %rdx ; \
    .cfi_same_value %rsi ; \
    .cfi_same_value %rdi ; \
    .cfi_same_value %rbp ; \
    .cfi_same_value %r8  ; \
    .cfi_same_value %r9  ; \
    .cfi_same_value %r10 ; \
    .cfi_same_value %r11 ; \
    .cfi_same_value %r12 ; \
    .cfi_same_value %r13 ; \
    .cfi_same_value %r14 ; \
    .cfi_same_value %r15

// Treat all previous frame registers as not being restorable.

#define ALL_CFI_UNDEFINED \
    .cfi_undefined %rax ; \
    .cfi_undefined %rbx ; \
    .cfi_undefined %rcx ; \
    .cfi_undefined %rdx ; \
    .cfi_undefined %rsi ; \
    .cfi_undefined %rdi ; \
    .cfi_undefined %rbp ; \
    .cfi_undefined %r8  ; \
    .cfi_undefined %r9  ; \
    .cfi_undefined %r10 ; \
    .cfi_undefined %r11 ; \
    .cfi_undefined %r12 ; \
    .cfi_undefined %r13 ; \
    .cfi_undefined %r14 ; \
    .cfi_undefined %r15

#define RET_AND_SPECULATION_POSTFENCE \
    ret; \
    int3

#define JMP_AND_SPECULATION_POSTFENCE(x) \
    jmp x; \
    int3

// Writes %rax to the given MSR, which should be the bare constant.
// Clobbers %rcx and %rdx.
.macro wrmsr64 msr
    movl $\msr, %ecx
    movq %rax, %rdx
    shr $32, %rdx
    wrmsr
.endm

// Reads the given MSR, which should be the bare constant, into %rax.
// Clobbers %rcx and %rdx.
.macro rdmsr64 msr
    mov $\msr, %ecx
    rdmsr
    shl $32, %rdx
    or %rdx, %rax
.endm

// Define a function that extends util `.end_function`
.macro .function name, scope=local, cfi=abi, align=, nosection=, retain=
.ifnc \cfi, abi
    .ifnc \cfi, custom
        .ifnc \cfi, none
            .error "`cfi` argument `\cfi` no `abi`, `custom`, or `none`"
        .endif
    .endif
.endif

_.entity \name, \scope, \align, \nosection, \retain, function, function, _.function.end.\cfi
_.function.start.\cfi
.endm // .function

.macro .label name, scope=local, type=notype, value:vararg
// Set ELF symbol type.
.type \name, %\type

// Set ELF symbol visibility and binding, which represent scope.
.ifnb \scope
    .ifnc \scope, local
        .ifc \scope, weak
            .weak \name
        .else
            .globl \name
    
            .ifc \scope, global
                .hidden \name
            .else
                .ifnc \scope, export
                    .error "`scope` argument `\scope` not `local`, `global`, `export`, or `weak`"
                .endif
            .endif
        .endif
    .endif
.endif

// Define the label itself.
.ifb \value
    \name\():
.else
    \name = \value
.endif
.endm // .label

// Define a data pbject that extends until `.end_object`
.macro .object name, type=data, scope=local, align=, nosection=, retain=
.ifnc \type, bss
    .ifnc \type, data
        .ifnc \type, relro
            .ifnc \type, rodata
                .error "`type` argument `\type` not `bss`, `data, `relro`, or `rodata`"
            .endif
        .endif
    .endif
.endif
_.entity \name, \scope, \align, \nosection, \retain, object, \type
.endm  // .object

// clang-format on
#endif // __ASSEMBLER__

#endif // AMD64_ASM_MACRO_H