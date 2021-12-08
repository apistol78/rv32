OUTPUT_ARCH( "riscv" )
ENTRY( _start )
SECTIONS
{
    . = 0x200;

    /* text: Code section */
    .text :
    {
        *(.text)
        *(.text.*)
    }

    /* init: Code executed before main. */
    .init :
    {
        KEEP( *(.init) )
    }

    /* fini: Code executed after main. */
    .fini :
    {
        KEEP( *(.fini) )
    }

    /* rodata: Read-only data. */
    .rodata :
    {
        *(.rdata)
        *(.rodata)
        *(.rodata.*)
    }

    . = ALIGN(16);

    /* data: Initialized data segment */
    .data :
    {
        *(.data)
        *(.data.*)
        . = ALIGN(8);
        __global_pointer$ = .;
    }

    . = ALIGN(4);
    _edata = .;
    edata = .;
    
    /* End of uninitalized data segement */
    _end = .;
}
