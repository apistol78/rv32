.text
.global _start

_start:
  # Initialize global pointer
.option push
.option norelax
1:  auipc gp, %pcrel_hi(__global_pointer$)
	addi  gp, gp, %pcrel_lo(1b)
.option pop

	# Copy data and sdata segments to RAM.
	call	__init_data

	# Clear the bss segment
	la      a0, _edata
	la      a2, _end
	sub     a2, a2, a0
	li      a1, 0
	call    memset

	la      a0, __libc_fini_array   		# Register global termination functions
	call    atexit                  		#  to be called upon exit
	call    __libc_init_array       		# Run global initialization functions

	lw      a0, 0(sp)                  		# a0 = argc
	addi    a1, sp, 4 # __SIZEOF_POINTER__ 	# a1 = argv
	li      a2, 0                      		# a2 = envp = NULL
	call    main
	tail    exit
	.size  _start, .-_start

	.global _init
	.global _fini
_init:
_fini:
	# These don't have to do anything since we use init_array/fini_array.
	ret
	.size  _init, .-_init
	.size  _fini, .-_fini