mod Emulator_Cpu;
mod SDLUtil;
mod EmulatorGrafik;

fn main() {
    println!("Hello, world!");
    let filename = SDLUtil::SDLUtil::Drag_Drop();

    let (filename, mut canvas, sdl_context) = match filename {
        Some(data) => {
            println!("Gedroppte File{}", data.0);
            data
            }

        None => {
            panic!("Fehler bei drag and drop");
        }
    };
 
    
    let mut chip = Emulator_Cpu::Chip8::new(filename);

    

    EmulatorGrafik::Grafik::emulator_mainloop(&mut canvas, sdl_context, &mut chip);



}