use std::fs;


pub struct Chip8{
    pub memory: [u8; 4096],
    pub pc: u16,
    pub v: [u8; 16],
    pub i: u16,
    pub stack: [u16; 16],
    pub stack_pointer: u16,
    pub timer: u8,
    pub display: [u8; 64 * 32],
    pub waiting_key: bool,
    pub target_key: u8 
}

impl Chip8{
    pub fn new(filename: String) -> Self{

        let mut chip: Chip8 = Self{
            memory: [0; 4096],
            pc: 0x200,
            v: [0;16],
            i: 0,
            stack:  [0;16],
            stack_pointer: 0,
            timer: 0,
            display: [0; 64* 32],
            waiting_key: false,
            target_key: 0
        };

        let bytes = fs::read(filename).expect("Fehler beim auslesen");

        let start = chip.pc as usize;
        let end = start + bytes.len();

        println!("Filesize: {}", bytes.len());

        if end <= chip.memory.len(){
            chip.memory[start..end].copy_from_slice(&bytes);
        }else{
            panic!("Zu große Game datei eingelesen");
        }
 
        chip.init_fontset();

        chip
    }
    
    pub fn execute(&mut self, keys: &[u8])
    {
        //println!("KEYS PRESSED {:?}", keys.to_ascii_lowercase());
        if self.timer > 0
        {
            self.timer -= 1;
        }

        if self.waiting_key{
            if keys.len() > 0{
                self.v[self.target_key as usize] = *keys.first().expect("Fehler waiting keys");
                self.waiting_key = false;
            }else{
                return;
            }
        }



        let nibbles = (self.memory[self.pc as usize] >> 4,self.memory[self.pc as usize] & 0x0F, self.memory[self.pc as usize + 1] >> 4, self.memory[self.pc as usize + 1] & 0x0F); 
        println!("{} {} {} {}",nibbles.0, nibbles.1,nibbles.2,nibbles.3);
        let x  = nibbles.1 as usize;
        let y = nibbles.2 as usize;
        let nnn: u16 = ((nibbles.1 as u16) << 8) | self.memory[self.pc as usize + 1] as u16;
        let nn = self.memory[self.pc as usize + 1];

        match nibbles.0 {
            0x0 => {
                if nibbles.0 == 0 && x == 0 && y == 0xE{
                    if nibbles.3 == 0 {
                        self.display_clear();
                    }

                    if nibbles.3 == 0xE {
                        //println!("{}", self.stack_pointer);
                        self.stack_pointer -= 1;
                        self.pc = self.stack[self.stack_pointer as usize] + 2;
                        return;
                    }
                }
            }
            
            0x1 => {self.pc = nnn; return;}

            0x2 =>{
                //println!("{}", self.stack_pointer);
                self.stack[self.stack_pointer as usize] = self.pc; //Fehler bei flightrunner
                self.stack_pointer +=1;
                self.pc = nnn;
                return;
            }

            0x3 =>{
                if self.v[x] == nn {
                    self.pc += 2;
                }
            }

            0x4 =>{
                if self.v[x] != nn {
                    self.pc += 2;
                }
            }

            0x5 =>{
                if self.v[x] == self.v[y] {
                    self.pc += 2;
                }
            }

            0x6 =>{self.v[x] = nn}

            0x7 =>{self.v[x] = self.v[x].wrapping_add(nn)}

            0x8 =>{
                match nibbles.3 {
                    0x0 =>{self.v[x] = self.v[y]}

                    0x1 =>{self.v[x] = self.v[x] | self.v[y]}

                    0x2 =>{self.v[x] = self.v[x] & self.v[y]}

                    0x3 =>{self.v[x] = self.v[x] ^ self.v[y]}

                    0x4 =>{
                        let (result, overflow) = self.v[x].overflowing_add(self.v[y]);
                        self.v[15] = overflow as u8;
                        self.v[x] = result;
                    }

                    0x5 =>{
                        let (result, overflow) = self.v[x].overflowing_sub(self.v[y]);
                        self.v[15] = (!overflow) as u8;
                        self.v[x] = result;                   
                    }

                    0x6 =>{
                        self.v[15] = self.v[x] & 1;
                        self.v[x] >>= 1;
                    }

                    0x7 =>{
                        let (result, overflow) = self.v[y].overflowing_sub(self.v[x]);
                        self.v[15] = (!overflow) as u8;
                        self.v[x] = result;
                    }

                    0xE =>{
                        self.v[15] = (self.v[x] >> 7) & 1;
                        self.v[x] <<= 1; 
                    }

                    _ => {}
                }
            }
            
            0x9 =>{
                if self.v[x] != self.v[y] {
                    self.pc += 2;
                }
            }

            0xA =>{self.i = nnn}

            0xB =>{
                self.pc = self.v[0] as u16 + nnn;
                return;
            }

            0xC =>{
                let n = rand::random::<u8>();
                //println!("Random zahl: {}", n);
                self.v[x] = n & nn;
                //println!("v[x] = {:02X}", self.v[x]);

            }

            0xD =>{
                self.v[15] = 0;
                self.draw(self.v[x], self.v[y], nibbles.3);

            }

            0xE => {
                if y == 9 && nibbles.3 == 0xE {
                    if keys.contains(&self.v[x]){
                        self.pc += 2;
                    }
                }

                if y == 0xA && nibbles.3 == 1 {
                    if !keys.contains(&self.v[x]){
                        self.pc += 2;
                    }
                }
            }

            0xF =>{
                match nn {
                    0x07 =>{self.v[x] = self.timer;}

                    0x0A =>{
                        self.waiting_key = true;
                        return;
                    }//waiting forkey

                    0x15 =>{self.timer = self.v[x];}

                    0x18 =>{}// sound

                    0x1E =>{self.i += self.v[x] as u16}

                    0x29 =>{self.i = 0x050 + (self.v[x] as u16 & 0xF) * 5}

                    0x33 =>{
                        self.memory[self.i as usize] = self.v[x] / 100;
                        self.memory[self.i as usize + 1] = (self.v[x] / 10) % 10;
                        self.memory[self.i as usize + 2] = self.v[x] % 10;
                    }

                    0x55 =>{
                        for i in 0..=x {
                            self.memory[self.i as usize + i] = self.v[i];
                        }
                    }

                    0x65 =>{
                        for i in 0..=x {
                            self.v[i] = self.memory[self.i as usize + i];
                        }
                    }


                    _ => {}
                }
            }




            _ => {println!("Unhandled Opcode");}
        }

        self.pc += 2;
    }

    fn display_clear(&mut self){
        for pixel in self.display.iter_mut(){
            *pixel = 0;
        }   
    }

    fn draw(&mut self, x:u8, y: u8, height: u8){
        for row in 0..height as u32{
            let sprite = self.memory[self.i as usize + row as usize];
            for col in 0..8{
                if ((sprite >> (7 - col)) & 1) != 0 {
                    let px:u32 = (x as u32 + col) % 64;
                    let py:u32 = (y as u32 + row) % 32;
                    let index = (py * 64 + px) as usize;

                    if self.display[index] == 1 {self.v[15] = 1}

                    self.display[index] ^= 1;
                }
            }
        }

    }

    fn init_fontset(&mut self) {
        let fontset: [u8; 80] = [
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        ];

        // Kopiere die Fontset Bytes in den Speicher bei 0x50
        self.memory[0x50..0x50 + 80].copy_from_slice(&fontset);
    }

}


