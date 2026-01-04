#include "emulator.h"

void printBits(uint16_t value);
void dispclear(Chip8 *chip);
void initFontset(Chip8 *chip);


Chip8 initChip8(char *filename)
{
    Chip8 chip;
    memset(&chip, 0, sizeof(Chip8)); 
    //read hex File
    FILE *fptr = fopen(filename, "rb");

    if(fptr == NULL)
    return chip;
    
    fseek(fptr,0, SEEK_END);
    long filesize = ftell(fptr);
    rewind(fptr);
    
    printf("Filesize %ld \n", filesize);


    fread(&chip.memory[0x200], 1, filesize, fptr);

    fclose(fptr);

    initFontset(&chip);

    //inti Chip struct
    chip.PC = 0x200;
    chip.I = 0;
    chip.stackpointer = 0;
    chip.waitingforkey = -1;
    dispclear(&chip);



    for (int i = 0x200; i < 10 + 0x200 && i < chip.memorylength; i++) {
        printf("0x%04X \t", chip.memory[i]);
        printBits(chip.memory[i]);
        printf("\n");
    }
    srand(time(NULL));

    return chip;
}

int execute(Chip8 *chip,int key)
{
    printf("Execute\n");
    if (chip->timer > 0)
    {
        chip->timer--;
    }
    
    if(chip->waitingforkey >= 0)
    {
        printf("KEYWAITING");
        if(key != 0)   
        {
            chip->V[chip->waitingforkey] = key;
            chip->PC += 2;
            chip->waitingforkey = -1;
        }else
            return 0;
    }
    uint16_t opcode = chip->memory[chip->PC] << 8 | chip->memory[chip->PC + 1];
    
    if(opcode == 0x00E0)
    {
        //DISPLAY clear
        dispclear(chip);
        chip->PC += 2;
        return 20;
    }


    if(opcode == 0x00EE)
    {
        chip->PC = chip->Stack[--chip->stackpointer];
        chip->PC += 2;
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
        //Jumps to address
        case 1:
            chip->PC = NNN;
            return 0;

        //calls to subroutine
        case 2:
            printf("Address: %d \n", NNN);
            chip->Stack[chip->stackpointer++] = chip->PC;
            chip->PC = NNN;
            return 0;

        case 3:
            if(chip->V[x] == NN)
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        case 4: 
            if(chip->V[x] != NN)
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        case 5:
            if(chip->V[x] == chip->V[y])
                chip->PC += 2;
            chip->PC += 2;
            return 0;


        case 6:
            chip->V[x] = NN;
            chip->PC += 2;
            return 0;

        case 7:
            chip->V[x] += NN;
            chip->PC += 2;
            return 0;

        case 8:
            switch (d)
            {
            case 0:
                chip->V[x] = chip->V[y];
                chip->PC += 2;
                return 0;

            case 1:
                chip->V[x] = chip->V[x] | chip->V[y];
                chip->PC += 2;
                return 0;
            
            case 2:
                chip->V[x] = chip->V[x] & chip->V[y];
                chip->PC += 2;
                return 0;
            
            case 3:
                chip->V[x] = chip->V[x] ^ chip->V[y];
                chip->PC += 2;
                return 0;

            case 4:
                if(chip->V[x] + chip->V[y] > 255)
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] += chip->V[y];

                chip->PC += 2;
                return 0;

            case 5:
                if(chip->V[x] >= chip->V[y])
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] -= chip->V[y];
                chip->PC += 2;
                return 0;

            case 6:
                chip->V[15] = chip->V[x] & 1;
                chip->V[x] >>= 1;

                chip->PC += 2;
                return 0;

            case 7:
                if(chip->V[y] >= chip->V[x])
                    chip->V[15] = 1;
                else
                    chip->V[15] = 0;

                chip->V[x] = chip->V[y] - chip->V[x];

                chip->PC += 2;
                return 0;

            case 15:
                chip->V[15] = (chip->V[x] >> 7) & 1;    
                chip->V[x] <<= 1;

                chip->PC += 2;
                return 0;

            default:
                break;
            }

        case 9:
            if(chip->V[x] != chip->V[y])
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        case 10: 
            chip->I = NNN;
            chip->PC += 2;
            return 0;

        case 11:
            chip->PC = chip->V[0] + NNN;
            return 0;

        case 12:
            int random = rand() % 256;
            chip->V[x] = random & NN;
            chip->PC += 2;
            return 0;

        case 13:
            chip->V[0xF] = 0;
            int px = chip->V[x];     // Wert von VX
            int py = chip->V[y];     // Wert von VY
            draw(px,py,d, chip);
            chip->PC += 2;
            return 0;            

        case 14:
            printf("TASTE: %d | KEY: %d\n", chip->V[x], key);

            if(y == 9 && d == 0xE)
                if(key == chip->V[x])
                    chip->PC += 2;

            if(y == 0xA && d == 1)
                if(key != chip->V[x])
                    chip->PC += 2;

            chip->PC += 2;
            return 0;

        case 15:
            switch (NN)
            {
            case 0x07:
                chip->V[x] = chip->timer;
                chip->PC += 2;
                return 0;

            case 0x0A:
                chip->waitingforkey = x;
                return 0;

            case 0x15:
                chip->timer = chip->V[x];
                chip->PC += 2;
                return 0;  

            case 0x18:
                chip->PC += 2;
                return 0;  

            case 0x1E:
                chip->I += chip->V[x];
                chip->PC += 2;
                return 0;  
                
            case 0x29:
                chip->I = 0x050 + (chip->V[x] & 0x0F) * 5;
                chip->PC += 2;
                return 0;   

            case 0x33:
                chip->memory[chip->I] = chip->V[x] / 100;
                chip->memory[chip->I + 1] = (chip->V[x] / 10) % 10;
                chip->memory[chip->I + 2] = chip->V[x] % 10;
                chip->PC += 2;
                return 0;   

            case 0x55:
                for (uint16_t i = 0; i <= x; i++)
                {
                    chip->memory[chip->I + i] = chip->V[i];
                }
                chip->PC += 2;
                return 0;   
            case 0x65:
                for (uint16_t i = 0; i <= x; i++)
                {
                    chip->V[i] = chip->memory[chip->I + i];
                }
                chip->PC += 2;
                return 0;   

            default:
                break;
            }                

    default:
        break;
    }




    return 1;
}

void dispclear(Chip8 *chip)
{
    for (int i = 0; i < 64 * 32; i++)
    {
        chip->display[i] = 0;
    }
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


void initFontset(Chip8 *chip)
{
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };
    memcpy(&chip->memory[0x50], fontset, 80);
}