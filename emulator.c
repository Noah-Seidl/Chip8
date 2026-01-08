#include "emulator.h"

void printBits(uint16_t value);
void dispclear(Chip8 *chip);
void initFontset(Chip8 *chip);

//init des Chip8
Chip8 initChip8(char *filename)
{
    Chip8 chip;
    //Sicherstellung das Alle varaiblen im struct mit 0 mal initialisiert werden
    memset(&chip, 0, sizeof(Chip8)); 

    //read hex File
    FILE *fptr = fopen(filename, "rb");

    if(fptr == NULL)
    return chip;
    
    //Drei zeilen um herauszubekommen wie groß die datei ist
    fseek(fptr,0, SEEK_END);
    long filesize = ftell(fptr);
    rewind(fptr);
    
    printf("Filesize %ld \n", filesize);

    //lest game File ab 0x200 da davor der speicher für fonts stehen
    fread(&chip.memory[0x200], 1, filesize, fptr);

    fclose(fptr);

    //Fonts werden von vielen Games benötigt
    initFontset(&chip);

    //inti Chip struct
    //eigentlich doppelt init ist wahrscheinlich nicht nötig
    chip.PC = 0x200;
    chip.waitingforkey = -1;

    dispclear(&chip);

    //Rand seed wird hier erstellt
    srand(time(NULL));

    return chip;
}

int execute(Chip8 *chip,int key)
{
    //Chip timer wird immer runtergezählt außer er ist bereits 0
    if (chip->timer > 0)
    {
        chip->timer--;
    }
    
    //funktion um wirklich auf key zu warten sodass programm weiterlaufen kann
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

    //hier wird der Opcde in eine variable gespeichert weil er 2 Bytes groß ist
    uint16_t opcode = chip->memory[chip->PC] << 8 | chip->memory[chip->PC + 1];
    
    //Könnte man auch in Switch packen
    if(opcode == 0x00E0)
    {
        //DISPLAY clear
        dispclear(chip);
        chip->PC += 2;
        return 20;
    }

    //kommt von eine subroutine zurück nimmt daher den PC vom stack wieder runter 
    if(opcode == 0x00EE)
    {
        chip->PC = chip->Stack[--chip->stackpointer];
        chip->PC += 2;
        return 1;
    }




    //initialisierung der einzelnen bytes des opcodes um leichter damit umgehen zu können könnte auch anders leichte gemacht werden
    uint8_t a,x,y,d;
    a = opcode>>12; 
    x = (opcode>>8) & 15; 
    y = (opcode>>4) & 15; 
    d = (opcode) & 15; 

    uint16_t NNN = (opcode & 0x0FFF);
    uint16_t NN = (opcode & 0x00FF);

    //Hier würde man den opcode angezeigt bekommen
    //printf("%d | %d | %d | %d\n",a,x,y,d);



    switch (a)
    {
        //Jumps to address
        case 0x01:
            chip->PC = NNN;
            return 0;

        //calls to subroutine legt den jetztigen PC auf den stack um dann wieder zurückkommen zu können
        case 0x02:
            printf("Address: %d \n", NNN);
            chip->Stack[chip->stackpointer++] = chip->PC;
            chip->PC = NNN;
            return 0;

        //überspringt den nächsten opcode wenn Vx == NN ist
        case 0x03:
            if(chip->V[x] == NN)
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        //überspringt den nächsten opcode wenn Vx != NN ist
        case 0x04: 
            if(chip->V[x] != NN)
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        //überspringt den nächsten opcode wenn Vx == Vy ist
        case 0x05:
            if(chip->V[x] == chip->V[y])
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        //Schreibt wert NN in Vx
        case 0x06:
            chip->V[x] = NN;
            chip->PC += 2;
            return 0;

        //addiert NN: Vx = Vx + NN
        case 0x07:
            chip->V[x] += NN;
            chip->PC += 2;
            return 0;

        //weitere opcodes mit a = 0x08: die meisten sind selbsterklärend
        case 0x08:
            switch (d)
            {
                case 0x00:
                    chip->V[x] = chip->V[y];
                    chip->PC += 2;
                    return 0;

                case 0x01:
                    chip->V[x] = chip->V[x] | chip->V[y];
                    chip->PC += 2;
                    return 0;
                
                case 0x02:
                    chip->V[x] = chip->V[x] & chip->V[y];
                    chip->PC += 2;
                    return 0;
                
                case 0x03:
                    chip->V[x] = chip->V[x] ^ chip->V[y];
                    chip->PC += 2;
                    return 0;

                //Vf 1 wenn ein overflow passiert sonst 0
                case 0x04:
                    if(chip->V[x] + chip->V[y] > 255)
                        chip->V[15] = 1;
                    else
                        chip->V[15] = 0;

                    chip->V[x] += chip->V[y];

                    chip->PC += 2;
                    return 0;

                //underflow
                case 0x05:
                    if(chip->V[x] >= chip->V[y])
                        chip->V[15] = 1;
                    else
                        chip->V[15] = 0;

                    chip->V[x] -= chip->V[y];
                    chip->PC += 2;
                    return 0;

                //Shift right und Vf wird auf den LSB vor dem shiften gesetzt
                case 0x06:
                    chip->V[15] = chip->V[x] & 1;
                    chip->V[x] >>= 1;

                    chip->PC += 2;
                    return 0;

                //underflow beachten
                case 0x07:
                    if(chip->V[y] >= chip->V[x])
                        chip->V[15] = 1;
                    else
                        chip->V[15] = 0;

                    chip->V[x] = chip->V[y] - chip->V[x];

                    chip->PC += 2;
                    return 0;

                //Left shift Vf wird auf MSB gesetzt
                case 0x0E:
                    chip->V[15] = (chip->V[x] >> 7) & 1;    
                    chip->V[x] <<= 1;

                    chip->PC += 2;
                    return 0;

                default:
                    break;
                }

        //Skip nächsten opcode wenn Vx != Vy
        case 0x09:
            if(chip->V[x] != chip->V[y])
                chip->PC += 2;
            chip->PC += 2;
            return 0;

        //Setzt I zu NNN
        case 0x0A: 
            chip->I = NNN;
            chip->PC += 2;
            return 0;

        //Springt zu PC V0 + NNN
        case 0x0B:
            chip->PC = chip->V[0] + NNN;
            return 0;

        //Random befehl
        case 0x0C:
            int random = rand() % 256;
            chip->V[x] = random & NN;
            chip->PC += 2;
            return 0;

        //Display drawing 
        case 0x0D:
            chip->V[0xF] = 0;
            int px = chip->V[x];     // Wert von VX
            int py = chip->V[y];     // Wert von VY
            draw(px,py,d, chip);
            chip->PC += 2;
            return 0;            

        //Tasten werden hier abgeprüft also key sonst überspringt er nächsten opcode
        case 0x0E:

            if(y == 9 && d == 0xE)
                if(key == chip->V[x])
                    chip->PC += 2;

            if(y == 0xA && d == 1)
                if(key != chip->V[x])
                    chip->PC += 2;

            chip->PC += 2;
            return 0;

        //weitere Opcodes
        case 0x0F:
            switch (NN)
            {
                //Setzt Vx = timer
                case 0x07:
                    chip->V[x] = chip->timer;
                    chip->PC += 2;
                    return 0;

                //muss auf key warten
                case 0x0A:
                    chip->waitingforkey = x;
                    return 0;

                //setzt timer = Vx
                case 0x15:
                    chip->timer = chip->V[x];
                    chip->PC += 2;
                    return 0;  

                //soundtimer sollte eigentlich hier implementiert werden 
                case 0x18:
                    chip->PC += 2;
                    return 0;  

                //I += Vx
                case 0x1E:
                    chip->I += chip->V[x];
                    chip->PC += 2;
                    return 0;  
                    
                // setzt I zu einer location eines Sprites mit I + Vx, Vx nur unterer nibble also untere 4 bits
                case 0x29:
                    chip->I = 0x050 + (chip->V[x] & 0x0F) * 5;
                    chip->PC += 2;
                    return 0;   

                //wiki chip8 opcode
                case 0x33:
                    chip->memory[chip->I] = chip->V[x] / 100;
                    chip->memory[chip->I + 1] = (chip->V[x] / 10) % 10;
                    chip->memory[chip->I + 2] = chip->V[x] % 10;
                    chip->PC += 2;
                    return 0;   

                
                // schreibt in memory alle V0 bis Vx ab I
                case 0x55:
                    for (uint16_t i = 0; i <= x; i++)
                    {
                        chip->memory[chip->I + i] = chip->V[i];
                    }
                    chip->PC += 2;
                    return 0;   
                
                //Lest memory aus 
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
    //hier wird einfach alles auf 0 gesetzt
    for (int i = 0; i < 64 * 32; i++)
    {
        chip->display[i] = 0;
    }
}


void draw(int x, int y, int height, Chip8 *chip)
{
    //Zeichnet das sprite
    //gibt wahrscheinlich bessere algos?
    for(int row = 0; row < height; row++)
    {
        uint8_t sprite = chip->memory[chip->I + row];

        for (int col = 0; col < 8; col++)//immer 8 pixel hoch
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


//nur für debugging 
void printBits(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");  // Leerzeichen alle 4 Bits
    }
}

//ChadGPt ... 
//IM prinzip ist das ein array das durch hex zahlen bestimmte sprites als zahlen zeichnet
/*
0xF0 = 11110000
0x90 = 10010000
0x90 = 10010000     wenn man nur die 1 beachtet kann man daraus 0 sprite sehen
0x90 = 10010000
0xF0 = 11110000
*/
void initFontset(Chip8 *chip)
{
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0  das zeigt das sprite oben
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