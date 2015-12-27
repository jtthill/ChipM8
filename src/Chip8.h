
#ifndef CHIPM8_CHIP8_H
#define CHIPM8_CHIP8_H

#include <string>


class Chip8
{
public:

//////////////////////////////////////////////////
// Memory and pointers
//////////////////////////////////////////////////

// Memory map
// 0x000-0x1FF: Chip 8 interpreter
// 0x050-0x0A0: Used for the font set (0-F)
// 0x200-0xFFF: Program ROM and work RAM
    unsigned char memory[4096];
    unsigned short opcode;
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delayTimer;
    unsigned char soundTimer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];

//////////////////////////////////////////////////
// CPU methods and operations
//////////////////////////////////////////////////

    void initialize();
    void emulateCycle();
    void loadGame(const char* filename);

};


#endif //CHIPM8_CHIP8_H
