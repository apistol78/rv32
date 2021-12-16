OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
  rom      (rx) : ORIGIN = 0x00000200, LENGTH = 0x00020000
  ram      (rw) : ORIGIN = 0x00020000, LENGTH = 0x00008000
}

STACK_SIZE = 0x2000;

SECTIONS
{
    /* text: Code section */
    .text :
    {
        *(.text)
        *(.text.*)
    } > rom

    /* init: Code executed before main. */
    .init :
    {
        KEEP( *(.init) )
    } > rom

    /* fini: Code executed after main. */
    .fini :
    {
        KEEP( *(.fini) )
    } > rom

    /* rodata: Read-only data. */
    .rodata :
    {
        *(.rdata)
        *(.rodata)
        *(.rodata.*)
    } > rom

    /* data: Initialized data segment */
    .data :
    {
        _data_rom = LOADADDR(.data);
        _data_ram = ADDR(.data);

        *(.data*);
        *(.sdata*);
        . = ALIGN(16);
        __global_pointer$ = .;

        _data_ram_end = ADDR(.data) + SIZEOF(.data);
    } >ram AT>rom

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
