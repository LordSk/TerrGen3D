#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <noise/noise.h>
#include <bgfx.h>
#include <bgfxplatform.h>
#include <bx/fpumath.h>

struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init()
    {
        ms_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
            .end();
    };

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[8] =
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

static const uint16_t s_cubeIndices[36] =
{
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

const bgfx::Memory* loadMem(const char* filePath)
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

bgfx::ProgramHandle loadProgram(const std::string& vsName, const std::string& fsName)
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

int main()
{
    int width = 1600;
    int height = 900;
    int debug = BGFX_DEBUG_TEXT;
    int reset = BGFX_RESET_VSYNC;

    if(!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(width, height, "TerrGen3D", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    // center window
    glfwSetWindowPos(window, (1920-width)/2, (1080-height)/2);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    bgfx::glfwSetWindow(window);

    bgfx::init();
    bgfx::reset(width, height, reset);

    // Enable debug text.
    bgfx::setDebug(debug);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , 0x303030ff, 1.0f, 0);

    // Create vertex stream declaration.
    PosColorVertex::init();

    // Create static vertex buffer.
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
          // Static data can be passed with bgfx::makeRef
          bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
        , PosColorVertex::ms_decl);

    // Create static index buffer.
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices)));

    // Create program from shaders.
    bgfx::ProgramHandle program = loadProgram("vs_cubes", "fs_cubes");
    if(!bgfx::isValid(program)) {
        std::cout << "[ERROR] failed to create 'cube' program" << std::endl;
        bgfx::shutdown();
        glfwTerminate();
        return -1;
    }

    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window))
    {
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/01-cube");
        bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Rendering simple static mesh.");

        float at[3]  = { 0.0f, 0.0f,   0.0f };
        float eye[3] = { 0.0f, 0.0f, -35.0f };

        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(width)/float(height), 0.1f, 100.0f);
        bgfx::setViewTransform(0, view, proj);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, width, height);

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::submit(0);

        // Submit 11x11 cubes.
        for(uint32_t yy = 0; yy < 11; ++yy)
        {
            for(uint32_t xx = 0; xx < 11; ++xx)
            {
                float mtx[16];
                bx::mtxRotateXY(mtx, xx*0.21f, yy*0.37f);
                mtx[12] = -15.0f + float(xx)*3.0f;
                mtx[13] = -15.0f + float(yy)*3.0f;
                mtx[14] = 0.0f;

                // Set model matrix for rendering.
                bgfx::setTransform(mtx);

                // Set vertex and fragment shaders.
                bgfx::setProgram(program);

                // Set vertex and index buffer.
                bgfx::setVertexBuffer(vbh);
                bgfx::setIndexBuffer(ibh);

                // Set render states.
                bgfx::setState(BGFX_STATE_DEFAULT);

                // Submit primitive for rendering to view 0.
                bgfx::submit(0);
            }
        }

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bgfx::destroyIndexBuffer(ibh);
    bgfx::destroyVertexBuffer(vbh);
    bgfx::destroyProgram(program);
    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
