; Fuheng Wu 11/12/2022
global _compute_gme_block

section .text
_compute_gme_block:
    mov rax, rdi
    mov rbx, rsi

    xor r13, r13
_compute_start:

    xor r14, r14
    vbroadcastss ymm0, [rdx+r13]
_compute1:
    vmovups ymm12, [rbx+r14*4]
    mov r15, [rax]
    vmovups ymm11, [r15+r14*4]
    vfmadd231ps ymm12, ymm0, ymm11
    vmovups [rbx+r14*4], ymm12
    add r14,8
    cmp r14, r8
    jl _compute1
    xor r14, r14
    mov r12, r8
    shl r12, 2
    vbroadcastss ymm3, [rdx+r13+3*4]
_compute2:
    vmovups ymm12, [rbx+r14*4]
    vmovups ymm13, [rbx+r12*4]
    mov r15, qword [rax+8]
    vmovups ymm11, [r15+r14*4]
    vfmadd231ps ymm12, ymm3, ymm11
    vfmadd231ps ymm13, ymm0, ymm11
    vmovups [rbx+r14*4], ymm12
    vmovups [rbx+r12*4], ymm13
    add r14,8
    add r12,8
    cmp r14, r8
    jl _compute2
    mov r9, 2
    mov r10, rcx
    sub r10, 2
_compute3_outer:
    xor r14, r14
    vbroadcastss ymm6, [rdx+r13+6*4]
_compute3_inner:
    mov r15, qword [rax+r9*8]
    vmovups ymm11, [r15+r14*4]
    mov r12, r9
    sub r12, 2
    imul r12, r8
    add r12, r14
    shl r12, 2
    mov r11, rbx
    add r11, r12
    mov r12, r11
    vmovups ymm12, [r12]
    vmovups ymm13, [r12+r8*4]
    vmovups ymm14, [r12+r8*8]
    vfmadd231ps ymm12, ymm6, ymm11
    vfmadd231ps ymm13, ymm3, ymm11
    vfmadd231ps ymm14, ymm0, ymm11
    vmovups [r12], ymm12
    vmovups [r12+r8*4], ymm13
    vmovups [r12+r8*8], ymm14
    add r14, 8
    cmp r14, r8
    jl _compute3_inner
    inc r9
    cmp r9, r10
    jl _compute3_outer
    xor r14, r14
_compute4:
    mov r15, [rax+r10*8]
    vmovups ymm11, [r15+r14*4]
    mov r12, r10
    sub r12, 2
    imul r12, r8
    add r12, r14
    shl r12, 2
    add r12, rbx
    vmovups ymm12, [r12]
    vmovups ymm13, [r12+r8*4]
    vfmadd231ps ymm12, ymm6, ymm11
    vfmadd231ps ymm13, ymm3, ymm11
    vmovups [r12], ymm12
    vmovups [r12+r8*4], ymm13
    add r14,8
    cmp r14, r8
    jl _compute4
    xor r14, r14
    inc r10
_compute5:
    mov r15, [rax+r10*8]
    vmovups ymm11, [r15+r14*4]
    mov r12, r10
    sub r12, 2
    imul r12, r8
    add r12, r14
    shl r12, 2
    add r12, rbx
    vmovups ymm12, [r12]
    vfmadd231ps ymm12, ymm6, ymm11
    vmovups [r12], ymm12
    add r14,8
    cmp r14, r8
    jl _compute5
_advance:
    xor r14, r14
_advance_loop:
    mov r15, qword [rax+r14*8]
    add r15, 4
    mov qword [rax+r14*8], r15
    inc r14
    cmp r14, rcx
    jl _advance_loop
_end_compute_block:
    inc r13
    cmp r13, 3
    Jl _compute_start
    ret
