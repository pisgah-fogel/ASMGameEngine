#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <chipmunk/chipmunk.h>

#include "Sprite.h"

#define ERROR_HANDLING

#define WIN_X 800
#define WIN_Y 600
#define WIN_NAME "GAME"

SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
Uint32 win_format = -1; // Window's optimal pixel format
void initSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("Error initializing SDL: %s\n", SDL_GetError()); 
    } 
    win = SDL_CreateWindow(WIN_NAME, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIN_X, WIN_Y, 0);
    
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Get window's optimal pixel format
    SDL_Surface* windowSurface = SDL_GetWindowSurface(win);
    win_format = windowSurface->format->format;
}
void freeSDL()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
}

int main(int argc, char **argv)
{
    initSDL();

    struct Sprite* mSprite = initSprite(rend);

    int close = 0;
    while (!close) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
                case SDL_KEYDOWN: 
                    switch (event.key.keysym.scancode) { 
                        case SDL_SCANCODE_A: 
                        case SDL_SCANCODE_LEFT: 
                            //dest.x -= speed; 
                            break; 
                        case SDL_SCANCODE_D: 
                        case SDL_SCANCODE_RIGHT: 
                            //dest.x += speed; 
                            break; 
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor( rend, 0x00, 0x00, 0x00, 0xFF );  
        SDL_RenderClear(rend);

        renderSprite(rend, mSprite);

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }

    freeSprite(mSprite);
    freeSDL();
    return 0;
}