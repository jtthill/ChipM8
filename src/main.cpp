#include <iostream>
#include "Chip8.h"

using namespace std;
int main(int argc, char** argv)
{
	const char* filenameL= "../ROMs/PONG";
    const char* filenameW= "..\\ROMs\\PONG";
	Chip8 chip8;

    chip8.initialize();
    chip8.loadGame(filenameL);
    chip8.traceLog("Hello world!", 2345);
    cout << hex << chip8.memory[0x200] << endl;
    cout << hex << chip8.memory[0x201] << endl;
	return 0;
}