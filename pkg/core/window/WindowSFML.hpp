#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Window.hpp"

class WindowSFML : public Window {
  private:
    sf::RenderWindow window_;
    sf::Clock clock_;

  public:
    WindowSFML(const std::string& name, uint width, uint height, bool is_fullscreen = false,
               bool vsync = true);

    bool is_open() const override;
    void process_events() override;

    virtual void draw(const Drawable&) override;
    void draw_ui(const Collection<UiElement>&) override;

    void display() override;

    ~WindowSFML() override;
};
