all: src/Chip8.cpp src/Chip8.h src/main.cpp
	g++ -o ChipM8 src/Chip8.cpp src/Chip8.h src/main.cpp
clean:
	rm -f ChipM8 *.o
