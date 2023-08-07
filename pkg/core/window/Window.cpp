#include "Window.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include "Drawable.hpp"
#include "UiElement.hpp"

Window::Window(const std::string& name, uint width, uint height, bool is_fullscreen, bool vsync)
    : window_(sf::VideoMode(width, height), name,
              is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default) {
    window_.setVerticalSyncEnabled(vsync);

    if (!ImGui::SFML::Init(window_)) {
        throw std::runtime_error("Imgui SFML Init failure\n");
    }
}

bool Window::is_open() const {
    return window_.isOpen();
}

void Window::process_events() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window_, event);
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
    }
}

sf::Vector2u Window::get_size() const {
    return window_.getSize();
}

void Window::set_view(const sf::View& view) {
    window_.setView(view);
}

void Window::set_default_view() {
    set_view(window_.getDefaultView());
}

void Window::draw(const Drawable& obj) {
    window_.draw(obj.drawable());
}

void Window::draw_ui(const Collection<UiElement>& ui_elements) {
    // ImGui::SFML::Update(window_, clock_.restart());

    for (const auto& obj : ui_elements) {
        obj->draw();
    }
}

void Window::display() {
    ImGui::SFML::Update(window_, clock_.restart());

    window_.clear();
    ImGui::SFML::Render(window_);
    window_.display();
}

Window::~Window() {
    ImGui::SFML::Shutdown();
}
