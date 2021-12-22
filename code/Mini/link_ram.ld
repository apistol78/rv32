OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
  ram      (rw) : ORIGIN = 0x10000000, LENGTH = 0x10000000
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

    /* data: Initialized data segment */
    .data :
    {
        *(.data*);
        *(.sdata*);
        . = ALIGN(16);
        __global_pointer$ = .;
    } > ram

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        *(.bss*)
    } > ram

    . = ALIGN(4);
    _edata = .;
    edata = .;

/*
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
    } > ram
*/

    /* End of uninitalized data segement */
    .heap (NOLOAD):
    {
        _end = .;
    } >ram
}
