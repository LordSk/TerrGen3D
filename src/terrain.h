#pragma once

#include <bgfx.h>
#include <vector>

class Terrain
{
    bgfx::ProgramHandle _program;
    bgfx::VertexBufferHandle _vertBuff;
    bgfx::IndexBufferHandle _indBuff;

    struct PosColorVertex
    {
        float x;
        float y;
        float z;
        uint32_t abgr;
    };

    bgfx::VertexDecl _vertexDecl;
    std::vector<PosColorVertex> _vertices;
    std::vector<uint16_t> _indices;


public:

    Terrain();
    ~Terrain();
    bool loadMat();
    void generate();

    const bgfx::ProgramHandle& getProgram() const;
    const bgfx::VertexBufferHandle& getVertBuff() const;
    const bgfx::IndexBufferHandle& getIndBuff() const;
};
