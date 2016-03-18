#pragma once
#include "Chip8.h"
#include <curses.h>
#define DEBUG_WIDTH 35
#define DEBUG_HEIGHT 20

WINDOW* debugSetup(Chip8* chip8);
void debugUpdate(Chip8* chip8, WINDOW* win);