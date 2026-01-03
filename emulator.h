#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct 
{
    uint16_t *memory; //um programm in memory zu laden
    size_t memorylength;
    int PC; //stackpointer der auf oppcodes in memory zeigt
    uint8_t V[16]; //registers
    uint8_t indexregisters;
    uint16_t I;
    uint16_t Stack[16]

}Chip8;

Chip8 initChip8(char *filename);

#endif
/*


bei Opcode: 0NNN
variablen:  abcd
*/