#pragma once
#include "ARM7TDMI.h"
#include "GamePAK.h"

#include <vector>

class Bus
{
public:
	Bus();
	~Bus();

public:
	ARM7TDMI armCpu;
	//LR35902 sharpCpu;

	std::vector<uint32_t> bios;		//4096 - 16KB
	std::vector<uint32_t> iwram;	//1024 - 32KB
	std::vector<uint16_t> ewram;	//16384 - 256KB
	std::vector<uint32_t> ioram;	//32 - 256B
	std::vector<uint16_t> vram;		//6144 - 96KB

public:
	void    cpuWrite(uint32_t a, uint32_t data);
	uint32_t cpuRead(uint32_t a, bool bReadOnly = false);

public:
	void reset();
	void clock();

	void step();
	void run();
};

