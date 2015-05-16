#include <iostream>
#include "utils.h"
#include "terrain.h"

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
    float chunkSize = 1024.f;
    int subDivs = 128;
    float squareSize = chunkSize / (float)subDivs;

    _vertices.clear();
    _vertices.reserve(subDivs*subDivs);
    _indices.clear();
    _indices.reserve(subDivs*subDivs*6);
    int v = 0; // _vertices size
    for(int i = 0; i < subDivs; i++) {
        for(int j = 0; j < subDivs; j++) {
            float x = (float)i * squareSize;
            float y = (float)j * squareSize;
            float z = 0.f;

            _vertices.push_back({ x, y, z, 0xff000000 });
            _vertices.push_back({ x + squareSize, y, z, 0xff00ff00 });
            _vertices.push_back({ x + squareSize, y + squareSize, z, 0xff00ffff });
            _vertices.push_back({ x, y + squareSize, z, 0xff0000ff });
            _indices.push_back(v);_indices.push_back(v+1);_indices.push_back(v+3);
            _indices.push_back(v+1);_indices.push_back(v+2);_indices.push_back(v+4);
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

