OUTPUT_ARCH( "riscv" )
ENTRY( _start )
SECTIONS
{
    . = 0x000;

    /* text: Code section */
    .text :
    {
        *(.text)
        *(.text.*)
    }
    
    /* End of uninitalized data segement */
    _end = .;
}
