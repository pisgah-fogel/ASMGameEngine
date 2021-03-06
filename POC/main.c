#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
  
int main(int argc, char** argv) 
{ 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("Error initializing SDL: %s\n", SDL_GetError()); 
    } 
    SDL_Window* win = SDL_CreateWindow("GAME", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        1000, 1000, 0);

    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
    SDL_Surface* surface = IMG_Load("hand_open.png");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest;

    // Texture color modulation
    SDL_SetTextureColorMod( tex, 0, 255, 0 );

    // Alpha blending
    SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( tex, 127 );

    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w *= 3; 
    dest.h *= 3; 
    dest.x = (1000 - dest.w) / 2;
    dest.y = (1000 - dest.h) / 2;

    int close = 0;
    int speed = 10;


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
                            dest.x -= speed; 
                            break; 
                        case SDL_SCANCODE_D: 
                        case SDL_SCANCODE_RIGHT: 
                            dest.x += speed; 
                            break; 
                    }
                    break;
            }
        }

        SDL_RenderClear(rend);

        // Texture
        SDL_RenderCopy(rend, tex, NULL, &dest);

        // Clip rendering (sprite sheet)
        SDL_Rect srcRect = { 0, 0, 50, 50 };
        SDL_Rect destRect = { 100, 100, 100, 100 };
        SDL_RenderCopy(rend, tex, &srcRect, &destRect);

        // Fill rect
        SDL_Rect fillRect = { 10, 10, 100, 50};
        SDL_SetRenderDrawColor( rend, 0xFF, 0x00, 0x00, 0xFF );        
        SDL_RenderFillRect( rend, &fillRect );

        // Wired rect
        SDL_Rect outlineRect = { 15, 15, 100, 50};
        SDL_SetRenderDrawColor( rend, 0x00, 0xFF, 0x00, 0xFF );        
        SDL_RenderDrawRect( rend, &outlineRect );

        // Line
        SDL_SetRenderDrawColor( rend, 0x00, 0x00, 0xFF, 0xFF );        
        SDL_RenderDrawLine( rend, 0, 0, 500, 500 );

        // Point
        SDL_RenderDrawPoint( rend, 550, 550 );

        SDL_SetRenderDrawColor( rend, 0xFF, 0x3F, 0xFF, 0xFF );
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    return 0; 
} 
