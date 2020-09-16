# ASM Game Engine

## Disclaimer
I make this project just for fun, if you are looking for a true game engine for your own project please have a look at Godot Engine.

## API/dependency
For this project I will use SDL2 as only high level API (Because I need something a not object oriented library, C libraries are good in this regard)

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
