#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Collection.hpp"
#include "UiElement.hpp"

class Drawable;

class Window {
  private:
    sf::RenderWindow window_;
    sf::Clock clock_;

  public:
    Window(const std::string& name, uint width, uint height, bool is_fullscreen = false, bool vsync = true);

    bool is_open() const;
    void process_events();

    sf::Vector2u get_size() const;

    void set_view(const sf::View& view);

    void clear();
    void set_default_view();

    void draw(const Drawable&);
    void update_ui();

    void display();

    ~Window();
};
