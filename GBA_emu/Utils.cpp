#include "Utils.h"

uint32_t rotateRight(uint32_t word, uint8_t n) {
	return (word >> n) | (word << (32 - n));
}

uint32_t arithmeticShift(uint32_t word, uint8_t n) {
	return (word >> 31 == 0) ? (word >> n)
						     : (word >> n) | ~(~(uint32_t)0 >> n);
}
