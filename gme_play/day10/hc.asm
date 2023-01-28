%include "h.inc"

section .rodata
    ;filename db "/sys/devices/system/cpu/present",0
    filename db "test.txt",0
section .bss
    buffer   resb 20

global GammaHC
section .text

GammaHC:
    ; Open the file
    mov rax, 2
    mov rdi, filename
    mov rsi, 0
    mov rdx, 0644o
    syscall

    mov rdi, rax ; Read the file
    mov rax, 0
    mov rsi, buffer
    mov rdx, 20
    syscall

    ; Close the file
    mov rax, 3
    mov rbx, rdi
    syscall

    ; Parse the file contents to get the number of CPUs
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

