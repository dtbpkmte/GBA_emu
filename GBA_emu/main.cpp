#include <cstdint>
#include <iostream>
#include "Bus.h"
#include <fstream>

using namespace std;

int main() {
	Bus GBA_bus;

	char buffer[16384];
	std::vector<uint32_t> biosData(16384/sizeof(uint32_t));
	std::ifstream biosFile;

	biosFile.open("gba_bios.bin", std::ios::in | std::ios::binary);
	biosFile.read(buffer, 16384);
	memcpy(&biosData[0], &buffer[0], 16384);
	/*for (auto i : biosData) {
		std::cout << std::hex << i << '\n';
	}*/
	//for (auto i = 0; i < 16; ++i) {
	//	std::cout << std::hex << (uint32_t) buffer[i] << '\n';
	//}
	GBA_bus.armCpu.disassembleARM(biosData);

	return 0;
}