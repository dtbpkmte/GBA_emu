#pragma once
#include <cstdint>

uint32_t rotateRight(uint32_t word, uint32_t n);

uint32_t arithmeticShift(uint32_t word, uint32_t n);

/*
	Given a 32-bit word, return a word with [nbits] LSB's with sign extended to MSB's
*/
uint32_t signExtend(uint32_t word, uint32_t nbits);

uint32_t getMulIter(uint32_t Rs);

