#pragma once

#include "terrain.h"

class App
{
    int _width;
    int _height;
    class GLFWwindow* _window;
    Terrain _terrain;
    float _camX;
    float _camY;

    void terminate();

public:
    App(int width, int height);
    ~App();

    bool init();
    void run();
    void keyPress(int key);
};

namespace global {

void setApp(App* app);
App* app();

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
