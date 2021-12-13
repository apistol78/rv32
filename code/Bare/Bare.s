.align 6
.globl _start

_start:
    call    main
    #sw      sp, 0(sp)
    #lw	    s2, 0(sp)
    j       _start
