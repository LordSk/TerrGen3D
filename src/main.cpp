#include <iostream>
#include <GLFW/glfw3.h>
#include <noise/noise.h>
#include <bgfx.h>
#include <bgfxplatform.h>

int main()
{
    int width = 1280;
    int height = 720;
    int debug = BGFX_DEBUG_TEXT;
    int reset = BGFX_RESET_VSYNC;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    bgfx::glfwSetWindow(window);

    bgfx::init();
    bgfx::reset(width, height, reset);

    // Enable debug text.
    bgfx::setDebug(debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
        , 0x303030ff, 1.0f, 0);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
    {
        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, width, height);

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::submit(0);

        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
        bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
