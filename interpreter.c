#include <stdio.h>
#include "SDLWindow.h"
#include "dragdrop.h"
#include "test.h"

int main()
{    

    Window window;

    initSDL(&window);

    char *filename = openWindowDragandDrop(&window);

    printf("%s\n", filename);    


    testWindow(&window);


    return 0;
}