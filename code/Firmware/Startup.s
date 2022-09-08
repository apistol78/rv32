.align 6

.globl _start
.globl _start_no_args

_start_no_args:
    addi    sp, sp, -8
    sw      x10, 0(sp)
    sw      x10, 4(sp)
    j       _start
