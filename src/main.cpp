#include <iostream>
#include "Chip8.h"

using namespace std;
int main(int argc, char** argv)
{
	const char* filenameL= "../tests/all_instructions";
	Chip8 chip8;

    chip8.initialize();
    chip8.loadGame(filenameL);
    cout << hex << (int)chip8.memory[512] << endl;
    cout << hex << (int)chip8.memory[513] << endl;
    std::cout << std::hex << 2345 << ": Running 0x4000, skip if not equal." << std::endl;
	return 0;
}
