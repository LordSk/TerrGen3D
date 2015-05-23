#include <iostream>
#include "utils.h"
#include "terrain.h"

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
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();

    _vertices = {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    _indices = {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

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
}

Terrain::~Terrain()
{
    if(bgfx::isValid(_program)) bgfx::destroyProgram(_program);
    if(bgfx::isValid(_vertBuff)) bgfx::destroyVertexBuffer(_vertBuff);
    if(bgfx::isValid(_indBuff)) bgfx::destroyIndexBuffer(_indBuff);
}

bool Terrain::loadMat()
{
    _program = utils::loadProgram("vs_cubes", "fs_cubes");
    return bgfx::isValid(_program);
}

void Terrain::generate()
{
    // generate vertices
    float squareSize = _chunkSize / (float)_subDivisions;

    _vertices.clear();
    _vertices.reserve(_subDivisions*_subDivisions);
    _indices.clear();
    _indices.reserve(_subDivisions*_subDivisions*6);
    int v = 0; // _vertices size

    for(int i = 0; i < _subDivisions; i++) {
        for(int j = 0; j < _subDivisions; j++) {
            float x = (float)i * squareSize;
            float y = (float)j * squareSize;
            float z1 = getZ(x , y);
            float z2 = getZ(x + squareSize, y);
            float z3 = getZ(x, y + squareSize);
            float z4 = getZ(x + squareSize, y + squareSize);

            _vertices.push_back({ x, y, z1, 0xff000000 });
            _vertices.push_back({ x + squareSize, y, z2, 0xff00ff00 });
            _vertices.push_back({ x + squareSize, y + squareSize, z4, 0xff00ffff });
            _vertices.push_back({ x, y + squareSize, z3, 0xff0000ff });

            _indices.push_back(v);_indices.push_back(v+1);_indices.push_back(v+3);
            _indices.push_back(v+1);_indices.push_back(v+2);_indices.push_back(v+3);

            v += 4;
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

