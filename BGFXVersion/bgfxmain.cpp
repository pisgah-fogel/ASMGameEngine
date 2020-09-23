#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include "bx/math.h"

#include "cube.h"

/*
 * The the native window (system dependant so...)
 */
inline bool sdlSetWindow(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi)) {
        return false;
    }

    bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    std::cout<<"This software is intented for Linux/BSD platform"<<std::endl;
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    pd.ndt = NULL;
    pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    pd.ndt = NULL;
    pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
    pd.ndt = wmi.info.vivante.display;
    pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    return true;
}

int main(void)
{
    // SDL_InitSubSystem(SDL_INIT_VIDEO); // TODO: Is it better ?
    SDL_Init(0);

    SDL_Window* window = SDL_CreateWindow("bgfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    sdlSetWindow(window);
    bgfx::renderFrame(); // prevent creation of a renderer thread

    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Automatically choose a renderer
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);
    //bgfx::reset(800, 600, BGFX_RESET_VSYNC); // TODO: Is it needed ?

    // Enable debug text.
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF /*purple*/, 1.f, 0);
    bgfx::setViewRect(0, 0, 0, uint16_t(800), uint16_t(600));
    // bgfx::frame(); // TODO: Is it needed/better ?

    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    cube_vertexDeclaration(&vbh, &ibh);

    bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    bool exit = false;
    SDL_Event event;
    int counter = 0;
    while (!exit) {

        while (SDL_PollEvent(&event)) {

            switch (event.type) {
            case SDL_QUIT:
                exit = true;
                break;

            case SDL_WINDOWEVENT: {
                const SDL_WindowEvent& wev = event.window;
                switch (wev.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    exit = true;
                    break;
                }
            } break;
            }
        }

        //
        const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, 800.f/600.f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        float mtx[16];
        bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
        bgfx::setTransform(mtx);        

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        //bgfx::touch(0); // draw an empty primitive

        bgfx::submit(0, program);

        // debug information
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Counter:%d", counter++);

        bgfx::frame(); // Single threaded: Do rendering, for multithread use bgfx::renderFrame();
    }

    bgfx::shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();
}