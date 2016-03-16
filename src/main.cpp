#include <GLFW\glfw3.h>
#include <iostream>
#include "Chip8.h"

int main(int argc, char** argv)
{
	const char* filenameL= "C:\\Users\\Joe\\Documents\\Programming\\ChipM8\\tests\\all_instructions";
	Chip8 chip8;

    chip8.initialize();
    chip8.loadGame(filenameL);
	while (!chip8.programEnd())
	{
		chip8.emulateCycle();
	}

	exit(EXIT_SUCCESS);
}
