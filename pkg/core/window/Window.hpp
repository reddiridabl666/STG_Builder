#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "LinAlg.hpp"

class Drawable;

class Window {
  public:
    Window(const std::string& name, uint width, uint height, bool is_fullscreen = false, bool vsync = true,
           bool default_font = true);

    bool is_open() const;
    void process_events(float delta_time);

    sf::Keyboard::Key await_key_press(float seconds = -1, int sleep_delta = 100);

    sf::Vector2f pixel_to_coords(int x, int y) const {
        return window_.mapPixelToCoords(sf::Vector2i{x, y});
    }

    void set_mouse_cursor(sf::Cursor::Type cursor_type) {
        auto cursor = cursors_.find(cursor_type);
        if (cursor != cursors_.end()) {
            window_.setMouseCursor(cursor->second);
            return;
        }
        cursors_[cursor_type].loadFromSystem(cursor_type);
        window_.setMouseCursor(cursors_[cursor_type]);
    }

    sf::Vector2f pixel_to_coords(const sf::Vector2i& vec) const {
        return window_.mapPixelToCoords(vec);
    }

    void add_handler(const std::string& key, sf::Event::EventType event, const std::function<void(sf::Event)>& handler);

    void remove_handler(const std::string& key);

    void main_loop(const std::function<void()>& cb);

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
    void resolve_key_hold_events(float delta_time);

    struct EventHandler {
        sf::Event::EventType type;
        std::function<void(const sf::Event&)> handler;

        void operator()(const sf::Event& event) {
            handler(event);
        }
    };

    struct KeyHoldHandler {
        float timeout;
        float cur_time = 0;
        std::function<void()> handler;
    };

    sf::RenderWindow window_;
    sf::Clock clock_;

    std::unordered_map<std::string, EventHandler> handlers_;
    std::unordered_map<sf::Cursor::Type, sf::Cursor> cursors_;

    std::unordered_set<sf::Keyboard::Key> held_keys_;
    std::unordered_map<sf::Keyboard::Key, std::vector<KeyHoldHandler>> key_hold_handlers_;
};
