#include <stdio.h>
#include <Core/Log/Log.h>
#include "verilated.h"
#include "verilated_fst_c.h"
#include "SoC/VSoC.h"

using namespace traktor;

VSoC* create_soc()
{
	VSoC* tb = new VSoC();
	tb->CPU_RESET_n = 1;
	tb->CLOCK_125_p = 0;
	tb->eval();

	// Fill rom with NOP since prefetch might read more instructions
	// than we evaluate.
	for (int i = 0; i < 256; ++i)
		tb->SoC__DOT__rom__DOT__data[i] = 0x00000013;

	return tb;
}

void evaluate(VSoC* tb, const char* trace, int32_t steps)
{
	//const std::unique_ptr< VerilatedContext > contextp{new VerilatedContext()};
	
	VerilatedFstC* tfp = nullptr;
	if (trace)
	{
		Verilated::traceEverOn(true);
		tfp = new VerilatedFstC;
		tb->trace(tfp, 99);  // Trace 99 levels of hierarchy
		tfp->open(trace); // "simx.vcd");
	}

	int32_t time = 0;

	for (int32_t i = 0; i < steps; ++i)
	{
		const uint32_t from = tb->SoC__DOT__cpu__DOT__writeback__DOT__retired;
		while (tb->SoC__DOT__cpu__DOT__writeback__DOT__retired == from)
		{
			//contextp->timeInc(1);
			++time;
			
			tb->CLOCK_125_p = 1;
			tb->eval();

			if (tfp)
				tfp->dump(time); //contextp->time());

			++time; // contextp->timeInc(1);
			tb->CLOCK_125_p = 0;
			tb->eval();

			if (tfp)
				tfp->dump(time); //contextp->time());

			if (time > 1000)
			{
				log::info << L"Unable to execute, seems stuck!" << Endl;
				break;
			}
		}
	}

	if (tfp)
	{
		tfp->close();
		delete tfp;
	}
}

#define ITERATIONS 100

#define S0 8
#define S1 9
#define S2 18
#define S3 19
#define A2 12
#define A5 15
#define RA 1

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

float rndf()
{
	return (2.0f * (rand() % 32767)) / 32767.0f - 1.0f;
}

bool verify_ADD(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x01248433; // add	s0,s1,s2

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 + s2))
		return false;

	delete tb;
	return true;
}

bool verify_ADDI(const char* trace)
{
	{
		int32_t s1 = rnd32();

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC__DOT__rom__DOT__data[0] = 0x00348413; // addi	s0,s1,3

		evaluate(tb, trace, 1);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 + 3))
			return false;

		delete tb;
	}
	{
		int32_t s1 = rnd32();

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
		tb->SoC__DOT__rom__DOT__data[0] = 0xffd48413; // addi	s0,s1,-3

		evaluate(tb, trace, 1);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 - 3))
			return false;

		delete tb;
	}
	return true;
}

bool verify_AND(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124f433; // and	s0,s1,s2

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 & s2))
		return false;

	delete tb;
	return true;
}

bool verify_ANDI(const char* trace)
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x03f4f413; // andi	s0,s1,63

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 & 63))
		return false;

	delete tb;
	return true;
}

bool verify_AUIPC(const char* trace)
{
	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0003f417; // auipc	s0,0x3f

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (0x3f << 12))
		return false;

	delete tb;
	return true;
}

bool verify_BEQ(const char* trace)
{
	uint32_t a = rand() & 15;
	uint32_t b = rand() & 15;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00940663; // beq	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("PC: %08x\n", tb->SoC__DOT__cpu__DOT__fetch_pc);
	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a == b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_BGE(const char* trace)
{
	int32_t a = (int32_t)(rand() & 15) - 7;
	int32_t b = (int32_t)(rand() & 15) - 7;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00945663; // bge	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a >= b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_BGEU(const char* trace)
{
	uint32_t a = rand() & 15;
	uint32_t b = rand() & 15;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00947663; // bgeu	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a >= b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_BLT(const char* trace)
{
	int32_t a = (int32_t)(rand() & 15) - 7;
	int32_t b = (int32_t)(rand() & 15) - 7;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00944663; // blt	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a < b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_BLTU(const char* trace)
{
	uint32_t a = rand() & 15;
	uint32_t b = rand() & 15;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00946663; // bltu	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a < b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_BNE(const char* trace)
{
	uint32_t a = rand() & 15;
	uint32_t b = rand() & 15;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = a;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = b;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00941663; // bne	s0,s1,c <jt>	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x00100413; // li	s0,1			4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop					8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200413; // li	s0,2			c
	tb->SoC__DOT__rom__DOT__data[4] = 0x00000013; // nop					10

	evaluate(tb, trace, 3);

	// printf("%08x != %08x -> %d\n", a, b, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (a != b)
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x10)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 2)
			return false;
	}
	else
	{
		// if (tb->SoC__DOT__cpu__DOT__fetch_pc != 0x08)
		// 	return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
			return false;
	}

	delete tb;
	return true;
}

bool verify_DIV(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0324c433; // div	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("DIV, %d / %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
		return false;

	delete tb;
	return true;
}

bool verify_UDIV(const char* trace)
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0324d433; // divu	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("DIV, %d / %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 / s2))
		return false;

	delete tb;
	return true;
}

bool verify_JAL(const char* trace)
{
	return true;
}

bool verify_JALR(const char* trace)
{
	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00c00413;	// li	s0,12	0
	tb->SoC__DOT__rom__DOT__data[1] = 0x000400e7;	// jalr	s0		4
	tb->SoC__DOT__rom__DOT__data[2] = 0x00100493;	// li	s1,1	8
	tb->SoC__DOT__rom__DOT__data[3] = 0x00200493;	// li	s1,2	c

	evaluate(tb, trace, 3);

	// printf("RA %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[RA]);
	// printf("S1 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1]);
	// printf("PC %08x\n", tb->SoC__DOT__cpu__DOT__fetch__DOT__pc);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[RA] != 0x00000008)
		return false;	

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] != 2)
		return false;	

	// if (tb->SoC__DOT__cpu__DOT__fetch__DOT__pc != 0x00000010)
	// 	return false;	

	delete tb;
	return true;
}

bool verify_LB(const char* trace)
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t v = rand() & 255;
		int32_t r = (int32_t)((int8_t)v);

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__rom__DOT__data[0] = 0x00400403 + (i << 20); // lb	s0,4(zero) # 4
		tb->SoC__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 8);

		evaluate(tb, trace, 1);

		// printf("%d. %d\n", i, tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;	
}

bool verify_LBU(const char* trace)
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t v = rand() & 255;
		uint32_t r = (uint32_t)v;

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__rom__DOT__data[0] = 0x00404403 + (i << 20); // lbu	s0,4(zero) # 4
		tb->SoC__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 8);

		evaluate(tb, trace, 1);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LH(const char* trace)
{
	for (int i = 0; i < 2; ++i)
	{
		uint16_t v = (uint16_t)urnd32();
		int32_t r = (int32_t)((int16_t)v);

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__rom__DOT__data[0] = 0x00401403 + ((i * 2) << 20); // lh	s0,4(zero) # 4
		tb->SoC__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 16);

		evaluate(tb, trace, 1);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LHU(const char* trace)
{
	for (int i = 0; i < 2; ++i)
	{
		uint16_t v = (uint16_t)urnd32();
		uint32_t r = (uint32_t)v;

		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__rom__DOT__data[0] = 0x00405403 + ((i * 2) << 20); // lhu	s0,4(zero) # 4
		tb->SoC__DOT__rom__DOT__data[1] = ((uint32_t)v) << (i * 16);

		evaluate(tb, trace, 1);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
			return false;

		delete tb;
	}

	return true;
}

bool verify_LUI(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__rom__DOT__data[0] = 0x008ff437; // lui	s0,0x8ff

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0x08ff000)
		return false;

	delete tb;
	return true;
}

bool verify_LW(const char* trace)
{
	return true;
}

bool verify_MUL(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x03248433; // mul	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("MUL, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	(int32_t)(s1 * s2)
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 * s2))
		return false;

	delete tb;
	return true;
}

bool verify_MULH(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x03249433; // mul	s0,s1,s2

	evaluate(tb, trace, 1);

	int32_t r = (int32_t)(((int64_t)s1 * (int64_t)s2) >> 32);

	// printf("MULH, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	r
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_MULHU(const char* trace)
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0324b433; // mulhu	s0,s1,s2

	evaluate(tb, trace, 1);

	uint32_t r = (uint32_t)(((uint64_t)s1 * (uint64_t)s2) >> 32);

	// printf("MULHU, %d * %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	r
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_OR(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124e433; // or	s0,s1,s2

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 | s2))
		return false;

	delete tb;
	return true;
}

bool verify_ORI(const char* trace)
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x03f4e413; // ori	s0,s1,63

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 | 63))
		return false;

	delete tb;
	return true;
}

bool verify_REM(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0324e433; // rem	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("REM, %d %% %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
		return false;

	delete tb;
	return true;
}

bool verify_REMU(const char* trace)
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0324f433; // remu	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("REMU, %d %% %d = %d (%d)\n",
	// 	s1,
	// 	s2,
	// 	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0],
	// 	s1 / s2
	// );

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 % s2))
		return false;

	delete tb;
	return true;
}

bool verify_SB(const char* trace)
{
	auto tb = create_soc();
	tb->SoC__DOT__ram__DOT__data[0] = 0x00000000;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = 0x12;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00010437; // lui	s0,0x10
	tb->SoC__DOT__rom__DOT__data[1] = 0x00240413; // addi	s0,s0,2 # 10002 
	tb->SoC__DOT__rom__DOT__data[2] = 0xfe940fa3; // sb		s1,-1(s0)

	evaluate(tb, trace, 3);

	// printf("%08x\n", tb->SoC__DOT__ram__DOT__data[0]);

	if (tb->SoC__DOT__ram__DOT__data[0] != 0x00001200)
		return false;

	delete tb;
	return true;
}

bool verify_SH(const char* trace)
{
	return true;
}

bool verify_SLL(const char* trace)
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = s1 << (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x01249433; // sll	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("%08x << (%08x, %u) = %08x\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLLI(const char* trace)
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00549413; // slli	s0,s1,0x5

	evaluate(tb, trace, 1);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 << 5))
		return false;

	delete tb;
	return true;
}

bool verify_SLT(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();
	int32_t r = (s1 < s2) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124a433; // slt	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("%d < %d = %d\n", s1, s2, r);
	// printf("%d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTI(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t r = (s1 < 5) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0054a413; // slti	s0,s1,5

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTIU(const char* trace)
{
	uint32_t s1 = urnd32();
	uint32_t r = (s1 < 5) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0054b413; // sltiu	s0,s1,5

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SLTU(const char* trace)
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = (s1 < s2) ? 1 : 0;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124b433; // sltu	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("%d < %d = %d\n", s1, s2, r);
	// printf("%d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRA(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();
	int32_t r = s1 >> (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x4124d433; // sra	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("%d >> (%d, %u) = %d\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRAI(const char* trace)
{
	int32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x4054d413; // srai	s0,s1,0x5

	evaluate(tb, trace, 1);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 >> 5))
		return false;

	delete tb;
	return true;
}

bool verify_SRL(const char* trace)
{
	uint32_t s1 = rnd32();
	uint32_t s2 = rnd32();
	uint32_t r = s1 >> (s2 & 0x1f);

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124d433; // srl	s0,s1,s2

	evaluate(tb, trace, 1);

	// printf("%d >> (%d, %u) = %d\n", s1, s2, (s2 & 0x1f), r);
	// printf("CPU %d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != r)
		return false;

	delete tb;
	return true;
}

bool verify_SRLI(const char* trace)
{
	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[A2] = 0x99999998;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[A2] = 0x99999998;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00165613; // srli	a2,a2,0x1

	evaluate(tb, trace, 1);

	// printf("%08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[A2]);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[A2] != (0x99999998 >> 1))
		return false;

	delete tb;

/*
	uint32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0054d413; // srli	s0,s1,0x5

	evaluate(tb, trace, 1);

	// printf("%d -- %d\n", s1 << 5,
	// tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 >> 5))
		return false;

	delete tb;
*/
	return true;
}

bool verify_SUB(const char* trace)
{
	int32_t s1 = rnd32();
	int32_t s2 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x41248433; // sub	s0,s1,s2

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 - s2))
		return false;

	delete tb;
	return true;
}

bool verify_SW(const char* trace)
{
	return true;
}

bool verify_XOR(const char* trace)
{
	uint32_t s1 = urnd32();
	uint32_t s2 = urnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = s2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0124c433; // xor	s0,s1,s2

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 ^ s2))
		return false;

	delete tb;
	return true;
}

bool verify_XORI(const char* trace)
{
	uint32_t s1 = rnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = s1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x03f4c413; // xori	s0,s1,63

	evaluate(tb, trace, 1);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != (s1 ^ 63))
		return false;

	delete tb;
	return true;
}

bool verify_ENDIAN(const char* trace)
{
	{
		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3] = 0;

		tb->SoC__DOT__rom__DOT__data[0] = 0x100002b7; // lui	t0,0x10000
		tb->SoC__DOT__rom__DOT__data[1] = 0x11223437; // lui	s0,0x11223
		tb->SoC__DOT__rom__DOT__data[2] = 0x34440413;	// addi	s0,s0,836 # 11223344
		tb->SoC__DOT__rom__DOT__data[3] = 0x0082a023;	// sw	s0,0(t0) # 0
		tb->SoC__DOT__rom__DOT__data[4] = 0x00028403;	// lb	s0,0(t0) # 0
		tb->SoC__DOT__rom__DOT__data[5] = 0x00128483;	// lb	s1,1(t0) # 0
		tb->SoC__DOT__rom__DOT__data[6] = 0x00228903;	// lb	s2,2(t0) # 0
		tb->SoC__DOT__rom__DOT__data[7] = 0x00328983;	// lb	s3,3(t0) # 0

		evaluate(tb, trace, 8);

		// printf("S0 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);
		// printf("S1 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1]);
		// printf("S2 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2]);
		// printf("S3 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3]);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0x44)
			return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] != 0x33)
			return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] != 0x22)
			return false;
		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3] != 0x11)
			return false;

		delete tb;
	}

	return true;
}

bool verify_MEM_LOAD_HAZARD(const char* trace)
{
	{
		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3] = 0;

		tb->SoC__DOT__rom__DOT__data[0] = 0x200007b7; // lui	a5,0x20000
		tb->SoC__DOT__rom__DOT__data[1] = 0x0007a783; // lw		a5,0(a5) # 20000000 <_edata+0x1fff0000>
		tb->SoC__DOT__rom__DOT__data[2] = 0x00078413; // mv		s0,a5
		tb->SoC__DOT__rom__DOT__data[3] = 0x00008067; // j		0

		tb->SoC__DOT__sdram__DOT__data[0] = 0xcafebabe;

		evaluate(tb, trace, 7);

		// printf("S0 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0xcafebabe)
			return false;

		delete tb;
	}

	return true;
}

bool verify_PIPELINE(const char* trace)
{
	{
		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;

		tb->SoC__DOT__rom__DOT__data[0] = 0x00100413; // li		s0,1
		tb->SoC__DOT__rom__DOT__data[1] = 0x00840433; // add	s0,s0,s0	2=1+1
		tb->SoC__DOT__rom__DOT__data[2] = 0x00840433; // add	s0,s0,s0	4=2+2
		tb->SoC__DOT__rom__DOT__data[3] = 0x00840433; // add	s0,s0,s0	8=4+4
		tb->SoC__DOT__rom__DOT__data[4] = 0x00840433; // add	s0,s0,s0	16=8+8
		tb->SoC__DOT__rom__DOT__data[5] = 0x00840433; // add	s0,s0,s0	32=16+16
		tb->SoC__DOT__rom__DOT__data[6] = 0x00008067; // j		0

		evaluate(tb, trace, 7 + 6 + 6 + 6);

		//printf("S0 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

		// if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0x11111111)
		// 	return false;

		delete tb;
	}

	return true;
}

bool verify_PIPELINE_MEMORY(const char* trace)
{
	{
		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3] = 0;

		tb->SoC__DOT__rom__DOT__data[0] = 0x200007b7; // lui	a5,0x20000
		tb->SoC__DOT__rom__DOT__data[1] = 0x0007a403; // lw		s0,0(a5) # 20000000 <_edata+0x1fff0000>
		tb->SoC__DOT__rom__DOT__data[2] = 0x0087a223; // sw		s0,4(a5)
		tb->SoC__DOT__rom__DOT__data[3] = 0x0047a403; // lw		s0,4(a5)
		tb->SoC__DOT__rom__DOT__data[4] = 0x0087a423; // sw		s0,8(a5)
		tb->SoC__DOT__rom__DOT__data[5] = 0x0087a403; // lw		s0,8(a5)
		tb->SoC__DOT__rom__DOT__data[6] = 0x0087a623; // sw		s0,12(a5)
		tb->SoC__DOT__rom__DOT__data[7] = 0x00c7a403; // lw		s0,12(a5)
		tb->SoC__DOT__rom__DOT__data[8] = 0xfe1ff06f; // j		0

		tb->SoC__DOT__sdram__DOT__data[0] = 0x11111111;
		tb->SoC__DOT__sdram__DOT__data[1] = 0x22222222;
		tb->SoC__DOT__sdram__DOT__data[2] = 0x33333333;
		tb->SoC__DOT__sdram__DOT__data[3] = 0x44444444;

		evaluate(tb, trace, 8);

		//printf("S0 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0x11111111)
			return false;

		delete tb;
	}

	return true;
}

bool verify_PIPELINE_MEMORY_B(const char* trace)
{
	{
		auto tb = create_soc();
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
		tb->SoC__DOT__cpu__DOT__registers__DOT__r[S3] = 0;

		tb->SoC__DOT__rom__DOT__data[0] = 0x200007b7; // lui	a5,0x20000
		tb->SoC__DOT__rom__DOT__data[1] = 0x00078403; // lb		s0,0(a5) # 20000000 <_edata+0x1fff0000>
		tb->SoC__DOT__rom__DOT__data[2] = 0x008780a3; // sb		s0,1(a5)
		tb->SoC__DOT__rom__DOT__data[3] = 0x00178403; // lb		s0,1(a5)
		tb->SoC__DOT__rom__DOT__data[4] = 0x00878123; // sb		s0,2(a5)
		tb->SoC__DOT__rom__DOT__data[5] = 0x00278403; // lb		s0,2(a5)
		tb->SoC__DOT__rom__DOT__data[6] = 0x008781a3; // sb		s0,3(a5)
		tb->SoC__DOT__rom__DOT__data[7] = 0x00378403; // lb		s0,3(a5)
		tb->SoC__DOT__rom__DOT__data[8] = 0xfe1ff06f; // j		0

		tb->SoC__DOT__sdram__DOT__data[0] = 0x00000011;
		tb->SoC__DOT__sdram__DOT__data[1] = 0x22222222;
		tb->SoC__DOT__sdram__DOT__data[2] = 0x33333333;
		tb->SoC__DOT__sdram__DOT__data[3] = 0x44444444;

		evaluate(tb, trace, 8);

		//printf("S0 %08x\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);
		//printf("D[0] %08x\n", tb->SoC__DOT__sdram__DOT__data[0]);

		if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0x00000011)
			return false;
		// if (tb->SoC__DOT__sdram__DOT__data[0] != 0x11111111)
		// 	return false;

		delete tb;
	}

	return true;
}

bool verify_ICACHE(const char* trace)
{
	auto tb = create_soc();

	tb->SoC__DOT__rom__DOT__data[0] = 0x00000013; // nop
	tb->SoC__DOT__rom__DOT__data[1] = 0x00000013; // nop
	tb->SoC__DOT__rom__DOT__data[2] = 0x00000013; // nop
	tb->SoC__DOT__rom__DOT__data[3] = 0x00000013; // nop
	tb->SoC__DOT__rom__DOT__data[4] = 0xff1ff06f; // j		0

	evaluate(tb, trace, 4 + 1 + 4 + 1 + 2);

	printf("PC: %08x\n", tb->SoC__DOT__cpu__DOT__fetch__DOT__pc);

	if (tb->SoC__DOT__cpu__DOT__fetch__DOT__pc != 0x0000000c)
		return false;

	delete tb;
	return true;
}

bool verify_FADD(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0000f153; // fadd.s	ft2,ft1,ft0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[34];
	if (*(float*)&r != (v1 + v2))
		return false;

	delete tb;
	return true;
}

bool verify_FSUB(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x0800f153; // fsub.s	ft2,ft1,ft0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[34];
	
	log::info << *(float*)&r << Endl;

	if (*(float*)&r != (v2 - v1))
		return false;

	delete tb;
	return true;
}

bool verify_FMUL(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1000f153; // fmul.s	ft2,ft1,ft0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[34];
	
	// printf("%f * %f = %f\n", v1, v2, *(float*)&r);

	if (*(float*)&r != (v2 * v1))
		return false;

	delete tb;
	return true;
}

bool verify_FDIV(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1800f153; // fdiv.s	ft2,ft1,ft0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[34];
	
	log::info << *(float*)&r << Endl;

	if (*(float*)&r != (v2 / v1))
		return false;

	delete tb;
	return true;
}

bool verify_FCVT(const char* trace)
{
	const float v1 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__rom__DOT__data[0] = 0xc0007453; // fcvt.w.s	s0,ft0

	evaluate(tb, trace, 1);

	int32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0];
	
	//printf("%f => %d\n", v1, r);

	if (r != (int32_t)v1)
		return false;

	delete tb;
	return true;
}

bool verify_FLW(const char* trace)
{
	const float v1 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__ram__DOT__data[0] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0x10000000;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00042007; // flw	ft0,0(s0)

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	
	//printf("%f => %d\n", v1, r);

	if (*(float*)&r != v1)
		return false;

	delete tb;
	return true;
}

bool verify_FSW(const char* trace)
{
	const float v1 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__ram__DOT__data[0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0x10000000;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__rom__DOT__data[0] = 0x00042027; // fsw	ft0,0(s0)

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__ram__DOT__data[0];
	
	//printf("%f => %d\n", v1, r);

	if (*(float*)&r != v1)
		return false;

	delete tb;
	return true;
}

bool verify_FMV_X_W(const char* trace)
{
	const float v1 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__rom__DOT__data[0] = 0xe0000453; // fmv.x.w	s0,ft0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0];
	
	//printf("%f => %d\n", v1, r);

	if (r != *(uint32_t*)&v1)
		return false;

	delete tb;
	return true;
}

bool verify_FMV_W_X(const char* trace)
{
	const uint32_t v1 = urnd32();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__rom__DOT__data[0] = 0xf0040053; // fmv.w.x	ft0,s0

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	
	//printf("%f => %d\n", v1, r);

	if (r != v1)
		return false;

	delete tb;
	return true;
}

bool verify_FEQ(const char* trace)
{
	const float v1 = 1.234f;
	const float v2 = 1.234f;
	const float v3 = -1.234f;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = ~0UL;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0xa0102453; // feq.s	s0,ft0,ft1
	tb->SoC__DOT__rom__DOT__data[1] = 0xa02024d3; // feq.s	feq.s	s1,ft0,ft2

	evaluate(tb, trace, 2);

	//printf("%f => %d\n", v1, r);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 1)
		return false;
	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] != 0)
		return false;

	delete tb;
	return true;
}

bool verify_FLT(const char* trace)
{
	const float v1 = 1.234f;
	const float v2 = 1.234f;
	const float v3 = -1.234f;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] = ~0UL;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0xa0101453; // flt.s	s0,ft0,ft1
	tb->SoC__DOT__rom__DOT__data[1] = 0xa02014d3; // flt.s	s1,ft0,ft2
	tb->SoC__DOT__rom__DOT__data[2] = 0xa0011953; // flt.s	s2,ft2,ft0

	evaluate(tb, trace, 3);

	//printf("%d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0]);
	//printf("%d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1]);
	//printf("%d\n", tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2]);

	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S0] != 0)
		return false;
	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S1] != 0)
		return false;
	if (tb->SoC__DOT__cpu__DOT__registers__DOT__r[S2] != 1)
		return false;

	delete tb;
	return true;
}

bool verify_FMADD(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();
	const float v3 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[35] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1820f043; // fmadd.s	ft0,ft1,ft2,ft3

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//printf("%f * %f = %f\n", v1, v2, *(float*)&r);

	if (*(float*)&r != (v1 * v2 + v3))
		return false;

	delete tb;
	return true;
}

bool verify_FMSUB(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();
	const float v3 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[35] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1820f047; // fmsub.s	ft0,ft1,ft2,ft3

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//printf("%f * %f - %f = %f\n", v1, v2, v3, *(float*)&r);

	if (*(float*)&r != (v1 * v2 - v3))
		return false;

	delete tb;
	return true;
}

bool verify_FNMADD(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();
	const float v3 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[35] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1820f04f; // fnmadd.s	ft0,ft1,ft2,ft3

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//printf("%f * %f = %f\n", v1, v2, *(float*)&r);

	if (*(float*)&r != -(v1 * v2 + v3))
		return false;

	delete tb;
	return true;
}

bool verify_FNMSUB(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();
	const float v3 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[35] = *(uint32_t*)&v3;
	tb->SoC__DOT__rom__DOT__data[0] = 0x1820f04b; // fnmsub.s	ft0,ft1,ft2,ft3

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//printf("%f * %f = %f\n", v1, v2, *(float*)&r);

	if (*(float*)&r != -(v1 * v2 - v3))
		return false;

	delete tb;
	return true;
}

bool verify_FSGNJ(const char* trace)
{
	const float v1 = 1.2f;
	const float v2 = -2.3f;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x20208053; // fsgnj.s	ft0,ft1,ft2

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//log::info << *(float*)&r << Endl;

	if (*(float*)&r != -1.2f)
		return false;

	delete tb;
	return true;
}

bool verify_FSGNJN(const char* trace)
{
	const float v1 = 1.2f;
	const float v2 = -2.3f;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x20209053; // fsgnjn.s	ft0,ft1,ft2

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//log::info << *(float*)&r << Endl;

	if (*(float*)&r != 1.2f)
		return false;

	delete tb;
	return true;
}

bool verify_FSGNJX(const char* trace)
{
	const float v1 = 1.2f;
	const float v2 = -2.3f;

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x2020a053; // fsgnjx.s	ft0,ft1,ft2

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	//log::info << *(float*)&r << Endl;

	if (*(float*)&r != -1.2f)
		return false;

	delete tb;
	return true;
}

bool verify_FMIN(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x28208053; // fmin.s	ft0,ft1,ft2

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	if (*(float*)&r != std::min< float >(v1, v2))
	{
		printf("min(%f, %f) = %f\n", v1, v2, *(float*)&r);
		printf("--> %f\n", std::min< float >(v1, v2));
		return false;
	}

	delete tb;
	return true;
}

bool verify_FMAX(const char* trace)
{
	const float v1 = rndf();
	const float v2 = rndf();

	auto tb = create_soc();
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[32] = 0;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[33] = *(uint32_t*)&v1;
	tb->SoC__DOT__cpu__DOT__registers__DOT__r[34] = *(uint32_t*)&v2;
	tb->SoC__DOT__rom__DOT__data[0] = 0x28209053; // fmax.s	ft0,ft1,ft2

	evaluate(tb, trace, 1);

	uint32_t r = tb->SoC__DOT__cpu__DOT__registers__DOT__r[32];
	if (*(float*)&r != std::max< float >(v1, v2))
	{
		printf("max(%f, %f) = %f\n", v1, v2, *(float*)&r);
		printf("--> %f\n", std::min< float >(v1, v2));
		return false;
	}
	delete tb;
	return true;
}

// ========================================================

bool verify(bool (*fn)(const char* trace), const char* name, bool ftrce)
{
	bool result = true;
	char fnf[256];

	sprintf(fnf, "%s.fst", name);

	if (!ftrce)
	{
		for (int i = 0; i < ITERATIONS; ++i)
		{
			if (!fn(nullptr))
			{
				result = false;
				break;
			}
		}
	
		// Re-run failed test with tracing enabled.
		if (!result)
		{
			log::info << L"Verify failed, re-run with trace..." << Endl;
			if (fn(fnf))
				log::warning << L"Inconsistent verification." << Endl;
		}
	}
	else
	{
		if (!fn(fnf))
			result = false;
	}

	return result;
}

#define CHECK(vfn) \
	if (verify(vfn, #vfn, ftrce)) { printf("%s SUCCEEDED!\n", #vfn); } \
	else { printf("%s FAILED!\n", #vfn); success = false; }

int main(int argc, char **argv)
{
	bool success = true;
	bool ftrce = false;

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
	CHECK(verify_MEM_LOAD_HAZARD);
	CHECK(verify_PIPELINE);
	CHECK(verify_PIPELINE_MEMORY);
	CHECK(verify_PIPELINE_MEMORY_B);
	CHECK(verify_ICACHE);
	CHECK(verify_FADD);
	CHECK(verify_FSUB);
	CHECK(verify_FMUL);
	CHECK(verify_FDIV);
	CHECK(verify_FCVT);
	CHECK(verify_FLW);
	CHECK(verify_FSW);
	CHECK(verify_FMV_X_W);
	CHECK(verify_FMV_W_X);
	CHECK(verify_FEQ);
	CHECK(verify_FLT);
	CHECK(verify_FMADD);
	CHECK(verify_FMSUB);
	CHECK(verify_FNMADD);
	CHECK(verify_FNMSUB);
	CHECK(verify_FSGNJ);
	CHECK(verify_FSGNJN);
	CHECK(verify_FSGNJX);
	CHECK(verify_FMIN);
	CHECK(verify_FMAX);

	if (success)
		printf("SUCCESS!\n");
	else
		printf("FAILED!\n");

	return 0;
}