#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Collection.hpp"
#include "LinAlg.hpp"

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
    sf::View get_view() const;

    sf::Vector2f get_center() const {
        return get_size() / 2.f;
    }

    void clear();
    void set_default_view();

    void draw(const Drawable&);
    void update_ui();

    void display();

    ~Window();
};
