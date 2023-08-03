#include "WindowSFML.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include "Drawable.hpp"
#include "UiElement.hpp"

WindowSFML::WindowSFML(const std::string& name, uint width, uint height, bool is_fullscreen, bool vsync)
    : window_(sf::VideoMode(sf::Vector2u{width, height}), name,
              is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default) {
    window_.setVerticalSyncEnabled(vsync);

    if (!ImGui::SFML::Init(window_)) {
        throw std::runtime_error("Imgui SFML Init failure\n");
    }
}

bool WindowSFML::is_open() const {
    return window_.isOpen();
}

void WindowSFML::process_events() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window_, event);
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
    }
}

void WindowSFML::draw(const Drawable& obj) {
    window_.draw(obj.drawable());
}

void WindowSFML::draw_ui(const Collection<UiElement>& ui_elements) {
    ImGui::SFML::Update(window_, clock_.restart());

    for (const auto& obj : ui_elements) {
        obj->draw();
    }
}

void WindowSFML::display() {
    window_.clear();
    ImGui::SFML::Render(window_);
    window_.display();
}

WindowSFML::~WindowSFML() {
    ImGui::SFML::Shutdown();
}
