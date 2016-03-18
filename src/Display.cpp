#include "Display.h"

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

WINDOW* debugSetup()
{
	WINDOW* win = newwin(DEBUG_HEIGHT, DEBUG_WIDTH, 1, 1);
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
}