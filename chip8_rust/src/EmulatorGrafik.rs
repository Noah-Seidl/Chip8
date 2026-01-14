use sdl2;

use sdl2::pixels::Color;
use sdl2::video::Window;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::render::Canvas;
use std::time::Duration;

pub struct Grafik;

impl Grafik{
    pub fn emulator_mainloop(canvas: &mut Canvas<Window>, sdl_context: sdl2::Sdl){

    canvas.window_mut().set_title("Emulator").expect("Fehler oder so");

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut i: u8 = 0;

    loop {
        canvas.set_draw_color(Color::RGB(255, 64, i));
        canvas.clear();
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} => {return ;}  //Kann in switch cases verwendet werden um keine code dupplication zu verursachen
                Event::KeyDown {keycode: Some(key), ..} => {
                    match key {
                        
                        Keycode::D => {i = (i + 10) % 245; println!("{}", i);}
                        Keycode::ESCAPE => {return;}

                        _ => {}
                    }
                }
                _ => {}
            }
        }
        // The rest of the game loop goes here...

        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
        }
    }
}