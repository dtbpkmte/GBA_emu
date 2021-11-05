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

uint32_t getBits(uint32_t word, size_t start_idx, size_t n) {
	uint32_t mask = ~(uint32_t)0 >> (32 - n);
	return (word >> start_idx) & mask;
}

uint16_t getBits(uint16_t word, size_t start_idx, size_t n) {
	uint16_t mask = ~(uint32_t)0 >> (16 - n);
	return (word >> start_idx) & mask;
}

uint32_t getBit(uint32_t word, size_t idx) {
	return getBits(word, idx, 1);
}
uint16_t getBit(uint16_t word, size_t idx) {
	return getBits(word, idx, 1);
}

bool contains(std::string s1, std::string s2) {
	return s1.find(s2) != std::string::npos;
}