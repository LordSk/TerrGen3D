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
      _window(nullptr),
      _camX(0.f),
      _camY(0.f)
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

    //keyboard
    glfwSetKeyCallback(_window, keyCallback);

    // bgfx
    int debug = BGFX_DEBUG_TEXT;
    int reset = BGFX_RESET_MSAA_X16 | BGFX_RESET_MAXANISOTROPY;

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
        float at[3]  = { _camX, _camY, 0.0f };
        float eye[3] = { 0.f, 0.f, -35.0f };

        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(_width)/float(_height), 0.1f, 10000.0f);
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

        glfwPollEvents();
    }
}

void App::keyRelease(int key)
{
    if(key == GLFW_KEY_W)
        _camY += 5.f;
    if(key == GLFW_KEY_S)
        _camY -= 5.f;
    if(key == GLFW_KEY_A)
        _camX -= 5.f;
    if(key == GLFW_KEY_D)
        _camX += 5.f;
}

namespace global {

App* g_app;

void setApp(App *app)
{
    g_app = app;
}

App* app()
{
    return g_app;
}

} // namespace global


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_RELEASE)
        global::app()->keyRelease(key);
}
