#include "Utils.h"

uint32_t rotateRight(uint32_t word, uint32_t n) {
	return (word >> n) | (word << (32 - n));
}

uint32_t arithmeticShift(uint32_t word, uint32_t n) {
	return (word >> 31 == 0) ? (word >> n)
						     : (word >> n) | ~(~(uint32_t)0 >> n);
}

uint32_t signExtend(uint32_t word, uint32_t nbits)
{
	uint32_t LSBs = word & (~(uint32_t)0 >> (32 - nbits));
	return ((word >> (nbits - 1)) & 0x1) == 0 ? LSBs
											  : (~(uint32_t)0 << nbits) | LSBs;
}

uint32_t getMulIter(uint32_t Rs) {
	if (Rs <= 0x1ff) {
		return 1;
	}
	else if (Rs <= 0x1ffff) {
		return 2;
	}
	else if (Rs <= 0x1ffffff) {
		return 3;
	}
	else {
		return 4;
	}
}