#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>

#include "Collection.hpp"
#include "LinAlg.hpp"

class Drawable;

class Window {
  private:
    sf::RenderWindow window_;
    sf::Clock clock_;

  public:
    Window(const std::string& name, uint width, uint height, bool is_fullscreen = false, bool vsync = true,
           bool default_font = true);

    bool is_open() const;
    void process_events();

    void main_loop(const std::function<void()>& cb);
    void frame(const std::function<void()>& cb);

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

    void close();
    ~Window();
};
