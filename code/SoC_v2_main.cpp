#include <stdio.h>
#include "VSoC_v2_tb.h"

double sc_time_stamp() { return 0; }

VSoC_v2_tb* create_soc()
{
	VSoC_v2_tb* tb = new VSoC_v2_tb();
	tb->i_reset = 0;
	tb->i_clock = 0;
	tb->eval();
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0xffffffff;
	tb->SoC_v2_tb__DOT__rom__DOT__data[1] = 0xffffffff;
	return tb;
}

void evaluate_single(VSoC_v2_tb* tb)
{
	uint32_t from = tb->SoC_v2_tb__DOT__cpu__DOT__writeback_tag;
	while (tb->SoC_v2_tb__DOT__cpu__DOT__writeback_tag == from)
	{
		tb->i_clock = 1;
		tb->eval();
		tb->i_clock = 0;
		tb->eval();
	}
}

#define ITERATIONS 1000

#define S0 8
#define S1 9
#define S2 18
#define S3 19
#define A2 12

int32_t rnd32()
{
	uint8_t b[] =
	{
		(uint8_t)rand(),
		(uint8_t)rand(),
		(uint8_t)rand(),
		(uint8_t)rand()
	};
	return *(int32_t*)b;
}

uint32_t urnd32()
{
	uint8_t b[] =
	{
		(uint8_t)rand(),
		(uint8_t)rand(),
		(uint8_t)rand(),
		(uint8_t)rand()
	};
	return *(uint32_t*)b;
}

bool verify_ADD()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x01248433; // add	s0,s1,s2

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 + s2))
		return false;

	delete tb;
	return true;
}

bool verify_ADDI()
{
	{
		int32_t s1 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00348413; // addi	s0,s1,3

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 + 3))
			return false;

		delete tb;
	}
	{
		int32_t s1 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0xffd48413; // addi	s0,s1,-3

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 - 3))
			return false;

		delete tb;
	}
	return true;
}

bool verify_AND()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124f433; // and	s0,s1,s2

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 & s2))
		return false;

	delete tb;
	return true;
}

bool verify_ANDI()
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03f4f413; // andi	s0,s1,63

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 & 63))
		return false;

	delete tb;
	return true;
}

bool verify_AUIPC()
{
	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0003f417; // auipc	s0,0x3f

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (0x3f << 12))
		return false;

	delete tb;
	return true;
}

bool verify_BEQ()
{
	return true;
}

bool verify_BGE()
{
	return true;
}

bool verify_BGEU()
{
	return true;
}

bool verify_BLT()
{
	return true;
}

bool verify_BLTU()
{
	return true;
}

bool verify_BNE()
{
	return true;
}

bool verify_DIV()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324c433; // div	s0,s1,s2

	evaluate_single(tb);

	// printf("DIV, %d / %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
		return false;

	delete tb;
	return true;
}

bool verify_UDIV()
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324d433; // divu	s0,s1,s2

	evaluate_single(tb);

	// printf("DIV, %d / %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
		return false;

	delete tb;
	return true;
}

bool verify_JAL()
{
	return true;
}

bool verify_JALR()
{
	return true;
}

bool verify_LB()
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t v = rand() & 255;
		int32_t r = (int32_t)((int8_t)v);

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00400403 + (i << 20); // lb	s0,4(zero) # 4
		tb->SoC_v2_tb__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 8);

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;	
}

bool verify_LBU()
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t v = rand() & 255;
		uint32_t r = (uint32_t)v;

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00404403 + (i << 20); // lbu	s0,4(zero) # 4
		tb->SoC_v2_tb__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 8);

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LH()
{
	for (int i = 0; i < 2; ++i)
	{
		uint16_t v = (uint16_t)urnd32();
		int32_t r = (int32_t)((int16_t)v);

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00401403 + ((i * 2) << 20); // lh	s0,4(zero) # 4
		tb->SoC_v2_tb__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 16);

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LHU()
{
	for (int i = 0; i < 2; ++i)
	{
		uint16_t v = (uint16_t)urnd32();
		uint32_t r = (uint32_t)v;

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00405403 + ((i * 2) << 20); // lhu	s0,4(zero) # 4
		tb->SoC_v2_tb__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 16);

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LUI()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x008ff437; // lui	s0,0x8ff

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != 0x08ff000)
		return false;

	delete tb;
	return true;
}

bool verify_LW()
{
	return true;
}

bool verify_LWU()
{
	return true;
}

bool verify_MUL()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03248433; // mul	s0,s1,s2

	evaluate_single(tb);

	// printf("MUL, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	(int32_t)(s1 * s2)
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 * s2))
		return false;

	delete tb;
	return true;
}

bool verify_MULH()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03249433; // mul	s0,s1,s2

	evaluate_single(tb);

	int32_t r = (int32_t)(((int64_t)s1 * (int64_t)s2) >> 32);

	// printf("MULH, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	r
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_MULHU()
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324b433; // mulhu	s0,s1,s2

	evaluate_single(tb);

	uint32_t r = (uint32_t)(((uint64_t)s1 * (uint64_t)s2) >> 32);

	// printf("MULHU, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	r
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_OR()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124e433; // or	s0,s1,s2

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 | s2))
		return false;

	delete tb;
	return true;
}

bool verify_ORI()
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03f4e413; // ori	s0,s1,63

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 | 63))
		return false;

	delete tb;
	return true;
}

bool verify_REM()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324e433; // rem	s0,s1,s2

	evaluate_single(tb);

	// printf("REM, %d %% %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
		return false;

	delete tb;
	return true;
}

bool verify_REMU()
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324f433; // remu	s0,s1,s2

	evaluate_single(tb);

	// printf("REMU, %d %% %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
		return false;

	delete tb;
	return true;
}

bool verify_SB()
{
	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__ram__DOT__data[0] = 0x00000000;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = 0x12;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00010437; // lui	s0,0x10
	tb->SoC_v2_tb__DOT__rom__DOT__data[1] = 0x00240413; // addi	s0,s0,2 # 10002 
	tb->SoC_v2_tb__DOT__rom__DOT__data[2] = 0xfe940fa3; // sb	s1,-1(s0)

	evaluate_single(tb);
	evaluate_single(tb);
	evaluate_single(tb);

	// printf("%08x\n", tb->SoC_v2_tb__DOT__ram__DOT__data[0]);

	if (tb->SoC_v2_tb__DOT__ram__DOT__data[0] != 0x00001200)
		return false;

	delete tb;
	return true;
}

bool verify_SH()
{
	return true;
}

bool verify_SLL()
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = s1 << (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x01249433; // sll	s0,s1,s2

	evaluate_single(tb);

	// printf("%08x << (%08x, %u) = %08x\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLLI()
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00549413; // slli	s0,s1,0x5

	evaluate_single(tb);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 << 5))
		return false;

	delete tb;
	return true;
}

bool verify_SLT()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();
	int32_t r = (s1 < s2) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124a433; // slt	s0,s1,s2

	evaluate_single(tb);

	// printf("%d < %d = %d\n", s1, s2, r);
	// printf("%d\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTI()
{
	int32_t s1 = rnd32();
	int32_t r = (s1 < 5) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0054a413; // slti	s0,s1,5

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTIU()
{
	uint32_t s1 = urnd32();
	uint32_t r = (s1 < 5) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0054b413; // sltiu	s0,s1,5

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTU()
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = (s1 < s2) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124b433; // sltu	s0,s1,s2

	evaluate_single(tb);

	// printf("%d < %d = %d\n", s1, s2, r);
	// printf("%d\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRA()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();
	int32_t r = s1 >> (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x4124d433; // sra	s0,s1,s2

	evaluate_single(tb);

	// printf("%d >> (%d, %u) = %d\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %d\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRAI()
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x4054d413; // srai	s0,s1,0x5

	evaluate_single(tb);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 >> 5))
		return false;

	delete tb;
	return true;
}

bool verify_SRL()
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = s1 >> (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124d433; // srl	s0,s1,s2

	evaluate_single(tb);

	// printf("%d >> (%d, %u) = %d\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %d\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRLI()
{
	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[A2] = 0x99999998;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[A2] = 0x99999998;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00165613; // srli	a2,a2,0x1

	evaluate_single(tb);

	// printf("%08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[A2]);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[A2] != (0x99999998 >> 1))
		return false;

	delete tb;

/*
	uint32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0054d413; // srli	s0,s1,0x5

	evaluate_single(tb);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 >> 5))
		return false;

	delete tb;
*/
	return true;
}

bool verify_SUB()
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x41248433; // sub	s0,s1,s2

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 - s2))
		return false;

	delete tb;
	return true;
}

bool verify_SW()
{
	return true;
}

bool verify_XOR()
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0124c433; // xor	s0,s1,s2

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 ^ s2))
		return false;

	delete tb;
	return true;
}

bool verify_XORI()
{
	uint32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03f4c413; // xori	s0,s1,63

	evaluate_single(tb);

	if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 ^ 63))
		return false;

	delete tb;
	return true;
}

bool verify_ENDIAN()
{
	{
		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S3] = 0;

		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x000102b7; // lui	t0,0x10
		tb->SoC_v2_tb__DOT__rom__DOT__data[1] = 0x11223437; // lui	s0,0x11223
		tb->SoC_v2_tb__DOT__rom__DOT__data[2] = 0x34440413;	// addi	s0,s0,836 # 11223344
		tb->SoC_v2_tb__DOT__rom__DOT__data[3] = 0x0082a023;	// sw	s0,0(t0) # 0
		tb->SoC_v2_tb__DOT__rom__DOT__data[4] = 0x00028403;	// lb	s0,0(t0) # 0
		tb->SoC_v2_tb__DOT__rom__DOT__data[5] = 0x00128483;	// lb	s1,1(t0) # 0
		tb->SoC_v2_tb__DOT__rom__DOT__data[6] = 0x00228903;	// lb	s2,2(t0) # 0
		tb->SoC_v2_tb__DOT__rom__DOT__data[7] = 0x00328983;	// lb	s3,3(t0) # 0

		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);
		evaluate_single(tb);

		// printf("S0 %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0]);
		// printf("S1 %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1]);
		// printf("S2 %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2]);
		// printf("S3 %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S3]);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != 0x44)
			return false;
		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] != 0x33)
			return false;
		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] != 0x22)
			return false;
		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S3] != 0x11)
			return false;

		delete tb;
	}

	return true;
}

// ========================================================

bool verify(bool (*fn)())
{
	for (int i = 0; i < ITERATIONS; ++i)
	{
		if (!fn())
			return false;
		;
	}
	return true;
}

#define CHECK(vfn) \
	if (verify(vfn)) { printf("%s SUCCEEDED!\n", #vfn); } \
	else { printf("%s FAILED!\n", #vfn); success = false; }

int main(int argc, char **argv)
{
	bool success = true;

	CHECK(verify_ADD);
	CHECK(verify_ADDI);
	CHECK(verify_AND);
	CHECK(verify_ANDI);
	CHECK(verify_AUIPC);
	CHECK(verify_BEQ);
	CHECK(verify_BGE);
	CHECK(verify_BGEU);
	CHECK(verify_BLT);
	CHECK(verify_BLTU);
	CHECK(verify_BNE);
	CHECK(verify_DIV);
	CHECK(verify_UDIV);
	CHECK(verify_JAL);
	CHECK(verify_JALR);
	CHECK(verify_LB);
	CHECK(verify_LBU);
	CHECK(verify_LH);
	CHECK(verify_LHU);
	CHECK(verify_LUI);
	CHECK(verify_LW);
	CHECK(verify_LWU);
	CHECK(verify_MUL);
	CHECK(verify_MULH);
	CHECK(verify_MULHU);
	CHECK(verify_OR);
	CHECK(verify_ORI);
	CHECK(verify_REM);
	CHECK(verify_REMU);
	CHECK(verify_SB);
	CHECK(verify_SH);
	CHECK(verify_SLL);
	CHECK(verify_SLLI);
	CHECK(verify_SLT);
	CHECK(verify_SLTI);
	CHECK(verify_SLTIU);
	CHECK(verify_SLTU);
	CHECK(verify_SRA);
	CHECK(verify_SRAI);
	CHECK(verify_SRL);
	CHECK(verify_SRLI);
	CHECK(verify_SUB);
	CHECK(verify_SW);
	CHECK(verify_XOR);
	CHECK(verify_XORI);
	CHECK(verify_ENDIAN);

	if (success)
		printf("SUCCESS!\n");
	else
		printf("FAILED!\n");

	return 0;
}