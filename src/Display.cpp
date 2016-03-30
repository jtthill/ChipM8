#include "Display.h"


GLFWwindow* setupWindow(Chip8* chip8)
{
	if (!glfwInit())
	{
		//Failed GLFW initialization
		exit(EXIT_FAILURE);
	}

	GLFWwindow* window = glfwCreateWindow(DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE, "ChipM8", NULL, NULL);
	if (!window)
	{
		//Failure to create window
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	return window;
}

void createTexture()
{
	// Clear screen data
	for (int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (int x = 0; x < DISPLAY_WIDTH; x++)
		{
			screenData[y][x][2] = screenData[y][x][1] = screenData[y][x][0] = 0;
		}
	}

	// Creating texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)screenData);

	// Set up texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_2D);
}

void updateTexture(Chip8* chip8)
{
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			if(chip8->gfx[(y * 64) + x] == 0)
				screenData[y][x][2] = screenData[y][x][1] = screenData[y][x][0] = 0; //Off
			else
				screenData[y][x][2] = screenData[y][x][1] = screenData[y][x][0] = 255; //On
		}
	}

	// Update texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)screenData);

	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);		glVertex2d(0.0, 0.0);
	glTexCoord2d(1.0, 0.0);		glTexCoord2d(DISPLAY_WIDTH * DISPLAY_SCALE, 0.0);
	glTexCoord2d(1.0, 1.0);		glTexCoord2d(DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE);
	glTexCoord2d(0.0, 1.0);		glTexCoord2d(0.0, DISPLAY_HEIGHT * DISPLAY_SCALE);

	glEnd();
}

void display(Chip8* chip8)
{
	glClear(GL_COLOR_BUFFER_BIT);
	updateTexture(chip8);
}

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static Chip8 chip8;
	chip8.keyCallback(window, key, scancode, action, mods);
}





//////////////////////////////////////////////////////
//Debug display
//////////////////////////////////////////////////////

WINDOW* displaySetup(Chip8* chip8)
{
	WINDOW* win = newwin(0, 0, 0, 0);
	int count = 0;
	for (int col = 0; col < DISPLAY_WIDTH; col++)
	{
		for (int row = 0; row < DISPLAY_HEIGHT; row++)
		{
			wmove(win, row, col);
			if (chip8->gfx[count] = 0)
				waddch(win, '.');
			else
				waddch(win, '-');
			count++;
		}
	}
	wrefresh(win);
	return win;
}

void renderDisplay(Chip8* chip8, WINDOW* win)
{
	int count = 0;
	for (int col = 0; col < DISPLAY_WIDTH; col++)
	{
		for (int row = 0; row < DISPLAY_HEIGHT; row++)
		{
			wmove(win, row, col);
			if (chip8->gfx[count] = 0)
				waddch(win, '.');
			else
				waddch(win, '-');
			count++;
		}
	}
	wrefresh(win);
}

WINDOW* debugSetup(WINDOW* orig)
{
	WINDOW* win = derwin(orig, DEBUG_HEIGHT, DEBUG_WIDTH, 0, 0);
	keypad(win, TRUE);
	wmove(win, 0, 0);
	wprintw(win, "Regist:   Stack:");
	wmove(win, 1, 0);
	wprintw(win, " V0: 00    0: 0000     I: 0000");
	wmove(win, 2, 0);
	wprintw(win, " V1: 00    1: 0000    PC: 0000");
	wmove(win, 3, 0);
	wprintw(win, " V2: 00    2: 0000    SP: 0000");
	wmove(win, 4, 0);
	wprintw(win, " V3: 00    3: 0000    Op: 0000");
	for (int i = 4; i < 10; i++)
	{
		wmove(win, i + 1, 0);
		wprintw(win, " V%d: 00    %d: 0000", i, i);
	}
	for (int i = 10; i < 16; i++)
	{
		wmove(win, i + 1, 0);
		wprintw(win, "V%d: 00   %d: 0000", i, i);
	}
	wrefresh(win);
	return win;
}

void debugUpdate(Chip8* chip8, WINDOW* win)
{
	//First update V and stack registers
	for (int i = 1; i < 16; i++)
	{
		wmove(win, i, 5);
		wprintw(win, "%02x", chip8->V[i - 1]);
		wmove(win, i, 14);
		wprintw(win, "%04x", chip8->stack[i - 1]);
	}

	//Then update I, PC, SP, and Opcode registers
	//I
	wmove(win, 1, 26);
	wprintw(win, "%04x", chip8->I);

	//PC
	wmove(win, 2, 26);
	wprintw(win, "%04x", chip8->pc);

	//SP
	wmove(win, 3, 26);
	wprintw(win, "%04x", chip8->sp);

	//Opcode
	wmove(win, 4, 26);
	wprintw(win, "%04x", chip8->opcode);

	wrefresh(win);
}