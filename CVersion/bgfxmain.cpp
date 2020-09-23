#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>

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

        bgfx::touch(0);
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Counter:%d", counter++);
        bgfx::frame();

        bgfx::renderFrame();
    }

    bgfx::shutdown();

    //while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {
    //};

    SDL_DestroyWindow(window);
    SDL_Quit();
}