#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <chipmunk/chipmunk.h>

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

struct Sprite
{
    SDL_Texture* texture;
    SDL_Rect rect; // x/y/w/h destination position and scale
    SDL_Rect clip; // Texture clipping
    SDL_Point center; // Rotation center
};

struct Sprite* initSprite()
{
    SDL_Surface* surface = IMG_Load("test1.png");
#ifdef ERROR_HANDLING
    if (surface == NULL) {
        fprintf(stderr, "initSprite: IMG_Load failed: %s\n", SDL_GetError());
        return NULL;
    }
#endif

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

    SDL_FreeSurface(surface);
#ifdef ERROR_HANDLING
    if (tex == NULL) {
        fprintf(stderr, "initSprite: CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return NULL;
    }
#endif
    struct Sprite* output = malloc(sizeof(struct Sprite));
#ifdef ERROR_HANDLING
    if (output == NULL) {
        fprintf(stderr, "initSprite: Malloc failed\n");
        SDL_DestroyTexture(tex);
        return NULL;
    }
#endif

    SDL_QueryTexture(tex, NULL, NULL, &output->rect.w, &output->rect.h);
    output->texture = tex;
    output->rect = (struct SDL_Rect){200, 100, 100, 100};
    output->clip = (struct SDL_Rect){0, 0, 100, 100};
    output->center = (struct SDL_Point){50, 50};
    return output;
}

void renderSprite(SDL_Renderer* renderer, struct Sprite* ptr) {
#ifdef ERROR_HANDLING
    if (ptr == NULL || ptr->texture == NULL || renderer == NULL) {
        return;
    }
#endif
    SDL_RenderCopy(renderer, ptr->texture, &ptr->clip, &ptr->rect);
    //SDL_RenderCopyEx(rend, tex, NULL, &dest, real_to_sprite_rot(rot), &centerImg, SDL_FLIP_NONE);
}

void freeSprite(struct Sprite* ptr)
{
#ifdef ERROR_HANDLING
    if (ptr == NULL) {
        fprintf(stderr, "Sprite's pointer is not valid\n");
        return;
    }
    else if (ptr->texture == NULL) {
        fprintf(stderr, "Texture's pointer is not valid (but Sprite's pointer is)\n");
        free(ptr);
        ptr = NULL;
        return;
    }
#endif
    SDL_DestroyTexture(ptr->texture);
    free(ptr);
    ptr = NULL;
}

int main(int argc, char **argv)
{
    initSDL();

    struct Sprite* mSprite = initSprite();

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