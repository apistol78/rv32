
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void main()
{

	__asm__ volatile (
		"li	s0, 0x00010002	\n"
		"sb	s1, -1(s0)		\n"
	);

}
