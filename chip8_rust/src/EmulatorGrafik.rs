const DISPLAY_MULTIPLIER:u32 = 10;

use sdl2;

use sdl2::pixels::Color;
use sdl2::rect::Rect;
use sdl2::sys::{DisableAccess, SDL_Rect, XFillRectangle};
use sdl2::video::Window;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::render::Canvas;
use std::time::Duration;

use crate::Emulator_Cpu::Chip8;

pub struct Grafik;


impl Grafik{
    pub fn emulator_mainloop(canvas: &mut Canvas<Window>, sdl_context: sdl2::Sdl, chip: &mut crate::Emulator_Cpu::Chip8){

    canvas.window_mut().set_title("Emulator").expect("Fehler oder so");

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();
    let mut event_pump = sdl_context.event_pump().unwrap();

    loop {
        canvas.set_draw_color(Color::RGB(255, 64, 255));
        canvas.clear();
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} => {return ;}  //Kann in switch cases verwendet werden um keine code dupplication zu verursachen
                Event::KeyDown {keycode: Some(key), ..} => {
                    match key {
                        
                        Keycode::D => {}
                        Keycode::ESCAPE => {return;}

                        _ => {}
                    }
                }
                _ => {}
            }
        }
        // The rest of the game loop goes here...

        Chip8::execute(chip, &[1,2,3]);

        renderDisplay(canvas, chip);



        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
        }
    }
}


fn renderDisplay(canvas: &mut Canvas<Window>, chip: &Chip8)
{
    canvas.set_draw_color(Color::RGB(255, 255, 255));
    canvas.clear();

    let mut x: u32;
    let mut y: u32;
    for i in (0..64 * 32){
        x = i % 64;
        y = i / 64;

        if(chip.display[i as usize] == 1){
            canvas.set_draw_color(Color::RGB(255, 255, 255));
        }else{
            canvas.set_draw_color(Color::RGB(0, 0, 0));
        }

        let rect: Rect = Rect::new((x * DISPLAY_MULTIPLIER)as i32, (y * DISPLAY_MULTIPLIER) as i32, DISPLAY_MULTIPLIER, DISPLAY_MULTIPLIER);
        
    canvas.fill_rect(rect).expect("Fehler bei Rectangle zeichnen");
    }

}