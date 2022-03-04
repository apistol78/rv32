#include "Runtime/HAL/CSR.h"
#include "Runtime/HAL/Interrupt.h"

enum
{
	RISCV_INT_POS_MSI = 3,
	RISCV_INT_POS_MTI = 7,
	RISCV_INT_POS_MEI = 11,
	RISCV_INT_POS_SSI = 1,
	RISCV_INT_POS_STI = 5,
	RISCV_INT_POS_SEI = 9,
	RISCV_INT_POS_USI = 0,
	RISCV_INT_POS_UTI = 4,
	RISCV_INT_POS_UEI = 8,
};

enum
{
	RISCV_INT_MASK_MSI = (1UL << RISCV_INT_POS_MSI),
	RISCV_INT_MASK_MTI = (1UL << RISCV_INT_POS_MTI),
	RISCV_INT_MASK_MEI = (1UL << RISCV_INT_POS_MEI),
	RISCV_INT_MASK_SSI = (1UL << RISCV_INT_POS_SSI),
	RISCV_INT_MASK_STI = (1UL << RISCV_INT_POS_STI),
	RISCV_INT_MASK_SEI = (1UL << RISCV_INT_POS_SEI),
	RISCV_INT_MASK_USI = (1UL << RISCV_INT_POS_USI),
	RISCV_INT_MASK_UTI = (1UL << RISCV_INT_POS_UTI),
	RISCV_INT_MASK_UEI = (1UL << RISCV_INT_POS_UEI),
};

enum
{
	RISCV_EXCP_INSTRUCTION_ADDRESS_MISALIGNED = 0,	/* Instruction address misaligned */
	RISCV_EXCP_INSTRUCTION_ACCESS_FAULT = 1,		/* Instruction access fault	*/
	RISCV_EXCP_ILLEGAL_INSTRUCTION = 2,				/* Illegal instruction */
	RISCV_EXCP_BREAKPOINT = 3,						/* Breakpoint */
	RISCV_EXCP_LOAD_ADDRESS_MISALIGNED = 4,			/* Load address misaligned */
	RISCV_EXCP_LOAD_ACCESS_FAULT = 5,				/* Load access fault */
	RISCV_EXCP_STORE_AMO_ADDRESS_MISALIGNED	= 6,	/* Store/AMO address misaligned	 */
	RISCV_EXCP_STORE_AMO_ACCESS_FAULT = 7,			/* Store/AMO access fault */
	RISCV_EXCP_ENVIRONMENT_CALL_FROM_U_MODE = 8,	/* Environment call from U-mode	*/
	RISCV_EXCP_ENVIRONMENT_CALL_FROM_S_MODE = 9,	/* Environment call from S-mode	*/
	RISCV_EXCP_RESERVED10 = 10,						/* Reserved	*/
	RISCV_EXCP_ENVIRONMENT_CALL_FROM_M_MODE = 11,	/* Environment call from M-mode	*/
	RISCV_EXCP_INSTRUCTION_PAGE_FAULT = 12,			/* Instruction page fault */
	RISCV_EXCP_LOAD_PAGE_FAULT = 13,				/* Load page fault */
	RISCV_EXCP_RESERVED14 = 14,						/* Reserved	*/
	RISCV_EXCP_STORE_AMO_PAGE_FAULT = 15,			/* Store/AMO page fault */
};

#define PLIC_ENABLE		(volatile uint32_t*)0xb0002000
#define PLIC_CLAIM_0	(volatile uint32_t*)0xb0200004
#define PLIC_COMPLETE_0	(volatile uint32_t*)0xb0200004

irq_handler_t g_handlers[] = { 0, 0, 0, 0, 0 };

#pragma GCC push_options
#pragma GCC optimize ("align-functions=4")

static void irq_entry() __attribute__ ((interrupt ("machine")));

static void irq_entry()
{
	uint32_t cause = csr_read_mcause();
	if (cause & MCAUSE_INTERRUPT_BIT_MASK)
	{
		cause &= 0xffff;
		if (cause == RISCV_INT_MASK_MTI)
		{
			// Timer interrupt.
			irq_handler_t handler = g_handlers[0];
			if (handler)
				handler();
		}
		else if (cause == RISCV_INT_MASK_MEI)
		{
			// External interrupt, read PLIC to determine source.
			uint32_t claimed = *PLIC_CLAIM_0;
			if (claimed)
			{
				// Call appropriate handler for source.
				irq_handler_t handler = g_handlers[claimed];
				if (handler)
					handler();
			}
			*PLIC_COMPLETE_0 = claimed;
		}
	}
	else	// Software interrupt.
	{
		// \hack Call timer interrupt handler, kernel scheduler.
		irq_handler_t handler = g_handlers[0];
		if (handler)
			handler();
	}
}

#pragma GCC pop_options

void interrupt_init()
{
	// Global interrupt disable
	csr_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK);
	csr_write_mie(0);

	// Setup the IRQ handler entry point
	csr_write_mtvec((uint_xlen_t)irq_entry);

	// Global interrupt enable 
	csr_set_bits_mie(MIE_MTI_BIT_MASK | MIE_MEI_BIT_MASK | MIE_MSI_BIT_MASK);
	csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);

	// Enable PLIC interrupts.
	*PLIC_ENABLE = 2 | 4 | 8 | 16;
}

void interrupt_set_handler(uint32_t source, irq_handler_t handler)
{
	g_handlers[source] = handler;
}

void interrupt_enable()
{
	csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);
}

void interrupt_disable()
{
	csr_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK);	
}
