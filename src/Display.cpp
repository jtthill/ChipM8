#include "Display.h"

bool createSDL()
{
	//if (SDL_Init(SDL_INIT_EVERYTHING))
	//{
		//return false;
	//}
	//if (SDL_SetVideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 8, SDL_OPENGL) == NULL)
	//{
		//return false;
	//}

	//initGL();
	//SDL_WM_SetCaption("ChipM8", NULL);
	return true;
}

void initGL()
{
	glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, -1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
}

void render(Chip8* chip8, SDL_Window* screen)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, chip8->screenData);
	SDL_GL_SwapWindow(screen);
	glFlush();
}

//////////////////////////////////////////////////////
//Debug display
//////////////////////////////////////////////////////

WINDOW* debugSetup()
{
	WINDOW* win = newwin(0, 0, 0, 0);
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