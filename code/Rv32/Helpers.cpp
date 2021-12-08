#include "Rv32/Helpers.h"

const wchar_t* getRegisterName(uint32_t index)
{
    const wchar_t* c_names[] =
    {
        L"zero",
		L"ra",
		L"sp",
		L"gp",
		L"tp",
		L"t0",
		L"t1",
		L"t2",
		L"s0",
		L"s1",
		L"a0",
		L"a1",
		L"a2",
		L"a3",
		L"a4",
		L"a5",
		L"a6",
		L"a7",
		L"s2",
		L"s3",
		L"s4",
		L"s5",
		L"s6",
		L"s7",
		L"s8",
		L"s9",
		L"s10",
		L"s11",
		L"t3",
		L"t4",
		L"t5",
		L"t6"
    };
    return c_names[index];
}