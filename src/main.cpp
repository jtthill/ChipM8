#include <glfw3.h>
#include <iostream>
#include <string>
#include "Chip8.h"
#include "Display.h"

int main(int argc, char** argv)
{
	Chip8 chip8;
	bool debug = false; bool debugMenu = false;
	char* filename = "..\\..\\ROMS\\PONG";
	WINDOW* regWin, *displayWin;
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

	//Checks the debug flag to see what display mode should be used
	//If debug is used, display will be ncurses with register values
	//Otherwise it will be displayed with GLFW and OpenGL
	if (debug)
	{
		//Setup curses
		initscr();
		cbreak();
		noecho();

		displayWin = displaySetup(&chip8);
		renderDisplay(&chip8, displayWin);

		while ((c = wgetch(displayWin)) && !chip8.programEnd())
		{
			switch (c)
			{
			case 'l':
			{
				delwin(displayWin);
				exit(EXIT_SUCCESS);
				break;
			}
			case 'n':
			{
				chip8.emulateCycle();
				renderDisplay(&chip8, displayWin);
			}
			case 'm':
			{
				if (debugMenu)
				{
					debugMenu = false;
					wrefresh(displayWin);
				}
				else
				{
					debugMenu = true;
				}
			}
			default:
				break;
			}
		}
	}

	exit(EXIT_SUCCESS);
}
