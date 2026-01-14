extern crate sdl2;

use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use std::time::Duration;

pub fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem.window("rust-sdl2 demo", 800, 600)
        .position_centered()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut i: u8 = 0;

    'running: loop {
        canvas.set_draw_color(Color::RGB(i, 64, 255 - i));
        canvas.clear();
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} => {break 'running}  //Kann in switch cases verwendet werden um keine code dupplication zu verursachen
                Event::KeyDown {keycode: Some(key), ..} => {
                    match key {
                        
                        Keycode::D => {i = (i + 10) % 245; println!("{}", i);}
                        Keycode::ESCAPE => {break 'running}

                        _ => {}
                    }
                }
                Event::DropFile { timestamp: _, window_id: _, filename } => {
                    println!("{filename}");
                }

                _ => {}
            }
        }
        // The rest of the game loop goes here...

        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }
}