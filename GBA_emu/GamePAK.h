#pragma once
#include <cstdint>
#include <vector>

class GamePAK
{
private:
	std::vector<uint16_t> rom;
	std::vector<uint8_t> ram;

public:
	GamePAK();
	~GamePAK();

};

