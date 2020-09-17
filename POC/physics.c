#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <chipmunk/chipmunk.h>

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

static inline float real_to_size(cpFloat v) {
    return v*50;
}

static inline cpVect real_to_pix(cpVect real) {
    return cpv(300+50*real.x, 500-50*real.y);
}

static inline float real_to_sprite_rot(cpFloat rot) {
    return -rot*180/3.14;
}

int main() 
{ 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("Error initializing SDL: %s\n", SDL_GetError()); 
    } 
    SDL_Window* win = SDL_CreateWindow("GAME", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        1000, 1000, 0);

    // Create empty world with gravity
    cpVect gravity = cpv(0, -10);
    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    // Create a ground
    cpShape *ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 0), cpv(20, -5), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    // Commun to physic objects
    cpFloat radius = 1;
    cpFloat mass = 1;
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero); // inertia

    // TODO: create a box
    cpBody *boxBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(boxBody, cpv(0, 25));
    cpShape *boxShape = cpSpaceAddShape(space, cpBoxShapeNew(boxBody, 5.f, 5.f));

    // create a ball
    cpBody *ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ballBody, cpv(0, 15));
    cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero)); // collisin shape attached to the body
    cpShapeSetFriction(ballShape, 0.7);
    cpFloat timeStep = 1.0/60.0; // time step for the simulation

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
    dest.w *= 1; 
    dest.h *= 1; 
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

        SDL_RenderClear(rend);

        // Render Texture
        SDL_SetRenderDrawColor( rend, 0x00, 0x00, 0x00, 0xFF );   
        cpVect pos = real_to_pix(cpBodyGetPos(ballBody));
        dest.x = pos.x; dest.y = pos.y;
        cpSpaceStep(space, timeStep);
        cpFloat rot = cpBodyGetAngle(ballBody);
        SDL_RenderCopyEx(rend, tex, NULL, &dest, real_to_sprite_rot(rot), NULL, SDL_FLIP_NONE);
        DrawCircle(rend, dest.x, dest.y, real_to_size(radius));

        // Clip rendering (sprite sheet)
        SDL_Rect srcRect = { 0, 0, 50, 50 };
        SDL_Rect destRect = { 100, 100, 100, 100 };
        SDL_RenderCopy(rend, tex, &srcRect, &destRect);

        // Fill rect
        cpVect posRec = real_to_pix(cpBodyGetPos(boxBody));
        SDL_Rect fillRect = { posRec.x-real_to_size(2.5), posRec.y-real_to_size(2.5), real_to_size(5.0), real_to_size(5.0)};
        SDL_SetRenderDrawColor( rend, 0xFF, 0x00, 0x00, 0xFF );        
        SDL_RenderFillRect( rend, &fillRect );

        // Wired rect
        SDL_Rect outlineRect = { 300, 300, 200, 10};
        SDL_SetRenderDrawColor( rend, 0x00, 0xFF, 0x00, 0xFF );        
        SDL_RenderDrawRect( rend, &outlineRect );

        // Line
        cpVect groundPos1 = real_to_pix(cpv(-20, 0));
        cpVect groundPos2 = real_to_pix(cpv(20, -5));
        SDL_SetRenderDrawColor( rend, 0x00, 0x00, 0xFF, 0xFF );        
        SDL_RenderDrawLine( rend, groundPos1.x, groundPos1.y, groundPos2.x, groundPos2.y );

        // Point
        SDL_RenderDrawPoint( rend, 550, 550 );

        SDL_SetRenderDrawColor( rend, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }

    // clean up physics objects
    cpShapeFree(ballShape);
    cpBodyFree(ballBody);
    cpShapeFree(boxShape);
    cpBodyFree(boxBody);
    cpShapeFree(ground);
    cpSpaceFree(space);

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    return 0; 
} 
