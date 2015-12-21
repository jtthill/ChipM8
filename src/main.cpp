#include <iostream>
#include "Chip8.h"

using namespace std;
int main()
{
	const char* filename= "/home/jtthill/Documents/Programming/ChipM8/ROMs/c8games/PONG";
	Chip8 chip8;

    chip8.initialize();
    chip8.loadGame(filename);
    cout << chip8.memory[512];
    cout << chip8.memory[514];
	return 0;
}