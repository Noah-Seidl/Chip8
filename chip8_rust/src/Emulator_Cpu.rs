use std::fs;
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

        let mut chip = Self{
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
    
    pub fn execute(&mut self)
    {
        if(self.timer > 0)
        {
            self.timer -= 1;
        }

        let nibbles = (self.memory[self.PC as usize] >> 4,self.memory[self.PC as usize] & 0x0F, self.memory[self.PC as usize + 1] >> 4, self.memory[self.PC as usize + 1] & 0x0F); 
        let x = nibbles.1;
        let y = nibbles.2;
        let NNN = (nibbles.1 << 8) | self.memory[self.PC as usize + 1];
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
            
            0x1 => {
                self.PC = NNN as u16;
            }



            

            0x1 =>{
                self.PC = ;
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


}


