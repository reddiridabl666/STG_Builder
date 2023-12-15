#include "app/App.hpp"

int main(int argc, char** argv) {
    std::filesystem::path base_path;

    if (argc < 2) {
        base_path = std::filesystem::current_path();
    } else {
        base_path = argv[1];
    }

    builder::App app(base_path, "STG_Builder", 1280, 720);
    app.run();
    return 0;
}
