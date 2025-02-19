// include C++ standard libraries
#include <iostream>
#include <unistd.h>

// include CMake Configuration file

// include interface header files
#include "./framework/BitBlock.h"

using namespace std;

// Main program
int main(int argc, char **argv)
{    	
	vector<CustomMat> binaryMap2D(1, CustomMat(32, 32)); // Example 2D binary map
    vector<CustomMat> binaryMap3D(10, CustomMat(32, 32)); // Example 3D binary map
    uchar poreColor = 0; // Example pore color

    BitBlock bitBlock2D;
    bitBlock2D.createBlockMap(binaryMap2D, poreColor);
    bitBlock2D.saveBitBlockArray_2D("bitblock2D.txt");

    BitBlock bitBlock3D;
    bitBlock3D.createBlockMap(binaryMap3D, poreColor);
    bitBlock3D.saveBitBlockArray_3D("bitblock3D.txt");

    return 0;

	return 0;
}
