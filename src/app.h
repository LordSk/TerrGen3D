#pragma once

#include "terrain.h"

class App
{
    int _width;
    int _height;
    class GLFWwindow* _window;
    Terrain _terrain;

    void terminate();

public:
    App(int width, int height);
    ~App();

    bool init();
    void run();
};
