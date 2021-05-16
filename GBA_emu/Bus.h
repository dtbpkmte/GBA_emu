#pragma once
#include "ARM7TDMI.h"
//#include "LR35902.h"

#include <array>

class Bus
{
public:
	Bus();
	~Bus();

public:
	ARM7TDMI armCpu;
	//LR35902 sharpCpu;

	std::array<uint32_t, 32768> iwram;
	std::array<uint16_t, 262144> ewram;
	std::array<uint32_t, 1024> ioram;
	std::array<uint32_t, 65536> vram;

public:
	void    cpuWrite(uint32_t a, uint32_t data);
	uint32_t cpuRead(uint32_t a, bool bReadOnly = false);

public:
	void reset();
	void clock();
};

