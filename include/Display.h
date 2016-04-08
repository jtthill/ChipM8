#ifndef DISPLAY_H
#define DISPLAY_H


#include "Chip8.h"
#include <curses.h>
#include <SDL.h>
#include <SDL_opengl.h>
#define DEBUG_WIDTH 35
#define DEBUG_HEIGHT 20
#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 320



//Replacing all GLFW with SDL
bool createSDL();


//Debug display functions
WINDOW* debugSetup();
void debugUpdate(Chip8* chip8, WINDOW* win);

#endif