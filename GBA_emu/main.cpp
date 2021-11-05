#include <cstdint>
#include <iostream>
#include "Bus.h"

using namespace std;

int main() {
	Bus GBA_bus;
	// ADD R1, R2, ASR #2
	// 1110 0010 1000 0001 0010 0001 0100 0011
	cout << GBA_bus.armCpu.disassembleARMInstruction(0xe0811142) << std::endl;
	cout << GBA_bus.armCpu.disassembleARMInstruction(0xe3710002) << std::endl;
	cout << GBA_bus.armCpu.disassembleARMInstruction(0xe1710102) << std::endl;
	cout << GBA_bus.armCpu.disassembleARMInstruction(0xe2811003) << std::endl;
	
	return 0;
}