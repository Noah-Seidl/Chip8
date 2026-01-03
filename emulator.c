#include "emulator.h"

void printBits(uint16_t value);


Chip8 initChip8(char *filename)
{
    Chip8 chip;

    //read hex File
    FILE *fptr = fopen(filename, "rb");

    if(fptr == NULL)
    return chip;
    
    fseek(fptr,0, SEEK_END);
    long filesize = ftell(fptr);
    rewind(fptr);
    
    printf("Filesize %ld \n", filesize);

    uint8_t *buffer = (uint8_t *) malloc(filesize);

    fread(buffer, 1, filesize, fptr);

    fclose(fptr);

    //convert to uint16 2bte opcode
    long num_opcodes = filesize >> 1;

    //inti Chip struct

    //memory init
    chip.memory = (uint16_t *) malloc(num_opcodes * sizeof(uint16_t));
    for (size_t i = 0; i < num_opcodes; i++)
    {
        //                              zuerst nach link schieben Bigendian
        chip.memory[i] = buffer[i * 2] << 8 | buffer[i * 2 + 1];
    }
    chip.memorylength = num_opcodes;
    
    chip.PC = 0;
    chip.I = 0;


    for (int i = 0; i < 10 && i < chip.memorylength; i++) {
        printf("0x%04X \t", chip.memory[i]);
        printBits(chip.memory[i]);
        printf("\n");
    }

    free(buffer);
    return chip;
}

void execute(Chip8 *chip)
{
    



}


void printBits(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");  // Leerzeichen alle 4 Bits
    }
}