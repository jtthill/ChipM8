#ifndef DISPLAY_H
#define DISPLAY_H


#include "Chip8.h"
#include <curses.h>
#include <glfw3.h>
#define DEBUG_WIDTH 35
#define DEBUG_HEIGHT 20
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10


//GLFW display functions
//Replace with SDL
GLFWwindow* setupWindow(Chip8* chip8);
void createTexture();
void updateTexture(Chip8* chip8);
void render(Chip8* chip8);


//GLFW callback functions
void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Debug display functions

WINDOW* debugSetup();
void debugUpdate(Chip8* chip8, WINDOW* win);

#endif