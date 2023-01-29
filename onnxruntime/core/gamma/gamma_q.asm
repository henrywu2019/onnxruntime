section .rodata
    q db "/sys/fs/cgroup/cpu/cpu.cfs_quota_us",0

section .bss
    buffer   resb 16

global _gamma_q
section .text

_gamma_q:
    mov rax, 2
    xor rdi, rdi
    mov edi, q
    mov rsi, 0
    mov rdx, 0644o
    syscall

    cmp rax, 0
    jle _gamma_q_exception
    mov rdi, rax
    mov rax, 0
    mov rsi, buffer
    mov rdx, 16
    syscall

    mov rax, 3
    mov rbx, rdi
    syscall
    mov rcx, buffer
    mov rdx, 0
    xor rbx, rbx
    xor rax, rax

    loop2:
        mov al, byte [rcx]
        cmp al, 10
        je end_loop2
        imul rdx, 10
        mov bl, al
        sub bl, '0'
        add rdx, rbx
        add rcx, 1
        jmp loop2
    end_loop2:
    mov rax, rdx
    ret
_gamma_q_exception:
    mov rax, -1
    ret