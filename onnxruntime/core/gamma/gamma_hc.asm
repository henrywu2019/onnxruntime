section .rodata
    f db 2FH, 73H, 79H, 73H, 2FH, 64H, 65H, 76H
      db 69H, 63H, 65H, 73H, 2FH, 73H, 79H, 73H
      db 74H, 65H, 6DH, 2FH, 63H, 70H, 75H, 2FH
      db 70H, 72H, 65H, 73H, 65H, 6EH, 74H, 00H

section .bss
    buffer   resb 8

global _gamma_hc
section .text

_gamma_hc:
    mov rax, 2
    mov rdi, f
    mov rsi, 0
    mov rdx, 0644o
    syscall

    cmp rax, 0
    jg good
    mov rax, -1
    ret
good:
    mov rdi, rax
    mov rax, 0
    mov rsi, buffer
    mov rdx, 8
    syscall
    mov rax, 3
    mov rbx, rdi
    syscall
    mov rcx, buffer
    mov rdx, 0
    xor rbx, rbx
    xor rax, rax
    loop1:
        mov al, byte [rcx]
        cmp al, '-'
        je end_loop1
        add rcx, 1
        jmp loop1
    end_loop1:
    add rcx, 1

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
    add rax, 1
    ret
