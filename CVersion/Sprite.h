#pragma once

#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

struct Sprite
{
    SDL_Texture* texture;
    SDL_Rect rect; // x/y/w/h destination position and scale
    SDL_Rect clip; // Texture clipping
    SDL_Point center; // Rotation center
};

struct Sprite* initSprite(SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load("assets/spritesheet.png");
#ifdef ERROR_HANDLING
    if (surface == NULL) {
        fprintf(stderr, "initSprite: IMG_Load failed: %s\n", SDL_GetError());
        return NULL;
    }
#endif

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
#ifdef ERROR_HANDLING
    if (tex == NULL) {
        fprintf(stderr, "initSprite: CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return NULL;
    }
#endif
    struct Sprite* output = (struct Sprite*)malloc(sizeof(struct Sprite));
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