#pragma once

#include <SFML/Graphics/View.hpp>
#include <iostream>

#ifdef DEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#endif

#include "GameObject.hpp"
#include "ShapeObject.hpp"
#include "SpriteObject.hpp"
#include "Window.hpp"

class GameField : public GameObject {  // TODO: Should it really be a GameObject?
  public:
    GameField(std::unique_ptr<SpriteObject>&& image, Window& window, const sf::FloatRect& screen_pos,
              int speed = 50, const Properties::Data& props = {});

    sf::Vector2f center() const {
        return view_.getCenter();
    }

    void update(float delta_time) override;

#ifdef DEBUG
    void draw(Window& window) const override;
#endif

    bool has_ended() const {
        return center().y < end();
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
    Window& window_;
#ifdef DEBUG
    RectObject border_;
#endif
};
