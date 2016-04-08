#include <iostream>
#include <string>
#include "Chip8.h"
#include "Display.h"
#include <SDL.h>
#include <SDL_opengl.h>



int main(int argc, char** argv)
{
	Chip8 chip8;
	bool debug = false;
	char* filename;
	WINDOW* debugDisplay;
	char c;
	int opcodesPerSecond;
	int fps = 60;
	int quit = 0;

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
	opcodesPerSecond = 500; //TODO: Change to read from a file or something

	//Checks the debug flag to see what display mode should be used
	//If debug is used, display will be ncurses with register values
	//Otherwise it will be displayed with SDL and OpenGL

	std::cout << "Debug? (y/n)" << std::endl;
	std::string in;
	std::cin >> in;
	if (in.compare("y") == 0)
	{
		debug = true;
	}
	else
	{
		debug = false;
	}


	if (debug)
	{
		//Setup curses
		initscr();
		cbreak();
		noecho();

		debugDisplay = debugSetup();
		debugUpdate(&chip8, debugDisplay);

		while ((c = wgetch(debugDisplay)) && !chip8.programEnd())
		{
			switch (c)
			{
			case 'l':
			{
				delwin(debugDisplay);
				exit(EXIT_SUCCESS);
				break;
			}
			case 'n':
			{
				chip8.emulateCycle();
				chip8.decreaseTimers();
				debugUpdate(&chip8, debugDisplay);
				break;
			}
			default:
				break;
			}
		}
	}
	else //Using SDL
	{
		//Set up display
		SDL_Event event;

		
	}

	exit(EXIT_SUCCESS);
}
