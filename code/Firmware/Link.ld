OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
  rom      (rx) : ORIGIN = 0x00000000, LENGTH = 0x00010000
  ram      (rw) : ORIGIN = 0x10000000, LENGTH = 0x00010000
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

    /* End of uninitalized data segement */
    .heap (NOLOAD):
    {
        _end = .;
    } >ram
}
