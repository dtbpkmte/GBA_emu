#include "Bus.h"

Bus::Bus()
{
	armCpu.ConnectBus(this);
	bios.resize(4096, 0);
	iwram.resize(1024, 0);
	ewram.resize(16384, 0);
	ioram.resize(32, 0);
	vram.resize(6144, 0);
}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint32_t a, uint32_t data)
{
}

uint32_t Bus::cpuRead(uint32_t a, bool bReadOnly)
{
	if (a <= 0x3fff) {
		return bios[a/4];
	}
	else if (a >= 0x02000000 && a <= 0x0203ffff) {
		return ewram[(a & 0xfffff)/2];
	}
	else if (a >= 0x03000000 && a <= 0x03007fff) {
		return iwram[(a & 0xffff)/4];
	}
	else if (a >= 0x04000000 && a <= 0x040003fe) {
		return ioram[(a & 0xfff)/4];
	}
	else {
		//actually there are some more else if's
		//to be added later
		return 0;
	}
}

void Bus::reset()
{
	armCpu.reset();
}

void Bus::clock()
{
	armCpu.clock();
}

void Bus::step() {
	armCpu.clock(true);
}
