#pragma once

#include <string>

class Bus;
class CPU;

bool loadELF(const std::wstring& fileName, CPU& cpu, Bus& bus);
