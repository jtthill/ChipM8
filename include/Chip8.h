
#ifndef CHIPM8_CHIP8_H
#define CHIPM8_CHIP8_H

#include <string>
#include <fstream>
#include <stdint.h>


class Chip8
{
private:
	int fileSize;
public:

//////////////////////////////////////////////////
// Memory and pointers
//////////////////////////////////////////////////

// Memory map
// 0x000-0x1FF: Chip 8 interpreter
// 0x050-0x0A0: Used for the font set (0-F)
// 0x200-0xFFF: Program ROM and work RAM
    uint8_t memory[4096];
    uint16_t opcode;
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint8_t gfx[64 * 32];
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t stack[16];
    uint16_t sp;
    uint8_t keyStates[16];
	bool drawFlag;

	uint8_t screenData[320][640][3];

//////////////////////////////////////////////////
// CPU methods and operations
//////////////////////////////////////////////////

    void initialize();
    void emulateCycle();
    void loadGame(const char* filename);
	void decreaseTimers();
	bool programEnd();
	void keyPressed(int key);
	void keyReleased(int key);
};


#endif //CHIPM8_CHIP8_H
