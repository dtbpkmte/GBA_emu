#include <cstdint>
#include <iostream>
#include "Bus.h"

using namespace std;

int main() {
	Bus GBA_bus;
	cout << (GBA_bus.armCpu.*(GBA_bus.armCpu.instruction_lookup[0][0].addrmode->addrmode_f))() << endl;

	return 0;
}