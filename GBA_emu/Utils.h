#pragma once
#include <cstdint>

uint32_t rotateRight(uint32_t word, uint32_t n);

uint32_t arithmeticShift(uint32_t word, uint32_t n);

/*
	Given a 32-bit word, return a word with [nbits] LSB's with sign extended to MSB's
*/
uint32_t signExtend(uint32_t word, uint32_t nbits);

/*
	
*/
uint32_t getMulIter(uint32_t Rs);

/*
	Get specified bits in a word (little endian)
*/

uint32_t getBits(uint32_t word, size_t start_idx, size_t n);
uint16_t getBits(uint16_t word, size_t start_idx, size_t n);
uint32_t getBit(uint32_t word, size_t idx);
uint16_t getBit(uint16_t word, size_t idx);

