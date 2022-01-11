OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
	ram		(rw) : ORIGIN = 0x20000000, LENGTH = 0x20000000
}

STACK_SIZE = 0x2000;

SECTIONS
{
	/* text: Code section */
	.text :
	{
		*(.text)
		*(.text.*)
	} > ram

	/* init: Code executed before main. */
	.init :
	{
		KEEP( *(.init) )
	} > ram

	/* fini: Code executed after main. */
	.fini :
	{
		KEEP( *(.fini) )
	} > ram

	/* rodata: Read-only data. */
	.rodata :
	{
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
	} > ram

    . = ALIGN(4);
    _etext = .;

	/* data: Initialized data segment */
	.data :
	{
		*(.data*);
		*(.sdata*);
		. = ALIGN(16);
		__global_pointer$ = .;
	} > ram

	. = ALIGN(4);
	_edata = .;
	edata = .;

	/* .bss section which is used for uninitialized data */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = . ;
		_szero = .;
		*(.bss .bss.*)
		*(.sbss .sbss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = . ;
		_ezero = .;
	} > ram

	/* End of uninitalized data segement */
	.heap (NOLOAD):
	{
		_end = .;
	} >ram
}
