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
    for (size_t i = 0; i < num_opcodes; i++)
    {
        //                              zuerst nach link schieben Bigendian
        chip.memory[i] = buffer[i * 2] << 8 | buffer[i * 2 + 1];
    }
    chip.memorylength = num_opcodes;
    
    chip.PC = 0;
    chip.I = 0;
    chip.stackpointer = 0;

    for (int i = 0; i < 10 && i < chip.memorylength; i++) {
        printf("0x%04X \t", chip.memory[i]);
        printBits(chip.memory[i]);
        printf("\n");
    }

    free(buffer);
    return chip;
}

int execute(Chip8 *chip,int key)
{
    srand(time(NULL));

    uint16_t opcode = chip->memory[chip->PC];
    
    if(opcode == 12 * 16)
    {
        //DISPLAY clear
        return 20;
    }


    if(opcode == 12 * 16 + 12)
    {
        chip->PC = chip->Stack[--chip->stackpointer];
        return 1;
    }





    uint8_t a,x,y,d;
    a = opcode>>12; 
    x = (opcode>>8) & 15; 
    y = (opcode>>4) & 15; 
    d = (opcode) & 15; 

    uint16_t NNN = (opcode & 0x0FFF);
    uint16_t NN = (opcode & 0x00FF);

    printf("%d | %d | %d | %d\n",a,x,y,d);



    switch (a)
    {
        case 1:
            chip->PC = NNN;
            return 0;
        //calls to subroutine
        case 2:
            printf("Address: %d \n", NNN);
            chip->PC = NNN / 2;
            chip->Stack[chip->stackpointer++] = chip->PC;
            return 0;

        case 3:
            if(chip->V[x] == NN)
                chip->PC++;
            chip->PC++;
            return 0;

        case 4: 
            if(chip->V[x] != NN)
                chip->PC++;
            chip->PC++;
            return 0;

        case 5:
            if(chip->V[x] == chip->V[y])
                chip->PC++;
            chip->PC++;
            return 0;


        case 6:
            chip->V[x] = NN;
            chip->PC++;
            return 0;

        case 7:
            chip->V[x] += NN;
            chip->PC++;
            return 0;

        case 8:
            switch (d)
            {
            case 0:
                chip->V[x] = chip->V[y];
                chip->PC++;
                return 0;

            case 1:
                chip->V[x] = chip->V[x] | chip->V[y];
                chip->PC++;
                return 0;
            
            case 2:
                chip->V[x] = chip->V[x] & chip->V[y];
                chip->PC++;
                return 0;
            
            case 3:
                chip->V[x] = chip->V[x] ^ chip->V[y];
                chip->PC++;
                return 0;

            case 4:
                if(chip->V[x] + chip->V[y] > 255)
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] += chip->V[y];

                chip->PC++;
                return 0;

            case 5:
                if(chip->V[x] - chip->V[y] < 0)
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] -= chip->V[y];

                chip->PC++;
                return 0;

            case 6:
                chip->V[15] = chip->V[x] & 1;
                chip->V[x] >>= 1;

                chip->PC++;
                return 0;

            case 7:
                if(chip->V[y] - chip->V[x]< 0)
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] = chip->V[y] - chip->V[x];

                chip->PC++;
                return 0;

            case 15:
                chip->V[15] = (chip->V[x] >> 7) & 1;    
                chip->V[x] <<= 1;

                chip->PC++;
                return 0;

            default:
                break;
            }

        case 9:
            if(chip->V[x] != chip->V[y])
                chip->PC++;
            chip->PC++;
            return 0;

        case 10: 
            chip->I = NNN;
            chip->PC++;
            return 0;

        case 11:
            chip->PC = chip->V[0] + NNN;
            return 0;

        case 12:
            int random = rand() % 256;
            chip->V[x] = random & NN;
            chip->PC++;
            return 0;

        case 13:
            draw(x,y,d, chip);
            chip->PC++;
            return 0;            



    default:
        break;
    }




    return 1;
}

void draw(int x, int y, int height, Chip8 *chip)
{
    for(int row = 0; row < height; row++)
    {
        uint8_t sprite = chip->memory[chip->I + row];

        for (int col = 0; col < 8; col++)
        {
            if ((sprite >> (7 - col)) & 1)
            {
                int px = (x + col) % 64;
                int py = (y + row) % 32;
                int index = py * 64 + px;

                if (chip->display[index] == 1)
                    chip->V[0xF] = 1;

                chip->display[index] ^= 1;
            }
        }
        
    }
}



void printBits(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");  // Leerzeichen alle 4 Bits
    }
}