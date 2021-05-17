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

	std::array<uint32_t, 8192> iwram;   //32kB
	std::array<uint16_t, 131072> ewram;
	std::array<uint32_t, 256> ioram;
	std::array<uint32_t, 16384> vram;

public:
	void    cpuWrite(uint32_t a, uint32_t data);
	uint32_t cpuRead(uint32_t a, bool bReadOnly = false);

public:
	void reset();
	void clock();
};

