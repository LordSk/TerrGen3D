#include <iostream>
#include "app.h"

int main()
{
    int width = 1600;
    int height = 900;

    App* app = new App(width, height);
    global::setApp(app);
    std::cout << "app::init() " << app->init() << std::endl;
    app->run();

    return 0;
}
