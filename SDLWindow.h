#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL2/SDL.h>

#define WINDOW_TITLE "Noahs Emulator"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512
#define FONT_SIZE 80
#define DISPLAY_SIZE 10

typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
}Window;


void cleanup(Window *window);
void initSDL(Window *window);


#endif