
mod SDLUtil;
mod EmulatorGrafik;
mod Emulator_Cpu;

fn main() {
    println!("Hello, world!");
    let filename = SDLUtil::SDLUtil::Drag_Drop();

    let (_, mut canvas, sdl_context) = match filename {
        Some(data) => {
            println!("Gedroppte File{}", data.0);
            data
            }

        None => {
            panic!("Fehler bei drag and drop");
        }
    };
 



    EmulatorGrafik::Grafik::emulator_mainloop(&mut canvas, sdl_context);



}