#pragma once

#include <fstream>
#include <bgfx.h>

namespace utils {

/**
 * @brief loadMem
 * @param filePath
 * @return
 */
const bgfx::Memory* loadMem(const char* filePath);

/**
 * @brief loadProgram
 * @param vsName
 * @param fsName
 * @return
 */
bgfx::ProgramHandle loadProgram(const std::string& vsName, const std::string& fsName);

} // namespace utils
