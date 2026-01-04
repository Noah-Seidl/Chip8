#include "test.h"

void renderDisplay(Window *window, Chip8 *chip);


void testWindow(Window *window, char *filename)
{

    Chip8 chip = initChip8(filename);

    int key = 0;

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

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_SPACE:
                        key = 1;
                        break;

                    case SDL_SCANCODE_A:
                        key = 5;
                        break;

                    case SDL_SCANCODE_Q:
                        key = 4;
                        break;
                    case SDL_SCANCODE_D:
                        key = 6;
                        break;
                    

                    default:
                        break;
                }
                
            default:
                break;
            }

        }        
        
        execute(&chip, key);
        key = -1;
        renderDisplay(window,&chip);

        SDL_RenderPresent(window->renderer);
 


        //SDL_Delay(1);
    }
    
}


void renderDisplay(Window *window, Chip8 *chip)
{
    int x = 0,y = 0;

    for (size_t i = 0; i < 64*32; i++)
    {
        x = i % 64;
        y = i / 64;
        SDL_Rect rect;
        rect.x = x * DISPLAY_SIZE;
        rect.y = y * DISPLAY_SIZE;
        rect.h = DISPLAY_SIZE;
        rect.w = DISPLAY_SIZE;

        if(chip->display[i])
            SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);

        SDL_RenderFillRect(window->renderer, &rect);
    }
}