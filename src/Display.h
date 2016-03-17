#pragma once
#include "Chip8.h"
#include <curses.h>
#define WIDTH 30
#define HEIGHT 20

WINDOW* screenSetup(Chip8* chip8);