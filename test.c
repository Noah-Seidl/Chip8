#include "test.h"

void testWindow(Window *window)
{
    char *dropped_filedir;
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
            
            case SDL_DROPFILE:
                dropped_filedir = event.drop.file;
                SDL_Log("Dropped %s",dropped_filedir);
                
                SDL_free(dropped_filedir);

                break;

            default:
                break;
            }

        }
        
        
        SDL_SetRenderDrawColor(window->renderer,255,0,255,255);
        SDL_RenderClear(window->renderer);
        SDL_RenderPresent(window->renderer);
 


        SDL_Delay(16);
    }
    
}