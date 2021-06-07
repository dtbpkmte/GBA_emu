#pragma once
#include "ARM7TDMI.h"
#include "Cartridge.h"
//#include "LR35902.h"

#include <array>
#include <cstdint>
#include <memory>

class Bus
{
public:
	Bus();
	~Bus();

public:
	ARM7TDMI armCpu;
	//LR35902 sharpCpu;

	std::array<uint32_t, 8192> iwram;   //32kB
	std::array<uint16_t, 131072> ewram; //256kB
	std::array<uint32_t, 256> ioram;    //1kB
	std::array<uint32_t, 16384> vram;   //96kB

	std::shared_ptr<Cartridge> cart;
	bool cartInserted = false;

public:
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);

public:
	void    cpuWrite(uint32_t a, uint32_t data);
	uint32_t cpuRead(uint32_t a, bool bReadOnly = false);

public:
	void reset();
	void clock();
	uint32_t nSystemClockCounter = 0;
};

