extern _gamma_p
extern _gamma_q
extern _gamma_hc

global _gamma_tn
section .text

_gamma_tn:
    call _gamma_q
    cmp rax, -1
    je fall_back
    mov rbx, rax
    call _gamma_p
    cmp rax, -1
    je fall_back
    idiv rbx
    mov rax, rbx
    ret
    
fall_back:
    call _gamma_hc
    ret
