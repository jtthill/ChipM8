#include <GLFW\glfw3.h>
#include <iostream>
#include "Chip8.h"

int main(int argc, char** argv)
{
	const char* filenameL= "tests/all_instructions";
	char exit;
	Chip8 chip8;

	

    chip8.initialize();
    chip8.loadGame(filenameL);
    std::cout << std::hex << (int)chip8.memory[512] << std::endl;
    std::cout << std::hex << (int)chip8.memory[513] << std::endl;
    std::cout << std::hex << 2345 << ": Running 0x4000, skip if not equal." << std::endl;
	return 0;
}
