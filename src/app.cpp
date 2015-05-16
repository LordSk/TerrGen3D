#include <GLFW/glfw3.h>
#include <bgfx.h>
#include <bgfxplatform.h>
#include <bx/fpumath.h>
#include "app.h"

void App::terminate()
{
    bgfx::shutdown();
    glfwTerminate();
}

App::App(int width, int height)
    : _width(width),
      _height(height),
      _window(nullptr)
{

}

App::~App()
{
    terminate();
}

bool App::init()
{
    if(!glfwInit())
        return false;

    _window = glfwCreateWindow(_width, _height, "TerrGen3D", nullptr, nullptr);
    if(!_window)
    {
        glfwTerminate();
        return false;
    }

    // center window, TODO: support all monitors
    glfwSetWindowPos(_window, (1920-_width)/2, (1080-_height)/2);

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    // bgfx
    int debug = BGFX_DEBUG_TEXT;
    int reset = BGFX_RESET_VSYNC;

    bgfx::glfwSetWindow(_window);
    bgfx::init();
    bgfx::reset(_width, _height, reset);
    bgfx::setDebug(debug);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
               , 0x303030ff, 1.0f, 0);

    if(!_terrain.loadMat())
        return false;

    _terrain.generate();
    return true;
}

void App::run()
{
    while(!glfwWindowShouldClose(_window))
    {
        /*bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/01-cube");
        bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Rendering simple static mesh.");*/

        // view
        float at[3]  = { 0.0f, 0.0f,   0.0f };
        float eye[3] = { 0.0f, 0.0f, -35.0f };

        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(_width)/float(_height), 0.1f, 100.0f);
        bgfx::setViewTransform(0, view, proj);


        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, _width, _height);

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::submit(0);

        // Set vertex and fragment shaders.
        bgfx::setProgram(_terrain.getProgram());

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(_terrain.getVertBuff());
        bgfx::setIndexBuffer(_terrain.getIndBuff());

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0);
        bgfx::frame();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

