#pragma once

#include <string>

class Bus;
class CPU;

bool loadHEX(const std::wstring& fileName, CPU& cpu, Bus& bus);
