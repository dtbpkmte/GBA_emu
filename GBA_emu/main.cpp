#include <cstdint>
#include <iostream>
#include "Bus.h"
#include <fstream>
#include "olcPixelGameEngine.h"

class Graphic : public olc::PixelGameEngine
{
public:
	Graphic()
	{
		sAppName = "GBA Emulator";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};

int main() {
	Bus bus;
	/*Graphic graphic;
	if (graphic.Construct(1080, 720, 1, 1))
		graphic.Start();*/
	bus.iwram[0] = 0xe3a00005;
	bus.iwram[1] = 0xe3a0100a;
	bus.iwram[2] = 0xe0800001;
	std::cout << "Press c to continue, q to quit\n";
	bus.armCpu.disassembleARM(bus.iwram);
	while (true) {
		char input;
		std::cin >> input;
		if (input == 'c') {
			bus.clock();
			std::cout << std::hex <<"cycles= " << bus.armCpu.cycles << " pc = " << bus.armCpu.pc
				<< " prefetch=" << bus.armCpu.prefetch << " opcode=" << bus.armCpu.opcode << '\n';
			std::cout << std::hex << "R0=" << bus.armCpu.readRegister(0) 
								<< " R1=" << bus.armCpu.readRegister(1) << '\n';
		}
		else if (input == 'q') break;
	}

	/*char buffer[16384]; //TODO: move to heap later
	std::vector<uint32_t> biosData(16384/sizeof(uint32_t));
	std::ifstream biosFile;*/

	/*biosFile.open("gba_bios.bin", std::ios::in | std::ios::binary);
	biosFile.read(buffer, 16384);
	memcpy(&biosData[0], &buffer[0], 16384);*/
	/*for (auto i : biosData) {
		std::cout << std::hex << i << '\n';
	}*/
	//GBA_bus.armCpu.disassembleARM(biosData);

	return 0;
}