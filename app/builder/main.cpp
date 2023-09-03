#include "Messages.hpp"
#include "app/App.hpp"

static const std::string kBase = "../../../test_games";

int main() {
    builder::App app(kBase, "STG_Builder", 1280, 720);
    app.run();
    return 0;
}
