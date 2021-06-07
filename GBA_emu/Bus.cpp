#include "Bus.h"
#include <algorithm>

Bus::Bus()
{
	std::fill(iwram.begin(), iwram.end(), 0);
	std::fill(ewram.begin(), ewram.end(), 0);
	std::fill(ioram.begin(), ioram.end(), 0);
	std::fill(vram.begin(), vram.end(), 0);
}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint32_t a, uint32_t data)
{
}

uint32_t Bus::cpuRead(uint32_t a, bool bReadOnly)
{
	if (a <= 0x1fffffff) {
		//BIOS - can't read
		//idk what to return here
		//maybe data abort
		return armCpu.read(armCpu.pc);
	}
	else if (a >= 0x02000000 && a <= 0x0203ffff) {
		return ewram[(a & 0xfffff) >> 1];
	}
	else if (a >= 0x03000000 && a <= 0x03007fff) {
		return iwram[(a & 0xffff) >> 2];
	}
	else if (a >= 0x04000000 && a <= 0x040003fe) {
		return ioram[(a & 0xfff) >> 2];
	}
	else {
		//actually there are some more else if's
		//to be added later
		return 0;
	}
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	// Connects cartridge to both Main Bus and CPU Bus
	this->cart = cartridge;
}

void Bus::reset()
{
	armCpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	armCpu.clock();
	++nSystemClockCounter;
}
