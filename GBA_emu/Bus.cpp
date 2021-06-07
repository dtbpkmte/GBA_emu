#include "Bus.h"

Bus::Bus()
{
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
		//BIOS - can't read
		//idk what to return here
		//maybe data abort
		return 0xffffffff;
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
}

void Bus::clock()
{
}
