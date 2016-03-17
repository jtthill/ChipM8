#include <GLFW\glfw3.h>
#include <curses.h>
#include <iostream>
#include <string>
#include "Chip8.h"

int main(int argc, char** argv)
{
	Chip8 chip8;
	bool debug = false;
	char* filename = "C:\\Users\\Joe\\Documents\\Programming\\ChipM8\\tests\\all_instructions";

	if (argc > 1)
	{
		//Parse arguments
		for (int i = 1; i < argc; i++)
		{
			std::string arg = argv[i];
			if (arg.compare("-d") == 0)
			{
				//Debug flag
				debug = true;
			}
			else if (arg.compare("-e") == 0)
			{
				//File to execute
				filename = argv[++i];
			}
		}
	}
	else
	{
		std::cout << "No arguments given." << std::endl;
	}

	//Setting up curses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	
    chip8.initialize();
    chip8.loadGame(filename);
	while (!chip8.programEnd())
	{
		chip8.emulateCycle();
	}

	exit(EXIT_SUCCESS);
}
