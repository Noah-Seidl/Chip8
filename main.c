#include <stdio.h>
#include "SDLWindow.h"
#include "dragdrop.h"
#include "test.h"
#include "emulator.h"

int main()
{    
    Window window;

    initSDL(&window);

    char *filename = openWindowDragandDrop(&window);
    if(filename == NULL)
        return 1;

    printf("%s\n", filename);    



    testWindow(&window, filename);


    return 0;
}
