#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL2/SDL.h>

#define WINDOW_TITLE "Noahs Emulator"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FONT_SIZE 80

typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
}Window;


void cleanup(Window *window);
void initSDL(Window *window);


#endif