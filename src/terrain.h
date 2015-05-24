#pragma once

#include <bgfx.h>
#include <vector>
#include <noise/noise.h>

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
        float nx;
        float ny;
        float nz;
    };

    bgfx::VertexDecl _vertexDecl;
    std::vector<PosColorVertex> _vertices;
    std::vector<uint16_t> _indices;

    noise::module::Perlin _noise;
    noise::module::RidgedMulti _mountain;
    noise::module::Billow _baseFlat;
    noise::module::ScaleBias _flat;
    noise::module::Select _select;
    noise::module::Turbulence _turb;

    int32_t _seed;
    float _chunkSize;
    float _zScale;
    float _xyScale;
    float _subDivisions;
    //const int _maxRadius;

    float getZ(float x, float y);

public:

    Terrain();
    ~Terrain();
    bool loadMat();
    void generate();

    const bgfx::ProgramHandle& getProgram() const;
    const bgfx::VertexBufferHandle& getVertBuff() const;
    const bgfx::IndexBufferHandle& getIndBuff() const;
};
