global _compute

;>>> p **(float**)($rax)
;$1 = 1
;>>> p *(float*)($r15)
;$2 = 1
; _compute(pa, O, F, ih, len);


section .text

; rdx - F, rcx - ih, r8 - len
_compute:
    mov rax, rdi ; pa
    mov rbx, rsi ; O

    xor r13, r13 ;k
_compute_start:

    xor r14, r14 ; j
    vbroadcastss ymm0, [rdx+r13] ; F[k]
_compute1:
    vmovups ymm12, [rbx+r14*4] ; O[j]
    mov r15, [rax]
    vmovups ymm11, [r15+r14*4] ; *(pa[0] + j)
    vfmadd231ps ymm12, ymm0, ymm11 ; O[j] += pa[0][j] * F[k];
    vmovups [rbx+r14*4], ymm12
    add r14,8
    cmp r14, r8
    jl _compute1


    xor r14, r14 ; j
    mov r12, r8 ; tmp for j+len
    shl r12, 2 ; len*4 as index
    vbroadcastss ymm3, [rdx+r13+3*4] ; F[k+3]
_compute2:
    vmovups ymm12, [rbx+r14*4] ; O[j]
    vmovups ymm13, [rbx+r12*4] ; O[j+len]
    mov r15, qword [rax+8] ; pa[1]
    ;add r15, 8*1 ;pa[1]
    vmovups ymm11, [r15+r14*4] ; pa[1][j];;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    vfmadd231ps ymm12, ymm3, ymm11; O[j] += pa[1][j] * F[k + 3];
    vfmadd231ps ymm13, ymm0, ymm11;
    vmovups [rbx+r14*4], ymm12
    vmovups [rbx+r12*4], ymm13
    add r14,8
    add r12,8
    cmp r14, r8
    jl _compute2


    mov r9, 2; i
    mov r10, rcx
    sub r10, 2 ; ih-2
_compute3_outer:
    xor r14, r14 ; j
    vbroadcastss ymm6, [rdx+r13+6*4] ; F[k+6]
_compute3_inner:
    mov r15, qword [rax+r9*8] ;pa
    ;mov r12, r9 ; i
    ;shl r12, 2 ; i*4
    ;add r15, r12 ; pa[i]
    vmovups ymm11, [r15+r14*4] ; pa[i][j]
    mov r12, r9
    sub r12, 2
    imul r12, r8 ; (i-2)*len
    add r12, r14; idx
    shl r12, 2 ; idx*4
    mov r11, rbx
    add r11, r12; O[idx]
    mov r12, r11
    vmovups ymm12, [r12] ; O[idx]
    vmovups ymm13, [r12+r8*4] ;O[idx+len]
    vmovups ymm14, [r12+r8*8] ; O[idx+2*len]
    vfmadd231ps ymm12, ymm6, ymm11; O[j] += pa[1][j] * F[k + 3];
    vfmadd231ps ymm13, ymm3, ymm11;
    vfmadd231ps ymm14, ymm0, ymm11;
    vmovups [r12], ymm12
    vmovups [r12+r8*4], ymm13
    vmovups [r12+r8*8], ymm14
    add r14, 8
    cmp r14, r8
    jl _compute3_inner
    inc r9
    cmp r9, r10 ; ih-2
    jl _compute3_outer

    xor r14, r14 ; j=0
_compute4:
    mov r15, [rax+r10*8]
    ;mov r9, r10
    ;shl r9, 2
    ;add r15, r9 ; pa[ih-2]
    vmovups ymm11, [r15+r14*4] ; pa[i][j]

    mov r12, r10 ; ih-2
    sub r12, 2
    imul r12, r8
    add r12, r14
    shl r12, 2
    add r12, rbx
    vmovups ymm12, [r12]      ; O[idx]
    vmovups ymm13, [r12+r8*4] ;O[idx+len]
    vfmadd231ps ymm12, ymm6, ymm11 ; O[j] += pa[1][j] * F[k + 3];
    vfmadd231ps ymm13, ymm3, ymm11
    vmovups [r12], ymm12
    vmovups [r12+r8*4], ymm13

    add r14,8
    cmp r14, r8
    jl _compute4


    xor r14, r14 ; j=0
    inc r10 ; ih-1
_compute5:
    ;mov r15, [rax] ;pa
    ;mov r12, r9 ; i
    ;shl r12, 2 ; i*4
    ;add r15, r12 ; pa[i]
    ;vmovups ymm11, [r15+r14*4] ; pa[i][j]

    mov r15, [rax+r10*8]
    ;mov r9, r10
    ;shl r9, 2
    ;add r15, r9 ; pa[ih-1]
    vmovups ymm11, [r15+r14*4] ; pa[ih-1][j]
    mov r12, r10 ; ih-1
    sub r12, 2 ; ih-3
    imul r12, r8 ; (ih-3)*len
    add r12, r14 ; (ih-3)*len + j
    shl r12, 2 ; ((ih-3)*len + j)*4
    add r12, rbx ; O[idx]
    vmovups ymm12, [r12]      ; O[idx]
    vfmadd231ps ymm12, ymm6, ymm11 ; O[j] += pa[1][j] * F[k + 3];
    vmovups [r12], ymm12
    add r14,8
    cmp r14, r8
    jl _compute5

_advance:
    xor r14, r14
  _advance_loop:
    mov r15, qword [rax+r14*8]; *pa
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