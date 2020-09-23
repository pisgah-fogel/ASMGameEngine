#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "bx/math.h"
#include <bx/bx.h>

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

void cube_vertexDeclaration(bgfx::VertexBufferHandle *pvbh, bgfx::IndexBufferHandle *pibh)
{
    bgfx::VertexLayout pcvDecl;
    //bgfx::VertexDecl pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    *pvbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    *pibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
}

bgfx::ShaderHandle loadShader(const char *FILENAME)
{
    const char* shaderDir = "???";
    std::string filePath = "";

    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  shaderDir = "shaders/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderDir = "shaders/dx11/";  break;
        case bgfx::RendererType::Gnm:        shaderDir = "shaders/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderDir = "shaders/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderDir = "shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderDir = "shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderDir = "shaders/spirv/"; break;
    }
    filePath.append(shaderDir);
    filePath.append(FILENAME);

    FILE *file = fopen(filePath.c_str(), "rb");
    if (file == NULL) {
        std::cout<<"Error: Cannot load shader "<<filePath<<std::endl;
        exit(1); // TODO: Free resources or do a proper error handling
    }

    // Get the file's size to alloc the memory
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);
    // TODO: Do I need to clean the memory ?

    return bgfx::createShader(mem);
}