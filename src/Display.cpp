#include "Display.h"


WINDOW* screenSetup(Chip8* chip8)
{
	initscr();
	cbreak();
	noecho();

	WINDOW* win = newwin(HEIGHT, WIDTH, 1, 1);
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