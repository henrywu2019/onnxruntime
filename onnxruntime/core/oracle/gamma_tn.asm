extern _gamma_p
extern _gamma_q
extern _gamma_hc
global _gamma_tn
section .text
_gamma_tn:
    push rbx
    push rcx
    push rdx
    call _gamma_q
    cmp rax, -1
    je gamma_tn.fall_back
    mov rbx, rax
    call _gamma_p
    cmp rax, -1
    je gamma_tn.fall_back
    idiv rbx
    mov rax, rbx
    pop rdx
    pop rcx
    pop rbx
    ret
gamma_tn.fall_back:
    call _gamma_hc
    pop rdx
    pop rcx
    pop rbx
    ret

