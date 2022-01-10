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
	while (tb->SoC_v2_tb__DOT__cpu__DOT__writeback_tag == 0)
	{
		tb->i_clock = 1;
		tb->eval();
		tb->i_clock = 0;
		tb->eval();
	}
}

#define S0 8
#define S1 9
#define S2 18

int32_t rnd32()
{
	unsigned char b[] =
	{
		rand(),
		rand(),
		rand(),
		rand()
	};
	return *(int32_t*)b;
}

uint32_t urnd32()
{
	unsigned char b[] =
	{
		rand(),
		rand(),
		rand(),
		rand()
	};
	return *(uint32_t*)b;
}

bool verify_ADD()
{
	for (int i = 0; i < 100; ++i)
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
	}
	return true;
}

bool verify_ADDI()
{
	for (int i = 0; i < 100; ++i)
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
	for (int i = 0; i < 100; ++i)
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
	for (int i = 0; i < 100; ++i)
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
	}
	return true;
}

bool verify_ANDI()
{
	for (int i = 0; i < 100; ++i)
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
	}
	return true;
}

bool verify_AUIPC()
{
	for (int i = 0; i < 100; ++i)
	{
		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0003f417; // auipc	s0,0x3f

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (0x3f << 12))
			return false;

		delete tb;
	}
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
	for (int i = 0; i < 100; ++i)
	{
		int32_t s1 = rnd32();
		int32_t s2 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324c433; // div	s0,s1,s2

		evaluate_single(tb);

		printf("DIV, %d / %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			s1 / s2
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
			return false;

		delete tb;
	}
	return true;
}

bool verify_UDIV()
{
	for (int i = 0; i < 100; ++i)
	{
		uint32_t s1 = urnd32();
		uint32_t s2 = urnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324d433; // divu	s0,s1,s2

		evaluate_single(tb);

		printf("DIV, %d / %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			s1 / s2
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
			return false;

		delete tb;
	}
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
	return true;
}

bool verify_LBU()
{
	return true;
}

bool verify_LH()
{
	return true;
}

bool verify_LHU()
{
	return true;
}

bool verify_LUI()
{
	for (int i = 0; i < 100; ++i)
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
	}
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
	for (int i = 0; i < 100; ++i)
	{
		int32_t s1 = rnd32();
		int32_t s2 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x03248433; // mul	s0,s1,s2

		evaluate_single(tb);

		printf("MUL, %d * %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			(int32_t)(s1 * s2)
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 * s2))
			return false;

		delete tb;
	}
	return true;
}

bool verify_MULH()
{
	for (int i = 0; i < 100; ++i)
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

		printf("MULH, %d * %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			r
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}
	return true;
}

bool verify_MULHU()
{
	for (int i = 0; i < 100; ++i)
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

		printf("MULHU, %d * %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			r
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}
	return true;
}

bool verify_OR()
{
	for (int i = 0; i < 100; ++i)
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
	}
	return true;
}

bool verify_ORI()
{
	for (int i = 0; i < 100; ++i)
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
	}
	return true;
}

bool verify_REM()
{
	for (int i = 0; i < 100; ++i)
	{
		int32_t s1 = rnd32();
		int32_t s2 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324e433; // rem	s0,s1,s2

		evaluate_single(tb);

		printf("REM, %d %% %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			s1 / s2
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
			return false;

		delete tb;
	}
	return true;
}

bool verify_REMU()
{
	for (int i = 0; i < 100; ++i)
	{
		uint32_t s1 = urnd32();
		uint32_t s2 = urnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x0324f433; // remu	s0,s1,s2

		evaluate_single(tb);

		printf("REMU, %d %% %d = %d (%d)\n",
			s1,
			s2,
			tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0],
			s1 / s2
		);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
			return false;

		delete tb;
	}
	return true;
}

bool verify_SB()
{
	return true;
}

bool verify_SH()
{
	return true;
}

bool verify_SLL()
{
	for (int i = 0; i < 100; ++i)
	{
		int32_t s1 = rnd32();
		int32_t s2 = rnd32() % 32;

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x01249433; // or	sll	s0,s1,s2

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 << s2))
			return false;

		delete tb;
	}
	return true;
}

bool verify_SLLI()
{
	for (int i = 0; i < 100; ++i)
	{
		int32_t s1 = rnd32();

		auto tb = create_soc();
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC_v2_tb__DOT__rom__DOT__data[0] = 0x00549413; // slli	s0,s1,0x5

		evaluate_single(tb);

		if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 << 5))
			return false;

		delete tb;
	}
	return true;
}

#define CHECK(vfn) \
	if (!vfn()) { printf("%s FAILED!\n", #vfn); success = false; }

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

	if (success)
		printf("SUCCESS!\n");

	return 0;
}