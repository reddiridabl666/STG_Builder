#include "App.hpp"

#include "test.hpp"

bool App::run() {
    ui_elements_.push_back(std::make_shared<Test>());

    while (window_.is_open()) {
        window_.draw_all(drawables_);
        window_.draw_ui(ui_elements_);

        window_.process_events();
        window_.display();
    }

    return 0;
}
