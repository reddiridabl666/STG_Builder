#pragma once

#include <SFML/Graphics/View.hpp>
#include <iostream>

#ifdef DEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#endif

#include "ImageContainer.hpp"
#include "ShapeObject.hpp"
#include "SpriteObject.hpp"
#include "Window.hpp"

class GameField : public ImageContainer {
  public:
    static inline const sf::FloatRect kDefaultRatio = {0.05, 0.05, 0.5, 0.9};

    GameField(std::unique_ptr<SpriteObject>&& image, Window& window, const sf::FloatRect& screen_pos, int speed = 50);

    sf::Vector2f center() const {
        return view_.getCenter();
    }

    void update(float delta_time);

#ifdef DEBUG
    void draw(Window& window) const override;
#endif

    bool has_ended() const {
        return center().y < end();
    }

    void move_view(const sf::Vector2f& offset);
    void zoom(float);

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
        return view_.getCenter().y - view_.getSize().y / 2;
    }

    void set_image(std::shared_ptr<sf::Texture>&&);

    bool is_in_bounds(const Transformable& obj, float margin = 0) const;

    void keep_in_bounds(Transformable& obj) const;

    sf::FloatRect get_bounds(float margin) const;
    sf::FloatRect get_bounds() const override;

  private:
    sf::View view_;
    Window& window_;

#ifdef DEBUG
    RectObject border_;
#endif
};
