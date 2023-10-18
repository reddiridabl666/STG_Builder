#include "Window.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include <thread>

#include "Drawable.hpp"

Window::Window(const std::string& name, uint width, uint height, bool is_fullscreen, bool vsync,
               bool default_font)
    : window_(sf::VideoMode(width, height), name,
              is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default) {
    window_.setVerticalSyncEnabled(vsync);

    if (!ImGui::SFML::Init(window_, default_font)) {
        throw std::runtime_error("Imgui SFML Init failure\n");
    }

    add_handler("window_close", sf::Event::Closed, [this](auto) {
        window_.close();
    });

    add_handler("window_resize", sf::Event::Resized, [this](const auto& event) {
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

        for (auto& [key, handler] : handlers_) {
            if (handler.type == event.type) {
                handler(event);
            }
        }
    }
}

sf::Keyboard::Key Window::await_key_press(float seconds, int sleep_delta) {
    sf::Clock clock;
    sf::Event event;

    while (seconds < 0 || clock.getElapsedTime().asSeconds() < seconds) {
        if (!window_.pollEvent(event)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_delta));
        }

        if (event.type != sf::Event::KeyReleased) {
            continue;
        }

        return event.key.code;
    }

    return sf::Keyboard::Unknown;
}

void Window::add_handler(const std::string& key, sf::Event::EventType event,
                         const std::function<void(sf::Event)>& handler) {
    handlers_.emplace(key, EventHandler{event, handler});
}

void Window::remove_handler(const std::string& key) {
    handlers_.erase(key);
}

void Window::main_loop(const std::function<void()>& cb) {
    while (is_open()) {
        process_events();
        clear();

        update_ui();

        cb();

        display();
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

void Window::draw(const sf::Drawable& obj) {
    window_.draw(obj);
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
