#pragma once
#include "ARM7TDMI.h"
//#include "LR35902.h"

#include <vector>

class Bus
{
public:
	Bus();
	~Bus();

public:
	ARM7TDMI armCpu;
	//LR35902 sharpCpu;

	std::vector<uint32_t> iwram;	//8192 - 32kB
	std::vector<uint16_t> ewram;	//131072
	std::vector<uint32_t> ioram;	//256
	std::vector<uint32_t> vram;		//16384

public:
	void    cpuWrite(uint32_t a, uint32_t data);
	uint32_t cpuRead(uint32_t a, bool bReadOnly = false);

public:
	void reset();
	void clock();
};

