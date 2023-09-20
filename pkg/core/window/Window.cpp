#include "Window.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include "Drawable.hpp"

Window::Window(const std::string& name, uint width, uint height, bool is_fullscreen, bool vsync,
               bool default_font)
    : window_(sf::VideoMode(width, height), name,
              is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default) {
    window_.setVerticalSyncEnabled(vsync);

    if (!ImGui::SFML::Init(window_, default_font)) {
        throw std::runtime_error("Imgui SFML Init failure\n");
    }

    add_handler(sf::Event::Closed, [this](auto) {
        window_.close();
    });

    add_handler(sf::Event::Resized, [this](const auto& event) {
        sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
        window_.setView(sf::View(visibleArea));
    });
}

bool Window::is_open() const {
    return window_.isOpen();
}

void Window::process_events() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window_, event);

        for (auto& [event_type, handler] : handlers_) {
            if (event_type == event.type) {
                handler(event);
            }
        }
    }
}

void Window::add_handler(sf::Event::EventType event, const std::function<void(sf::Event)>& handler) {
    handlers_.emplace_back(event, handler);
}

void Window::main_loop(const std::function<void()>& cb) {
    while (is_open()) {
        frame(cb);
    }
}

void Window::frame(const std::function<void()>& cb) {
    process_events();
    clear();

    update_ui();

    cb();

    display();
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
