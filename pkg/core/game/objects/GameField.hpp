#pragma once

#include <SFML/Graphics/View.hpp>

#include "GameObject.hpp"
#include "SpriteObject.hpp"
#include "Window.hpp"

class GameField : public GameObject {  // TODO: Should it really be a GameObject?
  public:
    GameField(std::unique_ptr<SpriteObject>&& image, Window& window, const sf::FloatRect& screen_pos,
              int speed = 50, const Properties::Data& props = {})
        : GameObject(std::move(image), speed, GameObject::Tag::Background, props) {
        set_pos(0, 0);

        float view_height = screen_pos.height * window.get_size().y;

        view_.setCenter(sf::Vector2f{left() + width() / 2, bottom() + view_height / 2});
        view_.setSize(sf::Vector2f{width(), view_height});

        view_.setViewport(screen_pos);
        // window.set_view(view_);
    }

    sf::Vector2f center() const {
        return view_.getCenter();
    }

    void update(float delta_time) override {
        view_.move(sf::Vector2f{0, -1 * speed() * delta_time});
    }

    float end() const {
        return top() + view_.getSize().y / 2;
    }

    float right() const {
        return pos().x + width();
    }

    float left() const {
        return pos().x;
    }

    float top() const {
        return pos().y;
    }

    float bottom() const {
        return pos().y + height();
    }

    float height() const {
        return get_size().y;
    }

    float width() const {
        return get_size().x;
    }

    const sf::View& view() const {
        return view_;
    }

    float view_top() const {
        return view_.getCenter().y - view_.getSize().y;
    }

  private:
    sf::View view_;
};
