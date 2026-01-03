#include "test.h"

void testWindow(Window *window, char *filename)
{

    Chip8 chip = initChip8(filename);


    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                cleanup(window);
                break;

            default:
                break;
            }

        }        
        
    execute(&chip, 1);



        SDL_SetRenderDrawColor(window->renderer,255,0,255,255);
        SDL_RenderClear(window->renderer);
        SDL_RenderPresent(window->renderer);
 


        SDL_Delay(16);
    }
    
}