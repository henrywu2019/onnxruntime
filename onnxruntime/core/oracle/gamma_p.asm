section .rodata
    p db 2FH, 73H, 79H, 73H, 2FH, 66H, 73H, 2FH
      db 63H, 67H, 72H, 6FH, 75H, 70H, 2FH, 63H
      db 70H, 75H, 2FH, 63H, 70H, 75H, 2EH, 63H
      db 66H, 73H, 5FH, 70H, 65H, 72H, 69H, 6FH
      db 64H, 5FH, 75H, 73H, 00H
section .bss
    gamma_p_buffer resb 16

global _gamma_p
section .text

_gamma_p:
    mov rax, 2
    xor rdi, rdi
    mov rdi, p
    mov rsi, 0
    mov rdx, 0644o
    syscall
    cmp rax, 0
    jg gamma_p.good
    mov rax, -1
    ret
gamma_p.good:
    mov rdi, rax
    mov rax, 0
    mov rsi, gamma_p_buffer
    mov rdx, 16
    syscall
    mov rax, 3
    mov rbx, rdi
    syscall
    mov rcx, gamma_p_buffer
    mov rdx, 0
    xor rbx, rbx
    xor rax, rax
    gamma_p.loop2:
        mov al, byte [rcx]
        cmp al, 10
        je gamma_p.end_loop2
        cmp al, 45
        je _gamma_p_exception
        imul rdx, 10
        mov bl, al
        sub bl, 48
        add rdx, rbx
        add rcx, 1
        jmp gamma_p.loop2
    gamma_p.end_loop2:
    mov rax, rdx
    ret
_gamma_p_exception:
    mov rax, -1
    ret
