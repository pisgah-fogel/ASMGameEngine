# ASMGameEngine
An Assembler Game Engine.

## Disclaimer
I make this project just for fun, if you are looking for a true game engine for your own project please have a look at Godot Engine.

## API/dependency
For this project I will use SDL2 (Because I need something a not object oriented library, C libraries are good in this regard).
I am also going to use Chipmunk2D for 2D physics (I would like to use Box2D but a C library is easier and 'cleaner' for an assembler usage).

## Progression
Not ready at all, barely started

## IDE
I may provide an IDE made with Qt/C++ later.
Or a VS Code plugin ? (If I can manage to integrate TileMap editor, animations editor...)

## Objectives
- targeted platform = x86_64 (With SSE2-4, AVX, AVX2, AVX-512)
- C/SDL2 experiment
- Assembler/SDL2 implementation
- Assembler/SDL2 templates
- Assembler/SDL2 demo
- IDE to generate Assembler/SDL2 code from nodes
- Interpreted high level assembler (easier to debug) that we can run from the IDE
- Translate high level assembler to standard (GAS) assembler
- new targeted platform = arm v7 (rapberry pi 4 ?)

## Learning resources:
- SDL tutorial: http://www.lazyfoo.net/tutorials/SDL/index.php#Geometry%20Rendering
- Chipmunk2D github: https://github.com/slembcke/Chipmunk2D
- Chipmunk2D example: https://chipmunk-physics.net/release/ChipmunkLatest-Docs/

## Develop on Windows:
### MinGW32
- Get mingw-get-setup.exe there: https://osdn.net/projects/mingw/
- Install MinGW at C:\MinGW (Check C:\MinGW\lib)
- Add Mingw bin directory to your PATH environment variable

### SDL2 for Windows-MinGW30
- Download SDL2 there: https://www.libsdl.org/download-2.0.php
- Download SDL2_image there: https://www.libsdl.org/projects/SDL_image/
- Copy SDL2 and SDL2_img in C:\SDL2 (Check C:\SDL2\i686-w64-mingw32\lib and C:\SDL2\i686-w64-mingw32\include)
- Copy all .DLL from C:\SDL2\i686-w64-mingw32\bin to the direcotry where the game's executable.

### Chipmunk from source
- Download CMake and install it: https://cmake.org/download/
- Dowmload Chipmunk there: https://chipmunk-physics.net/release/ChipmunkLatest.tgz
- Copy Chipmunk C:\Chipmunk (Check ????)
- cmake -G "MinGW Makefiles" .
- If it complains about sh.exe being in your path, just rename it sh.exe.old
- make
- if you get "error: unknown type name 'ptrdiff_t'", add #include <stddef.h> to demo\sokol\sokol_app.h
-  Copy C:\Chipmunk\src\libchipmunk.dll next to the executable

### Visual Studio Code setup
- Append to settings.json:
  ```
  "C_Cpp.default.includePath": ["C:\\SDL2\\i686-w64-mingw32\\include", "C:\\Chipmunk\\include"]
  ```