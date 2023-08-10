#include "GameField.hpp"

GameField::GameField(std::unique_ptr<SpriteObject>&& image, Window& window, const sf::FloatRect& screen_pos,
                     int speed, const Properties::Data& props)
    : GameObject(std::move(image), speed, GameObject::Tag::Background, props), window_(window) {
    set_pos(0, 0);

    float view_height = screen_pos.height * window_.get_size().y;

    view_.setCenter(sf::Vector2f{left() + width() / 2, bottom() - view_height / 2});
    view_.setSize(sf::Vector2f{width(), view_height});

    view_.setViewport(screen_pos);

    // view_.zoom(1.5);
    window_.set_view(view_);
#ifdef DEBUG
    border_.shape().setSize(view_.getSize());
    border_.shape().setFillColor(sf::Color::Transparent);
    border_.shape().setOutlineColor(sf::Color::Green);
    border_.shape().setOutlineThickness(-5);
    // border_.shape().setOrigin(border_.get_size() / 2);
    // border_.shape().setPosition(view_.getCenter());
    border_.shape().setPosition(left(), bottom() - view_height);
#endif
}

void GameField::update(float delta_time) {
    if (has_ended()) {
        return;
    }

    view_.move(sf::Vector2f{0, -1 * speed() * delta_time});
    border_.move(sf::Vector2f{0, -1 * speed() * delta_time});
    window_.set_view(view_);
}

#ifdef DEBUG
void GameField::draw(Window& window) const {
    GameObject::draw(window);
    window.draw(border_);
}
#endif
