OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
	ram	(rwx)	: ORIGIN = 0x20000000, LENGTH = 0x00100000
	krn	(rw)	: ORIGIN = 0x20040000, LENGTH = 0x00000080
}

SECTIONS
{
	/*--------------------------------------------------------------------*/
	/* Code and read-only segment                                         */
	/*--------------------------------------------------------------------*/

	/* Begining of code and text segment */
	_ftext = .;
	PROVIDE( eprol = . );

	/* text: Program code section */
	.text : 
	{
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
	} >ram

	/* init: Code to execute before main (called by crt0.S) */
	.init : 
	{
		KEEP( *(.init) )
	} >ram

	/* fini: Code to execute after main (called by crt0.S) */
	.fini : 
	{
		KEEP( *(.fini) )
	} >ram

	/* rodata: Read-only data */
	.rodata : 
	{
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r.*)
	} >ram

	/* End of code and read-only segment */
	PROVIDE( etext = . );
	_etext = .;

	/*--------------------------------------------------------------------*/
	/*--------------------------------------------------------------------*/
	.kernal :
	{
		KEEP( *(.kernal) )
	} >krn

	/*--------------------------------------------------------------------*/
	/* Global constructor/destructor segement                             */
	/*--------------------------------------------------------------------*/

	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} >ram

	.init_array     :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array ))
		PROVIDE_HIDDEN (__init_array_end = .);
	} >ram

	.fini_array     :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array ))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >ram

	/*--------------------------------------------------------------------*/
	/* Other misc gcc segments (this was in idt32.ld)                     */
	/*--------------------------------------------------------------------*/
	/* I am not quite sure about these sections but it seems they are for
		 C++ exception handling. I think .jcr is for "Java Class
		 Registration" but it seems to end up in C++ binaries as well. */

	.eh_frame_hdr     : { *(.eh_frame_hdr)     } >ram
	.eh_frame         : { KEEP( *(.eh_frame) ) } >ram
	.gcc_except_table : { *(.gcc_except_table) } >ram
	.jcr              : { KEEP (*(.jcr))       } >ram

	/*--------------------------------------------------------------------*/
	/* Initialized data segment                                           */
	/*--------------------------------------------------------------------*/

	/* Start of initialized data segment */
	. = ALIGN(16);
	 _fdata = .;

	/* data: Writable data */
	.data : 
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d.*)
	} >ram

	/* Have _gp point to middle of sdata/sbss to maximize displacement range */
	. = ALIGN(16);
	_gp = . + 0x800;

	/* Writable small data segment */
	.sdata : 
	{
		*(.sdata)
		*(.sdata.*)
		*(.srodata.*)
		*(.gnu.linkonce.s.*)
	} >ram

	__global_pointer$ = .;

		/* End of initialized data segment */
	PROVIDE( edata = . );
	_edata = .;

	/*--------------------------------------------------------------------*/
	/* Uninitialized data segment                                         */
	/*--------------------------------------------------------------------*/

	/* Start of uninitialized data segment */
	. = ALIGN(8);
	_fbss = .;

	/* Writable uninitialized small data segment */
	.sbss : 
	{
		*(.sbss)
		*(.sbss.*)
		*(.gnu.linkonce.sb.*)
	} >ram

	/* bss: Uninitialized writeable data section */
	. = .;
	_bss_start = .;
	.bss : 
	{
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
	} >ram

	/* End of uninitialized data segment (used by syscalls.c for heap) */
	PROVIDE( end = . );
	_end = ALIGN(8);
}
