section .rodata
    q db 2FH, 73H, 79H, 73H, 2FH, 66H, 73H, 2FH
      db 63H, 67H, 72H, 6FH, 75H, 70H, 2FH, 63H
      db 70H, 75H, 2FH, 63H, 70H, 75H, 2EH, 63H
      db 66H, 73H, 5FH, 71H, 75H, 6FH, 74H, 61H
      db 5FH, 75H, 73H, 00H
section .bss
    gamma_q_buffer   resb 16
global _gamma_q
section .text
_gamma_q:
    mov rax, 2
    xor rdi, rdi
    mov rdi, q
    mov rsi, 0
    mov rdx, 0644o
    syscall
    cmp rax, 0
    jle _gamma_q_exception
    mov rdi, rax
    mov rax, 0
    mov rsi, gamma_q_buffer
    mov rdx, 16
    syscall

    mov rax, 3
    mov rbx, rdi
    syscall
    mov rcx, gamma_q_buffer
    mov rdx, 0
    xor rbx, rbx
    xor rax, rax

    gamma_q_loop2:
        mov al, byte [rcx]
        cmp al, 10
        je gamma_q.end_loop2
        imul rdx, 10
        mov bl, al
        sub bl, '0'
        add rdx, rbx
        add rcx, 1
        jmp gamma_q_loop2
    gamma_q.end_loop2:
    mov rax, rdx
    ret
_gamma_q_exception:
    mov rax, -1
    ret
