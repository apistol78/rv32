/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include "Rv32/Helpers.h"

const wchar_t* getRegisterName(uint32_t index)
{
	const wchar_t* c_names[] =
	{
		L"ZERO",
		L"RA",
		L"SP",
		L"GP",
		L"TP",
		L"T0",
		L"T1",
		L"T2",
		L"S0/FP",
		L"S1",
		L"A0",
		L"A1",
		L"A2",
		L"A3",
		L"A4",
		L"A5",
		L"A6",
		L"A7",
		L"S2",
		L"S3",
		L"S4",
		L"S5",
		L"S6",
		L"S7",
		L"S8",
		L"S9",
		L"S10",
		L"S11",
		L"T3",
		L"T4",
		L"T5",
		L"T6"
	};
	return c_names[index];
}