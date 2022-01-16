
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void _start()
{

	__asm__ volatile (
"		bltu s0, s1, jt	\n"
"jf:	li s0, 1		\n"
"		nop				\n"
"jt:	li s0, 2		\n"		
"		nop				\n"
	);

}
