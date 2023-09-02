#include "Window.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include "Drawable.hpp"

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

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            window_.setView(sf::View(visibleArea));
        }

#ifdef DEBUG
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Equal) {
                auto view = window_.getView();
                view.zoom(0.8);
                window_.setView(view);
            }
            if (event.key.code == sf::Keyboard::Dash) {
                auto view = window_.getView();
                view.zoom(1.25);
                window_.setView(view);
            }
        }
#endif
    }
}

sf::Vector2u Window::get_size() const {
    return window_.getSize();
}

void Window::set_view(const sf::View& view) {
    window_.setView(view);
}

sf::View Window::get_view() const {
    return window_.getView();
}

void Window::clear() {
    window_.clear();
}

void Window::set_default_view() {
    set_view(window_.getDefaultView());
}

void Window::draw(const Drawable& obj) {
    window_.draw(obj.drawable());
}

void Window::update_ui() {
    ImGui::SFML::Update(window_, clock_.restart());
}

void Window::display() {
    ImGui::SFML::Render(window_);
    window_.display();
}

void Window::close() {
    window_.close();
}

Window::~Window() {
    ImGui::SFML::Shutdown();
}
