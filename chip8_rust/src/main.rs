mod emulator_cpu;
mod sdl_util;
mod emulator_grafik;

fn main() {
    println!("Hello, world!");
    let filename = sdl_util::SDLUtil::drag_drop();

    let (filename, mut canvas, sdl_context) = match filename {
        Some(data) => {
            println!("Gedroppte File{}", data.0);
            data
            }

        None => {
            panic!("Fehler bei drag and drop");
        }
    };
 
    
    let mut chip = emulator_cpu::Chip8::new(filename);

    

    emulator_grafik::Grafik::emulator_mainloop(&mut canvas, sdl_context, &mut chip);



}