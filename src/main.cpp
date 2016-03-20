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
	WINDOW* regWin, *debugDisplay;
	GLFWwindow* display;
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

		debugDisplay = displaySetup(&chip8);
		renderDisplay(&chip8, debugDisplay);

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
				if (chip8.drawFlag)
				{
					if (debugMenu)
					{
						debugUpdate(&chip8, regWin);
					}
					else
					{
						renderDisplay(&chip8, debugDisplay);
					}
				}
				break;
			}
			case 'm':
			{
				if (debugMenu)
				{
					delwin(regWin);
					debugMenu = false;
					renderDisplay(&chip8, debugDisplay);
				}
				else
				{
					regWin = debugSetup(debugDisplay);
					debugUpdate(&chip8, regWin);
					debugMenu = true;
				}
				break;
			}
			default:
				break;
			}
		}
	}
	else //Using GLFW
	{
		display = setupWindow(&chip8);
		int width = DISPLAY_WIDTH * DISPLAY_SCALE; 
		int height = DISPLAY_HEIGHT * DISPLAY_SCALE;

		while (!glfwWindowShouldClose(display))
		{
			glfwGetFramebufferSize(display, &width, &height);
			glViewport(0, 0, width, height);
			chip8.emulateCycle();
			if (chip8.drawFlag)
			{
				//draw shit
				glfwSwapBuffers(display);
			}
			glfwPollEvents();
		}
		glfwDestroyWindow(display);
		glfwTerminate();
	}

	exit(EXIT_SUCCESS);
}
