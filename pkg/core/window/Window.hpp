#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>
#include <unordered_map>

#include "Collection.hpp"
#include "LinAlg.hpp"

class Drawable;

class Window {
  public:
    Window(const std::string& name, uint width, uint height, bool is_fullscreen = false, bool vsync = true,
           bool default_font = true);

    bool is_open() const;
    void process_events();

    sf::Vector2f pixel_to_coords(int x, int y) const {
        return window_.mapPixelToCoords(sf::Vector2i{x, y});
    }

    sf::Vector2f pixel_to_coords(const sf::Vector2i& vec) const {
        return window_.mapPixelToCoords(vec);
    }

    void add_handler(const std::string& key, sf::Event::EventType event,
                     const std::function<void(sf::Event)>& handler);

    void remove_handler(const std::string& key);

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

    const sf::Window& base() const {
        return window_;
    }

    void draw(const Drawable&);
    void draw(const sf::Drawable&);
    void update_ui();

    void display();

    void close();
    ~Window();

  private:
    struct EventHandler {
        sf::Event::EventType type;
        std::function<void(const sf::Event&)> handler;

        void operator()(const sf::Event& event) {
            handler(event);
        }
    };

    sf::RenderWindow window_;
    sf::Clock clock_;

    std::unordered_map<std::string, EventHandler> handlers_;
};
