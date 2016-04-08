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
	WINDOW* debugDisplay;
	GLFWwindow* display;
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
	//Otherwise it will be displayed with GLFW and OpenGL

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
	else //Using GLFW
	{
		//Set up display
		display = setupWindow(&chip8);
		int width = DISPLAY_WIDTH * DISPLAY_SCALE; 
		int height = DISPLAY_HEIGHT * DISPLAY_SCALE;
		createTexture();

		//Set up timing
		int numFrame = opcodesPerSecond / fps;
		double interval = 1000;
		interval /= fps;
		double timeNow = glfwGetTime() * 1000; //In milliseconds
		double lastFrame = glfwGetTime() * 1000;

		while (!quit) //Change to proper timing
		{
			glfwGetFramebufferSize(display, &width, &height);
			glViewport(0, 0, width, height);
			glfwPollEvents();
			quit = glfwWindowShouldClose(display);

			timeNow = glfwGetTime() * 1000;
			if ((lastFrame + interval) < timeNow)
			{
				chip8.emulateCycle();
				//if (chip8.drawFlag)
				//{
					//draw
					render(&chip8);
					glfwSwapBuffers(display);
				//}

				timeNow = glfwGetTime() * 1000;
			}
		}
		glfwDestroyWindow(display);
		glfwTerminate();
	}

	exit(EXIT_SUCCESS);
}
