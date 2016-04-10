#include <iostream>
#include <string>
#include "Chip8.h"
#include "Display.h"
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>

void handleInput(Chip8* chip8, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
			case SDLK_x: key = 0; break;
			case SDLK_1: key = 1; break;
			case SDLK_2: key = 2; break;
			case SDLK_3: key = 3; break;
			case SDLK_q: key = 4; break;
			case SDLK_w: key = 5; break;
			case SDLK_e: key = 6; break;
			case SDLK_a: key = 7; break;
			case SDLK_s: key = 8; break;
			case SDLK_d: key = 9; break;
			case SDLK_z: key = 10; break;
			case SDLK_c: key = 11; break;
			case SDLK_4: key = 12; break;
			case SDLK_r: key = 13; break;
			case SDLK_f: key = 14; break;
			case SDLK_v: key = 15; break;
			default: break;
		}
		if (key != -1)
		{
			chip8->keyPressed(key);
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
			case SDLK_x: key = 0; break;
			case SDLK_1: key = 1; break;
			case SDLK_2: key = 2; break;
			case SDLK_3: key = 3; break;
			case SDLK_q: key = 4; break;
			case SDLK_w: key = 5; break;
			case SDLK_e: key = 6; break;
			case SDLK_a: key = 7; break;
			case SDLK_s: key = 8; break;
			case SDLK_d: key = 9; break;
			case SDLK_z: key = 10; break;
			case SDLK_c: key = 11; break;
			case SDLK_4: key = 12; break;
			case SDLK_r: key = 13; break;
			case SDLK_f: key = 14; break;
			case SDLK_v: key = 15; break;
			default: break;
		}
		if (key != -1)
		{
			chip8->keyReleased(key);
		}
	}
}

int main(int argc, char** argv)
{
	Chip8 chip8;
	bool debug = false;
	char* filename;
	WINDOW* debugDisplay;
	char c;
	int opcodesPerSecond;


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
		
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			exit(EXIT_FAILURE);
		}
		SDL_Window* screen = SDL_CreateWindow("ChipM8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_OPENGL);
		SDL_GL_CreateContext(screen);
		initGL();

		SDL_Event event;
		int fps = 60;
		int numframe = opcodesPerSecond / fps;
		bool quit = false;
		float interval = 1000;
		interval /= fps;
		

		unsigned int time2 = SDL_GetTicks();

		while (!quit)
		{
			while (SDL_PollEvent(&event))
			{
				handleInput(&chip8, event);
				
				if (event.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			unsigned int current = SDL_GetTicks();

			if ((time2 + interval) < current)
			{
				chip8.decreaseTimers();
				for (int i = 0; i < numframe; i++)
					chip8.emulateCycle();

				time2 = current;
				render(&chip8, screen);
			}
		}
	}
	SDL_Quit();
	exit(EXIT_SUCCESS);
}
