use std::fs;
use rand::Rng;

pub struct CPU;



pub struct Chip8{
    pub memory: [u8; 4096],
    pub PC: u16,
    pub V: [u8; 16],
    pub I: u16,
    pub Stack: [u16; 16],
    pub stack_pointer: u8,
    pub timer: u8,
    pub display: [u8; 64 * 32]
}

impl Chip8{
    pub fn new(filename: &String) -> Self{

        let mut chip: Chip8 = Self{
            memory: [0; 4096],
            PC: 0x200,
            V: [0;16],
            I: 0,
            Stack:  [0;16],
            stack_pointer: 0,
            timer: 0,
            display: [0; 64* 32]
        };

        let bytes = fs::read(filename).expect("Fehler beim auslesen");
        
        for (i, &byte) in bytes.iter().enumerate(){
            chip.memory[chip.PC as usize + i] = byte;
        }
    
        chip
    }
    
    pub fn execute(&mut self, keys: &[u8])
    {
        if(self.timer > 0)
        {
            self.timer -= 1;
        }

        let nibbles = (self.memory[self.PC as usize] >> 4,self.memory[self.PC as usize] & 0x0F, self.memory[self.PC as usize + 1] >> 4, self.memory[self.PC as usize + 1] & 0x0F); 
        let x  = nibbles.1 as usize;
        let y = nibbles.2 as usize;
        let NNN: u16 = ((nibbles.1 as u16) << 8) | self.memory[self.PC as usize + 1] as u16;
        let NN = self.memory[self.PC as usize + 1];

        match nibbles.0 {
            0x0 => {
                if(nibbles.0 == 0 && x == 0 && y == 0xE){
                    if(nibbles.3== 0){
                        self.display_clear();
                    }

                    if(nibbles.3 == 0xE){
                        self.stack_pointer -= 1;
                        self.PC = self.Stack[self.stack_pointer as usize];
                    }
                }
            }
            
            0x1 => {self.PC = NNN}

            0x2 =>{
               self.Stack[self.stack_pointer as usize] = self.PC;
               self.stack_pointer +=1;
            }

            0x3 =>{
                if(self.V[x] == NN){
                    self.PC += 2;
                }
            }

            0x4 =>{
                if(self.V[x] != NN){
                    self.PC += 2;
                }
            }

            0x5 =>{
                if(self.V[x] == self.V[y]){
                    self.PC += 2;
                }
            }

            0x6 =>{self.V[x] = NN}

            0x7 =>{self.V[x] += NN}

            0x8 =>{
                match nibbles.3 {
                    0x0 =>{self.V[x] = self.V[y]}

                    0x1 =>{self.V[x] = self.V[x] | self.V[y]}

                    0x2 =>{self.V[x] = self.V[x] & self.V[y]}

                    0x3 =>{self.V[x] = self.V[x] ^ self.V[y]}

                    0x4 =>{
                        let (result, overflow) = self.V[x].overflowing_add(self.V[y]);
                        self.V[15] = overflow as u8;
                        self.V[x] = result;
                    }

                    0x5 =>{
                        let (result, overflow) = self.V[x].overflowing_sub(self.V[y]);
                        self.V[15] = overflow as u8;
                        self.V[x] = result;                   
                    }

                    0x6 =>{
                        self.V[x] = self.V[x] & 1;
                        self.V[x] >>= 1;
                    }

                    0x7 =>{
                        let (result, overflow) = self.V[y].overflowing_sub(self.V[x]);
                        self.V[15] = overflow as u8;
                        self.V[x] = result;
                    }

                    0xE =>{
                        self.V[15] = (self.V[x] >> 7) & 1;
                        self.V[15] <<= 1; 
                    }

                    _ => {}
                }
            }
            
            0x9 =>{
                if(self.V[x] != self.V[y]){
                    self.PC += 2;
                }
            }

            0xA =>{self.I = NNN}

            0xB =>{
                self.PC = self.V[0] as u16 + NNN;
                return;
            }

            0xC =>{
                let mut rng = rand::rng();
                let n: u8 = rng.random();
                self.V[x] = n & NN;
            }

            0xD =>{
                self.V[15] = 0;
                self.draw(self.V[x], self.V[y], nibbles.3);

            }

            0xE => {
                if(y == 9 && nibbles.3 == 0xE){
                    for key in keys.iter(){
                        if(*key == self.V[x]){
                            self.PC += 2;
                        }
                    }
                }

                if(y == 0xA && nibbles.3 == 1){
                    for key in keys.iter(){
                        if(*key != self.V[x]){
                            self.PC += 2;
                        }
                    }
                }
            }

            0xF =>{
                
            }




            _ => {println!("Unhandled Opcode");}
        }

        self.PC += 2;
    }

    fn display_clear(&mut self){
        for pixel in self.display.iter_mut(){
            *pixel = 0;
        }   
    }

    fn draw(&mut self, x:u8, y: u8, height: u8){
        for row in (0..height){
            let mut sprite = self.memory[self.I as usize + row as usize];
            for col in (0..8){
                if(((sprite >> (7 - col)) & 1) != 0){
                    let px = (x + col) % 64;
                    let py = (y + row) % 32;
                    let index = (py * 64 + px) as usize;

                    if(self.display[index] == 1){self.V[15] = 1}

                    self.display[index] ^= 1;
                }
            }
        }

    }


}


