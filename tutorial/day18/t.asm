section .data
    data:  dd 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
    indices: dd 5, 6, 7, 0, 1, 2, 3, 4
    result: times 8 dd 0.0

section .text
    global _start
_start:
    ; load indices into ymm register
    vmovdqu ymm0, indices

    ; gather data using indices
    vpgatherdps ymm1, data, ymm0, 4

    ; store result
    vmovups result, ymm1

    ; exit program
    xor eax, eax
    ret


