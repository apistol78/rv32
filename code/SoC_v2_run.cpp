#include <stdio.h>
#include "VSoC_v2_tb.h"

double sc_time_stamp() { return 0; }

int main(int argc, char **argv)
{
	VSoC_v2_tb* tb = new VSoC_v2_tb();
	tb->i_reset = 0;
	tb->i_clock = 0;
	tb->eval();

	//tb->SoC_v2_tb__DOT__cpu__DOT__fetch_pc = 0x0000009c;
	// tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[2] = 0x10000000 - 4;
	//for (int i = 0; i < 100; ++i)

	FILE* fp = fopen("trace_hw_2.txt", "w");

	uint32_t lastPC = -1;

	int32_t cd = 9467;

	uint32_t lastA2 = -1;

	for (;;)
	{
		// printf("PC: %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__fetch_pc);

		// if (tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[21] == 0x4ccccccc)
		// {
		// 	printf("%d left\n", cd);
		// 	break;
		// }

		if (lastPC != tb->SoC_v2_tb__DOT__cpu__DOT__decode_pc)
		{
		 	fprintf(fp, "%08x", tb->SoC_v2_tb__DOT__cpu__DOT__decode_pc);

			for (int i = 1; i < 32; ++i)
				fprintf(fp, " %08x", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[i]);

			fprintf(fp, "\n");

		 	lastPC = tb->SoC_v2_tb__DOT__cpu__DOT__decode_pc;

			if (cd < 80)
			{
				printf("%08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__decode_pc);
				printf("A0 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[10]);
				printf("A1 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[11]);
				printf("A2 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[12]);
				printf("S5 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[21]);
				printf("S8 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[24]);
				printf("T6 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[31]);
			}

			if (--cd < 0)
			{
				// printf("S5 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__registers__DOT__r[21]);
				// printf("rs1 id = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__decode_inst_rs1);
				// printf("rs2 id = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__decode_inst_rs2);
				// printf("fwd rs1 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__fwd_rs1);
				// printf("fwd rs2 = %08x\n", tb->SoC_v2_tb__DOT__cpu__DOT__fwd_rs2);
				break;
			}
		}

		tb->i_clock = 1;
		tb->eval();
		tb->i_clock = 0;
		tb->eval();
	}

	fclose(fp);
	return 0;
}