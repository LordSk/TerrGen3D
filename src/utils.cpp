#include <iostream>
#include "utils.h"

namespace utils {

const bgfx::Memory* loadMem(const char *filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate); // start at the end to read size (ios::ate)
    if(file) {
        std::streampos size = file.tellg();
        const bgfx::Memory* mem = bgfx::alloc((uint32_t)size+1);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(mem->data), size);
        file.close();
        mem->data[mem->size-1] = '\0';
        return mem;
    }

    std::cout << "[ERROR] couldn't open " << filePath << std::endl;
    return nullptr;
}

bgfx::ProgramHandle loadProgram(const std::string &vsName, const std::string &fsName)
{
    auto vMem = loadMem(("shaders/" + vsName + ".bin").c_str());
    auto fMem = loadMem(("shaders/" + fsName + ".bin").c_str());

    if(!vMem || !fMem) {
        return BGFX_INVALID_HANDLE;
    }

    bgfx::ShaderHandle vsh = bgfx::createShader(vMem);
    bgfx::ShaderHandle fsh = bgfx::createShader(fMem);

    return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

} // namespace utils
