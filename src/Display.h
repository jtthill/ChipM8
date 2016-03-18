#pragma once
#include "Chip8.h"
#include <curses.h>
#define DEBUG_WIDTH 35
#define DEBUG_HEIGHT 20
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

//Temporary display using ncurses. 
//Eventually use GLFW
WINDOW* displaySetup(Chip8* chip8);
void renderDisplay(Chip8* chip8, WINDOW* win);

WINDOW* debugSetup();
void debugUpdate(Chip8* chip8, WINDOW* win);