#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once
#include "Chip8.h"
#include <curses.h>
#include <glfw3.h>
#define DEBUG_WIDTH 35
#define DEBUG_HEIGHT 20
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10

//GLFW display functions
GLFWwindow* setupWindow(Chip8* chip8);


//GLFW callback functions
void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Debug display functions

WINDOW* displaySetup(Chip8* chip8);
void renderDisplay(Chip8* chip8, WINDOW* win);

WINDOW* debugSetup(WINDOW* orig);
void debugUpdate(Chip8* chip8, WINDOW* win);

#endif