module;

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <stdexcept>
#include <string>

export module Window;

export class Window {
  public:
    virtual bool is_open() const = 0;
    virtual void process_events() = 0;
    virtual void render_ui() = 0;
    virtual void display() = 0;
    virtual ~Window() = default;
};

export class WindowSFML : public Window {
  private:
    sf::RenderWindow window_;
    sf::Clock clock_;

  public:
    WindowSFML(const std::string& name, uint width, uint height, bool is_fullscreen = false, bool vsync = true);

    bool is_open() const override;
    void process_events() override;
    void render_ui() override;
    void display() override;

    ~WindowSFML() override;
};

WindowSFML::WindowSFML(const std::string& name, uint width, uint height, bool is_fullscreen, bool vsync)
    : window_(sf::VideoMode(sf::Vector2u{width, height}), name,
              is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default) {
    window_.setVerticalSyncEnabled(vsync);

    if (!ImGui::SFML::Init(window_)) {
        throw std::runtime_error("Imgui SFML Init failure\n");
    }
}

bool WindowSFML::is_open() const { return window_.isOpen(); }

void WindowSFML::process_events() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window_, event);
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
    }
}

void WindowSFML::render_ui() {
    ImGui::SFML::Update(window_, clock_.restart());

    ImGui::Begin("Imgui window");
    ImGui::Text("Hello, world!");
    ImGui::End();
}

void WindowSFML::display() {
    window_.clear();
    ImGui::SFML::Render(window_);
    window_.display();
}

WindowSFML::~WindowSFML() { ImGui::SFML::Shutdown(); }
