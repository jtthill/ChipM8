#include "Chip8.h"
#include <iostream>
#include <fstream>

void Chip8::initialize()
{
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    for (int i = 0; i < 4096; i++)
    {
        //Clearing memory
        memory[i] = 0;
    }
    for (int i = 0; i < 16; i++)
    {
        //Clearing V registers, stack, and key states
        V[i] = 0;
        stack[i] = 0;
        key[i] = 0;
    }
    for (int i = 0; i < (64 * 32); i++)
    {
        //Clearing pixel states
        gfx[i] = 0;
    }

    delayTimer = 0;
    soundTimer = 0;

    try
    {
        tracefile.open("../logs/tracelog.txt");
    }
    catch(std::exception e)
    {
        try
        {
            tracefile.open("..\\logs\\tracelog.txt");
        }
        catch (std::exception e)
        {
            throw "Trace log couldn't be opened.\n";
        }

    }
}

void Chip8::loadGame(const char *filename)
{
    uint8_t buffer[4096 - 512];
    FILE *file;
    file = fopen(filename, "r");

    if (file)
    {
        //TODO load directly into memory rather than into buffer?
        std::cout << "Reading ROM file into buffer." << std::endl;
        fread(buffer, sizeof(uint8_t), 4096 - 512, file);
        std::cout << "Loading from buffer into memory" << std::endl;
        int bufferSize = sizeof(buffer) / sizeof(uint8_t);
        for (int i = 0; i < bufferSize; i++)
        {
            memory[512 + i] = buffer[i];
        }
    }
    else
    {
        std::cerr << "File couldn't be read." << std::endl;
        //Other error thing?
    }
}

void Chip8::emulateCycle()
{
    //Run the basic emulation cycle:
    //  -Fetch
    //  -Decode
    //  -Execute

    //Fetch current opcode
    //Since each memory location is one byte, and opcodes are two bytes
    //the next two memory bytes are combined into two bytes by shifting
    //the first left by 8 bits, and then OR-ing the current and next byte.
    opcode = memory[pc] << 8 | memory[pc + 1];

    //Decode the opcode
    //Using a large switch-case to find which opcode needs to be run
    //Once the proper opcode is found, it will execute the proper operations

    //TODO Change switch case to an array of function pointers
    //Easier to manage and is faster

    switch (opcode & 0xF000) //Only the first 4 bits are considered for most opcodes
    {
        case 0x0000:
            //Multiple 0x0000 opcodes
            switch (opcode & 0x00FF)
            {
                case 0x00E0: //0x00E0
                    //TODO Clear the display
                    std::cout << std::hex << opcode << ": Running 0x00E0, clearing display." << std::endl;
                    break;
                case 0x00EE: //0x00EE
                    //TODO Return from a subroutine
                    std::cout << std::hex << opcode << ": Running 0x00EE, return from subroutine." << std::endl;
                    //Set pc to the address at the top of the stack, then decrement sp
                    break;
                default:
                {
                    std::cerr << "Opcode not recognized. Skipping..." << std::endl;
                    pc += 2;
                    break;
                }
            }
            break;
        case 0x1000: //0x1nnn
        {
            //Jump to location nnn. Set pc to nnn
            std::cout << std::hex << opcode << ": Running 0x1nnn, Jump to location nnn." << std::endl;
            pc = (uint16_t) (opcode & 0x0FFF);
            break;
        }
        case 0x2000: //0x2nnn
        {
            //Call subroutine at nnn.
            //Increment sp, put current pc on stack, pc set to nnn
            std::cout << std::hex << opcode << ": Running 0x2nnn, calling subroutine at nnn." << std::endl;
            stack[sp] = pc;
            sp++;
            pc = (uint16_t) (opcode & 0x0FFF);
            break;
        }
        case 0x3000: //0x3xkk
        {
            //Skip next instruction if Vx == kk. If condition is true, increment pc by 2.
            std::cout << std::hex << opcode << ": Running 0x3xkk, skip if Vx == kk." << std::endl;
            uint8_t kk = (uint8_t) (opcode & 0x00FF);
            if (V[opcode >> 8 & 0x0F] == kk)
                pc += 2;
            pc += 2;
            break;
        }
        case 0x4000: //0x4xkk
        {
            //Skip next instruction if Vx != kk. If condition is true, increment pc by 2.
            std::cout << std::hex << opcode << ": Running 0x4xkk, skip if Vx != kk." << std::endl;
            uint8_t kk = (uint8_t) (opcode & 0x00FF);
            if (V[(opcode & 0x0F00) >> 8] != kk)
                pc += 2;
            pc += 2;
            break;
        }
        case 0x5000: //0x5xy0
        {
            //Skip next instruction if Vx == Vy.
            //Compares registers Vx and Vy, if they are equal, increment pc by 2.
            std::cout << std::hex << opcode << ": Running 0x5xy0, skip if Vx == Vy." << std::endl;
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 2;
            pc += 2;
            break;
        }
        case 0x6000: //0x6xkk
        {
            //Sets value kk into register Vx.
            std::cout << std::hex << opcode << ": Running 0x6xkk, set Vx = kk." << std::endl;
            uint8_t kk = (uint8_t) (opcode & 0x00FF);
            V[(opcode & 0x0F00) >> 8] = kk;
            pc += 2;
            break;
        }
        case 0x7000: //0x7xkk
        {
            //Set register Vx to Vx + kk
            std::cout << std::hex << opcode << ": Running 0x7xkk, add kk to Vx." << std::endl;
            uint8_t kk = (uint8_t)(opcode & 0x00FF);
            V[(opcode & 0x0F00) >> 8] += kk;
            pc += 2;
            break;
        }
        case 0x8000:
            //Multiple 0x8000 codes
            switch (opcode & 0x000F)
            {
                case 0x0000: //0x8xy0
                    //Set register Vx to the value of Vy
                    std::cout << std::hex << opcode << ": Running 0x8xy0, set Vx = Vy." << std::endl;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0001: //0x8xy1
                    //Set register Vx to the value of Vx OR Vy
                    std::cout << std::hex << opcode << ": Running 0x8xy1, set Vx = Vx OR Vy." << std::endl;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0002: //0x8xy2
                    //Set register Vx to the value of Vx AND Vy
                    std::cout << std::hex << opcode << ": Running 0x8xy2, set Vx = Vx AND Vy." << std::endl;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    break;
                case 0x0003: //0x8xy3
                    //Set register Vx to the value of Vx XOR Vy
                    std::cout << std::hex << opcode << ": Running 0x8xy3, set Vx = Vx XOR Vy." << std::endl;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    break;
                case 0x0004: //0x8xy4
                    //Set register Vx to the value of Vx + Vy
                    //Set VF (carry) to 1 if the value is greater than 255, otherwise set to 0
                    std::cout << std::hex << opcode << ": Running 0x8xy4, set Vx = Vx + Vy." << std::endl;
                    break;
                case 0x0005: //0x8xy5
                    //Set register Vx to the value of Vx - Vy
                    //Set VF to 1 if Vx > Vy, otherwise set to 0
                    std::cout << std::hex << opcode << ": Running 0x8xy5, set Vx = Vx - Vy." << std::endl;
                    break;
                case 0x0006: //0x8xy6
                    //If the least-significant bit of Vx is 1, then the VF is set to 1, otherwise 0
                    //Then Vx is right shifted by dividing by 2.
                    std::cout << std::hex << opcode << ": Running 0x8xy6, right shift Vx." << std::endl;
                    break;
                case 0x0007: //0x8xy7
                    //Set register Vx to the value of Vy - Vx
                    //Set VF to 1 if Vy > Vx, otherwise set to 0
                    std::cout << std::hex << opcode << ": Running 0x8xy7, set Vx = Vy - Vx." << std::endl;
                    break;
                case 0x000E: //0x8xyE
                    //If the most-significant bit of Vx is 1, then VF is set to 1, otherwise 0
                    //Then Vx is left shifted by multiplying by 2
                    std::cout << std::hex << opcode << ": Running 0x8xyE, left shift Vx." << std::endl;
                    break;
                default:
                    std::cerr << "Opcode not recognized. Skipping..." << std::endl;
                    pc += 2;
                    break;
            }
            break;
        case 0x9000: //0x9xy0
            //Skips next instruction if Vx != Vy, so if condition is true, increment pc by 2
            std::cout << std::hex << opcode << ": Running 0x9xy0, skip if Vx != Vy." << std::endl;
            break;
        case 0xA000: //0xAnnn
            //Set register I to nnn.
            std::cout << std::hex << opcode << ": Running 0xAnnn, set I = nnn." << std::endl;
            break;
        case 0xB000: //0xBnnn
            //Jump to location nnn + V0
            //pc is set to the value of nnn + value of V0
            std::cout << std::hex << opcode << ": Running 0xBnnn, set pc to nnn + V0." << std::endl;
            break;
        case 0xC000: //0xCxkk
            //Set register Vx to the value of a random number 0 <= num <= 255 plus kk
            std::cout << std::hex << opcode << ": Running 0xCxkk, set Vx = random number + kk." << std::endl;
            break;
        case 0xD000: //0xDxyn
            //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
            //Width of sprites are 8 bits
            std::cout << std::hex << opcode << ": Running 0xDxyn, display n-byte sprite" << std::endl;
            break;
        case 0xE000:
            //Two 0xE000 opcodes
            switch (opcode & 0x000F)
            {
                case 0x000E: //0xEx9E
                    //Skip next instruction if key with value of Vx is pressed
                    //If key corresponding to Vx is currently down, pc is incremented by 2.
                    std::cout << std::hex << opcode << ": Running 0xEx9E, skip if key Vx is pressed." << std::endl;
                    break;
                case 0x0001: //0xExA1
                    //Skip next instruction if key with value of Vx is not pressed
                    //If key corresponding to Vx is currently up, pc is incremented by 2.
                    std::cout << std::hex << opcode << ": Running 0xExA1, skip if key Vx isn't pressed." << std::endl;
                    break;
                default:
                    std::cerr << "Opcode not recognized. Skipping..." << std::endl;
                    pc += 2;
                    break;
            }
            break;
        case 0xF000:
            //Multiple 0xF000 opcodes
            switch (opcode & 0x00FF)
            {
                case 0x0007: //0xFx07
                    //Set Vx = delay timer value.
                    std::cout << std::hex << opcode << ": Running 0xFx07, set Vx = delay timer val." << std::endl;
                    break;
                case 0x000A: //0xFx0A
                    //Wait for a key press, store the value of the key in Vx
                    //Halt all execution until that key is pressed
                    std::cout << std::hex << opcode << ": Running 0xFx0A, wait for key press and store in Vx." << std::endl;
                    break;
                case 0x0015: //0xFx15
                    //Set delay timer = Vx
                    //Opposite of Fx07, sets the value of the delay timer to be the value of Vx
                    std::cout << std::hex << opcode << ": Running 0xFx15, set delay timer val = Vx." << std::endl;
                    break;
                case 0x0018: //0xFx18
                    //Set sound timer = Vx
                    std::cout << std::hex << opcode << ": Running 0xFx18, set sound timer = Vx." << std::endl;
                    break;
                case 0x001E: //0xFx1E
                    //Set I = I + Vx
                    //I and Vx are added together, and stored in I
                    std::cout << std::hex << opcode << ": Running 0xFx1E, set I = I + Vx." << std::endl;
                    break;
                case 0x0029: //0xFx29
                    //Set I = location of sprite for digit Vx
                    std::cout << std::hex << opcode << ": Running 0xFx29, set I = location of sprite for digit Vx." << std::endl;
                    break;
                case 0x0033: //0xFx33
                    //Store BCD representation of Vx in memory locations I, I+1, I+2
                    //Take decimal value of Vx, put hundreds digit in I, tens in I+1, ones in I+2
                    std::cout << std::hex << opcode << ": Running 0xFx33, store BCD representation of Vx." << std::endl;
                    break;
                case 0x0055: //0xFx55
                    //Store registers V0 through Vx in memory starting at location I
                    //Copies the values of registers V0 through Vx into memory
                    std::cout << std::hex << opcode << ": Running 0xFx55, store register V0-Vx in memory." << std::endl;
                    break;
                case 0x0065: //0xFx65
                    //Read registers V0 through Vx from memory starting at location I.
                    //Reads memory values starting at I into registers V0 through Vx
                    std::cout << std::hex << opcode << ": Running 0xFx65, read memory values into registers V0-Vx." << std::endl;
                    break;
                default:
                    std::cerr << "Opcode not recognized. Skipping..." << std::endl;
                    pc += 2;
                    break;
            }
            break;
        default:
            std::cerr << "Opcode not recognized. Skipping..." << std::endl;
            pc += 2;
            break;
    }

    //Timers decrement by one every cycle when set to value above zero
    //If sound timer reaches zero, it plays a beep
    if (delayTimer > 0)
        delayTimer--;
    if (soundTimer > 0)
    {
        std::cout << "BEEP" << std::endl;
        soundTimer--;
    }
}

void Chip8::traceLog(const char* message, uint16_t opcode)
{
    tracefile << "0x" << std::hex << opcode << ": " << message << std::endl;
}
