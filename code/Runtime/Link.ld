OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
	ram	(rwx)	: ORIGIN = 0x20100000, LENGTH = 0x01000000
}

SECTIONS
{
	/*--------------------------------------------------------------------*/
	/* Code and read-only segment                                         */
	/*--------------------------------------------------------------------*/

	. = ORIGIN(ram);

	_ftext = .;
	PROVIDE( eprol = . );

	/* text: Program code section */
	.text : 
	{
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
	}

	/* init: Code to execute before main (called by crt0.S) */
	.init : 
	{
		KEEP( *(.init) )
	}

	/* fini: Code to execute after main (called by crt0.S) */
	.fini : 
	{
		KEEP( *(.fini) )
	}

	/* rodata: Read-only data */
	.rodata : 
	{
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r.*)
	}

	/* End of code and read-only segment */
	PROVIDE( etext = . );
	_etext = .;

	/*--------------------------------------------------------------------*/
	/* Global constructor/destructor segement                             */
	/*--------------------------------------------------------------------*/

	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	}

	.init_array     :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array ))
		PROVIDE_HIDDEN (__init_array_end = .);
	}

	.fini_array     :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array ))
		PROVIDE_HIDDEN (__fini_array_end = .);
	}

	/*--------------------------------------------------------------------*/
	/* Other misc gcc segments (this was in idt32.ld)                     */
	/*--------------------------------------------------------------------*/
	/* I am not quite sure about these sections but it seems they are for
		 C++ exception handling. I think .jcr is for "Java Class
		 Registration" but it seems to end up in C++ binaries as well. */

	.eh_frame_hdr     : { *(.eh_frame_hdr)     }
	.eh_frame         : { KEEP( *(.eh_frame) ) }
	.gcc_except_table : { *(.gcc_except_table) }
	.jcr              : { KEEP (*(.jcr))       } 

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
	}

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
	}

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
	}

	/* bss: Uninitialized writeable data section */
	. = .;
	__bss_start = .;
	.bss : 
	{
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
	}

	/* End of uninitialized data segment (used by syscalls.c for heap) */
	PROVIDE( end = . );
	_end = ALIGN(8);
}
