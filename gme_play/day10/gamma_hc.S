.file "henry.c"
.text
.section .rodata
    f db "/sys/devices/system/cpu/present",0

.section .bss
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

