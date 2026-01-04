#ifndef EMULATOR_H
#define EMULATOR_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>



typedef struct 
{
    uint8_t memory[4096]; //um programm in memory zu laden
    size_t memorylength;
    int PC; //stackpointer der auf oppcodes in memory zeigt
    uint8_t V[16]; //registers
    uint8_t indexregisters;
    uint16_t I;
    uint16_t Stack[16];
    uint8_t stackpointer;

    uint8_t display[64 * 32];


}Chip8;

Chip8 initChip8(char *filename);
int execute(Chip8 *chip, int key);
void draw(int x, int y, int height, Chip8 *chip);

#endif
/*


bei Opcode: 0NNN
variablen:  abcd
*/