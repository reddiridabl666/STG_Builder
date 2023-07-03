#include "App.hpp"
#include "WindowSFML.hpp"

int main() {
    WindowSFML window("STG_Builder", 640, 480);
    App app(window);
    return app.run();
}
