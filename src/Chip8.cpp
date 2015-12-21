//
// Created by jtthill on 12/19/15.
//

#include "Chip8.h"
#include <iostream>
#include <fstream>

void Chip8::initialize()
{
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp= 0;

    for(int i = 0; i < 4096; i++)
    {
        //Clearing memory
        memory[i] = 0;
    }
    for(int i = 0; i < 16; i++)
    {
        //Clearing V registers, stack, and key states
        V[i] = 0;
        stack[i] = 0;
        key[i] = 0;
    }
    for(int i = 0; i < (64 * 32); i++)
    {
        //Clearing pixel states
        gfx[i] = 0;
    }

    delayTimer = 0;
    soundTimer = 0;
}

void Chip8::loadGame(std::string filename)
{
    unsigned char *buffer;
    FILE* file;
    file = fopen(filename, "r");

    if(file)
    {
        std::cout << "Reading ROM file into buffer." << std::endl;

    }
    else
    {
        std::cout << "File couldn't be read." << std::endl;
        //Other error thing?
    }
}

void Chip8::emulateCycle()
{

}