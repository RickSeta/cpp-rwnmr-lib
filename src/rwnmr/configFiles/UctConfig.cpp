#include "UctConfig.h"

using namespace std;

// default constructors
UctConfig::UctConfig() : BaseConfig(), PORE_COLOR(0)
{
}

// read config file


void UctConfig::readFirstIdx(string s)
{
	this->FIRST_IDX = std::stoi(s);
}

void UctConfig::readDigits(string s)
{
	this->DIGITS = std::stoi(s);
}

void UctConfig::readExtension(string s)
{
	this->EXTENSION = s;
}

void UctConfig::readSlices(string s)
{
	this->SLICES = std::stoi(s);
}

void UctConfig::readResolution(string s)
{
	this->RESOLUTION = std::stod(s);
}

void UctConfig::readVoxelDivision(string s)
{
	this->VOXEL_DIVISION = std::stoi(s);
}

void UctConfig::readPoreColor(string s)
{
    uint8_t color = (uint8_t) std::stoi(s);
	if(color > 255) this->PORE_COLOR = 255;
    else this->PORE_COLOR = color;
}
