.align 6
.globl _start

_start:
#   la sp, __stack_top
    add s0, sp, zero
    jal zero, main

_end:
    j _end
