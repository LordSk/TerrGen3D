#include <iostream>
#include <bx/fpumath.h>
#include "utils.h"
#include "terrain.h"
#include <cstdlib>
#include <ctime>

float Terrain::getZ(float x, float y)
{
    return _turb.GetValue(x / _xyScale, y / _xyScale, 0.1) * _zScale;
}

Terrain::Terrain():
    _program(BGFX_INVALID_HANDLE),
    _vertBuff(BGFX_INVALID_HANDLE),
    _indBuff(BGFX_INVALID_HANDLE)
{
    _vertexDecl
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,   3, bgfx::AttribType::Float)
        .end();

    _seed = 123456789;

    _noise.SetSeed(_seed);
    _baseFlat.SetSeed(_seed);
    _mountain.SetSeed(_seed);

    _baseFlat.SetFrequency(2.0);
    _flat.SetSourceModule(0, _baseFlat);
    _flat.SetScale(0.125);
    _flat.SetBias(-0.75);

    _noise.SetFrequency(0.5);
    _noise.SetPersistence(0.25);

    _select.SetSourceModule(0, _flat);
    _select.SetSourceModule(1, _mountain);
    _select.SetControlModule(_noise);
    _select.SetBounds(0, 1000);
    _select.SetEdgeFalloff(0.125);

    _turb.SetSourceModule(0, _select);
    _turb.SetFrequency(4.0);
    _turb.SetPower(0.125);

    _chunkSize = 512.f;
    _subDivisions = 128;
    _zScale = 10.f;
    _xyScale = 100.f;

    srand(time(nullptr));
}

Terrain::~Terrain()
{
    if(bgfx::isValid(_program)) bgfx::destroyProgram(_program);
    if(bgfx::isValid(_vertBuff)) bgfx::destroyVertexBuffer(_vertBuff);
    if(bgfx::isValid(_indBuff)) bgfx::destroyIndexBuffer(_indBuff);
}

bool Terrain::loadMat()
{
    _program = utils::loadProgram("vs_terrain", "fs_terrain");
    return bgfx::isValid(_program);
}

void Terrain::generate()
{
    // generate vertices
    float squareSize = _chunkSize / (float)_subDivisions;
    int width = _subDivisions;

    _vertices.clear();
    _vertices.reserve(width*width);
    _indices.clear();
    _indices.reserve(width*width*6);

    // vertices
    for(int xx = 0; xx < width; xx++) {
        for(int yy = 0; yy < width; yy++) {
            float x = (float)xx * squareSize;
            float y = (float)yy * squareSize;
            float z = getZ(x , y);

            _vertices.push_back({ x, y, z, 0, 0, 0 });
        }
    }

    // indices
    for(int x = 0; x < width-1; x++) {
        for(int y = 0; y < width-1; y++) {
            int bl = x + width*y;
            int br = bl + 1;
            int tl = x + width*(y+1);
            int tr = tl + 1;
            _indices.push_back(bl);_indices.push_back(tl);_indices.push_back(tr);
            _indices.push_back(bl);_indices.push_back(tr);_indices.push_back(br);

            // normals
            auto& vertBl = _vertices[bl];
            const auto& vertTl = _vertices[tl];
            const auto& vertBr = _vertices[br];
            float norm[3];
            float v1[3] = { vertTl.x-vertBl.x, vertTl.y-vertBl.y, vertTl.z-vertBl.z};
            float v2[3] = { vertBr.x-vertBl.x, vertBr.y-vertBl.y, vertBr.z-vertBl.z};
            bx::vec3Cross(norm, v1, v2);

            vertBl.nx = norm[0];
            vertBl.ny = norm[1];
            vertBl.nz = norm[2];
        }
    }

    // assign buffers
    _vertBuff = bgfx::createVertexBuffer(
        bgfx::makeRef(_vertices.data(), _vertices.size()*sizeof(PosColorVertex))
        , _vertexDecl);

    _indBuff = bgfx::createIndexBuffer(
        bgfx::makeRef(_indices.data(), _indices.size()*sizeof(uint16_t))
    );

    std::cout << "vertbuff valid: " << bgfx::isValid(_vertBuff) << std::endl;
    std::cout << "indbuff valid: " << bgfx::isValid(_indBuff) << std::endl;
}

const bgfx::ProgramHandle &Terrain::getProgram() const
{
    return _program;
}

const bgfx::VertexBufferHandle &Terrain::getVertBuff() const
{
    return _vertBuff;
}

const bgfx::IndexBufferHandle &Terrain::getIndBuff() const
{
    return _indBuff;
}

