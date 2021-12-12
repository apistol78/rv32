.align 6
.globl _start

_start:
    addi    a1, a1, 1
    addi    a2, a2, 2
    addi    a3, a3, 3
    j       _start
