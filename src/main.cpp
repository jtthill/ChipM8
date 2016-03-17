#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include "Chip8.h"
#include "Display.h"

int main(int argc, char** argv)
{
	Chip8 chip8;
	bool debug = false;
	char* filename = "C:\\Users\\Joe\\Documents\\Programming\\ChipM8\\tests\\all_instructions";
	WINDOW* win;
	char c;

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

	//Initialize Chip8 core
	chip8.initialize();
	chip8.loadGame(filename);

	//Setting up curses
	win = debugSetup(&chip8);
	debugUpdate(&chip8, win);
	
	while ((c = wgetch(win)) && !chip8.programEnd())
	{
		switch (c)
		{
		case 'q':
		{
			delwin(win);
			exit(EXIT_SUCCESS);
			break;
		}
		case 'n':
		{
			chip8.emulateCycle();
			debugUpdate(&chip8, win);
		}
		default:
			break;
		}
	}

	exit(EXIT_SUCCESS);
}
