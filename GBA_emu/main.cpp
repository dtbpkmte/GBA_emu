#include <cstdint>
#include <iostream>
#include "Bus.h"
#include <fstream>
#include <bitset>
#include "olcPixelGameEngine.h"


Bus bus;

#define SPACING 5
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 720
#define ASM_MAX_LENGTH 35
const auto CHAR_WIDTH = 16; // scaling of 2
const auto CHAR_BOX_HEIGHT = 16 + SPACING; // spacing 5 top and bottom
const auto ADDR_BOX_WIDTH = 8*CHAR_WIDTH + 2*SPACING;
const auto HEX_BOX_WIDTH = 8*CHAR_WIDTH + 2*SPACING;
const auto ASM_BOX_WIDTH = ASM_MAX_LENGTH*CHAR_WIDTH + 2*SPACING; // max 60 characters, with left and right spacing
const auto MEMORY_BOX_WIDTH = ADDR_BOX_WIDTH + HEX_BOX_WIDTH + ASM_BOX_WIDTH;
const auto MEMORY_BOX_HEIGHT = (16*2+1)*CHAR_BOX_HEIGHT;
const auto REGISTER_WIDTH = 11*CHAR_WIDTH;

const auto ADDR_UI_WIDTH = 8*CHAR_WIDTH + 2*SPACING;
const auto ADDR_UI_HEIGHT = 2 * CHAR_BOX_HEIGHT;

const auto ADDR_X = SCREEN_WIDTH - MEMORY_BOX_WIDTH;
const auto HEX_X = SCREEN_WIDTH - ASM_BOX_WIDTH - HEX_BOX_WIDTH;
const auto ASM_X = SCREEN_WIDTH - ASM_BOX_WIDTH;

const auto CPSR_X = (ADDR_X - 32*CHAR_WIDTH)/2;
const auto CPSR_Y = 8*CHAR_BOX_HEIGHT + SCREEN_HEIGHT/2 + 35;
const auto OPCODE_HEX_X = (ADDR_X - 8*CHAR_WIDTH)/2;
const auto OPCODE_BIN_X = CPSR_X;
const auto OPCODE_Y = CPSR_Y + 75;

const auto topLimit = 0;
const auto botLimit = 0x0E00FFFF;

class Graphic : public olc::PixelGameEngine
{
public:
	Graphic()
	{
		sAppName = "GBA Emulator";
	}

private:
	uint32_t topAddr = 0x03000000; // topAddr must be >= 0
	// botAddr will be topAddr + 15 but must not be > botLimit
	bool run = false;

	bool enterAddrMode = false;
	uint32_t addrInput = 0;
	uint8_t numInputs = 0; // must be <= 8

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// handle input
		if (GetKey(olc::Key::S).bPressed) bus.step();
		else if (GetKey(olc::Key::SPACE).bPressed) run = !run;
		else if (GetKey(olc::Key::UP).bPressed) {
			if ((topAddr -= 4) > botLimit) {
				std::cout << "Cannot scroll up!\n";
				topAddr -= 4;
			}
		}
		else if (GetKey(olc::Key::DOWN).bPressed) {
			if ((topAddr += 4) > botLimit) {
				std::cout << "Cannot scroll down!\n";
				topAddr -= 4;
			}
		}
		else if (GetKey(olc::Key::LEFT).bPressed) {
			if ((topAddr -= 64) > botLimit) {
				std::cout << "Cannot go to previous page!\n";
				topAddr += 64;
			}
		}
		else if (GetKey(olc::Key::RIGHT).bPressed) {
			if ((topAddr += 64) > botLimit) {
				std::cout << "Cannot go to next page!\n";
				topAddr -= 64;
			}
		}
		else if (GetKey(olc::Key::ENTER).bPressed) {
			if (!enterAddrMode) {
				enterAddrMode = true;
				numInputs = 0;
				addrInput = 0;
			}
			else {
				if (addrInput > botLimit) 
					std::cout << "Input addr too large\n";
				else {
					topAddr = addrInput;
					enterAddrMode = false;
				}
			}
		}
		else if (GetKey(olc::Key::ESCAPE).bPressed && enterAddrMode)
			enterAddrMode = false;

		if (enterAddrMode) readHexInput(addrInput);

		// draw frame
		Clear(olc::BLACK);
		
		// draw borders
		DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, olc::WHITE);
		// memory box
		DrawLine(ADDR_X, 0, ADDR_X, MEMORY_BOX_HEIGHT, olc::WHITE); // |
		DrawLine(ADDR_X, CHAR_BOX_HEIGHT, ScreenWidth() - 1, CHAR_BOX_HEIGHT, olc::WHITE); // -
		DrawLine(HEX_X, 0, HEX_X, MEMORY_BOX_HEIGHT, olc::WHITE); // |
		DrawLine(ADDR_X, MEMORY_BOX_HEIGHT, ScreenWidth() - 1, MEMORY_BOX_HEIGHT, olc::WHITE); // -
		DrawLine(ASM_X, 0, ASM_X, MEMORY_BOX_HEIGHT, olc::WHITE); // |
		// draw labels
		DrawString(((ADDR_X + HEX_X) - 3*CHAR_WIDTH)/2, SPACING, "ADDR", olc::WHITE, 2);
		DrawString(((HEX_X + ASM_X) - 3*CHAR_WIDTH)/2, 5, "HEX", olc::WHITE, 2);
		DrawString(((ASM_X + SCREEN_WIDTH) - 3*CHAR_WIDTH)/2, 5, "ASM", olc::WHITE, 2);

		// draw addr, hex, disassembly, line
		for (auto i = 0; i < 16; ++i) {
			auto realAddr = topAddr + i*4;
			auto yCoord = (i*2+1)*CHAR_BOX_HEIGHT + SPACING;
			auto color = (realAddr == bus.armCpu.pc) ? olc::YELLOW : olc::WHITE;
			// draw addr
			DrawString(ADDR_X + SPACING, yCoord,
					n2hexstr(realAddr), color, 2);
			// draw hex
			DrawString(HEX_X + SPACING, yCoord, 
					n2hexstr(bus.cpuRead(realAddr, true)), color, 2);
			// draw disassembly
			DrawString(ASM_X + SPACING, yCoord, 
					breakInstruction(bus.armCpu.disassembleARMInstruction(bus.cpuRead(realAddr, true), realAddr)), 
					color, 2);
			// draw separating line
			DrawLine(ADDR_X, (i*2+1)*CHAR_BOX_HEIGHT, 
					SCREEN_WIDTH - 1, (i*2+1)*CHAR_BOX_HEIGHT, olc::WHITE);
		}

		// GBA LCD - 240x160
		DrawRect(ADDR_X/2 - 120, SCREEN_HEIGHT/4 - 80, 240, 160, olc::WHITE);
		DrawLine(0, SCREEN_HEIGHT/2, ADDR_X, SCREEN_HEIGHT/2, olc::WHITE);

		// registers
		for (auto i = 0; i < 4; ++i) {
			for (auto j = 1; j <= 4; ++j) {
				DrawString(j*ADDR_X/4 - REGISTER_WIDTH + 40, 
						i*2*CHAR_BOX_HEIGHT+SCREEN_HEIGHT/2 + 5, 
						bus.armCpu.getRegisterName(i*4+j-1), olc::WHITE, 2);
				DrawString(j*ADDR_X/4 - REGISTER_WIDTH + 40, 
						(i*2+1)*CHAR_BOX_HEIGHT+SCREEN_HEIGHT/2 + 5, 
						padZero(bus.armCpu.readRegister(i*4+j-1), 8), olc::WHITE, 2);
			}
		}

		// cpsr
		DrawString(CPSR_X, CPSR_Y, "NZCV          R         IFT  M  ", olc::WHITE, 2);
		DrawString(CPSR_X, CPSR_Y+CHAR_BOX_HEIGHT, 
				std::bitset<32>(bus.armCpu.cpsr.reg).to_string(), olc::WHITE, 2);

		// opcode
		DrawString(OPCODE_HEX_X, OPCODE_Y, n2hexstr(bus.armCpu.opcode), olc::WHITE, 2);
		DrawString(OPCODE_BIN_X, OPCODE_Y+CHAR_BOX_HEIGHT, 
				std::bitset<32>(bus.armCpu.opcode).to_string(), olc::WHITE, 2);

		// draw the user input box
		if (enterAddrMode) {
			FillRect((SCREEN_WIDTH - ADDR_UI_WIDTH)/2, (SCREEN_HEIGHT - ADDR_UI_HEIGHT)/2,
					ADDR_UI_WIDTH, ADDR_UI_HEIGHT, olc::DARK_GREEN);
			DrawString((SCREEN_WIDTH - ADDR_UI_WIDTH)/2 + SPACING,
						(SCREEN_HEIGHT - ADDR_UI_HEIGHT)/2 + SPACING,
						"GOTO:", olc::YELLOW, 2);
			DrawString((SCREEN_WIDTH - ADDR_UI_WIDTH)/2 + SPACING,
						(SCREEN_HEIGHT - ADDR_UI_HEIGHT)/2 + CHAR_BOX_HEIGHT,
						n2hexstr(addrInput), olc::YELLOW, 2);
		}

		return true;
	}
private:
	std::string breakInstruction(std::string s) {
		if (s.length() > ASM_MAX_LENGTH)
			return s.substr(0, ASM_MAX_LENGTH) + '\n' + s.substr(ASM_MAX_LENGTH);
		return s;
	}

	void readHexInput(uint32_t &out) {
		if (GetKey(olc::Key::BACK).bPressed) {
			if (numInputs) {
				out >>= 4;
				--numInputs;
			}
		}
		else if (numInputs < 8) {
			++numInputs;
			if (GetKey(olc::Key::K0).bPressed) out = (out << 4);
			else if (GetKey(olc::Key::K1).bPressed) out = (out << 4) | 0x1;
			else if (GetKey(olc::Key::K2).bPressed) out = (out << 4) | 0x2;
			else if (GetKey(olc::Key::K3).bPressed) out = (out << 4) | 0x3;
			else if (GetKey(olc::Key::K4).bPressed) out = (out << 4) | 0x4;
			else if (GetKey(olc::Key::K5).bPressed) out = (out << 4) | 0x5;
			else if (GetKey(olc::Key::K6).bPressed) out = (out << 4) | 0x6;
			else if (GetKey(olc::Key::K7).bPressed) out = (out << 4) | 0x7;
			else if (GetKey(olc::Key::K8).bPressed) out = (out << 4) | 0x8;
			else if (GetKey(olc::Key::K9).bPressed) out = (out << 4) | 0x9;
			else if (GetKey(olc::Key::A).bPressed) out = (out << 4) | 0xa;
			else if (GetKey(olc::Key::B).bPressed) out = (out << 4) | 0xb;
			else if (GetKey(olc::Key::C).bPressed) out = (out << 4) | 0xc;
			else if (GetKey(olc::Key::D).bPressed) out = (out << 4) | 0xd;
			else if (GetKey(olc::Key::E).bPressed) out = (out << 4) | 0xe;
			else if (GetKey(olc::Key::F).bPressed) out = (out << 4) | 0xf;
			else --numInputs;
		}
	}
};

int main() {
	// init some asm code
	bus.iwram[0] = 0xe3a00005;
	bus.iwram[1] = 0xe3a0100a;
	bus.iwram[2] = 0xe0800001;

	Graphic graphic;
	if (graphic.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1))
		graphic.Start();
	/*bus.iwram[0] = 0xe3a00005;
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
	}*/

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