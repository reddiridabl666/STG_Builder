#include "GameField.hpp"

GameField::GameField(std::unique_ptr<SpriteObject>&& image, Window& window, const sf::FloatRect& screen_pos, int speed)
    : ImageContainer(std::move(image), speed), window_(window) {
    set_pos(0, 0);

    float view_height = screen_pos.height * window_.get_size().y;
    set_width(screen_pos.width * window_.get_size().x);

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
        speed_ = 0;
        return;
    }

#ifdef DEBUG
    border_.move(sf::Vector2f{0, -1 * speed() * delta_time});
#endif
    move_view(sf::Vector2f{0, -1 * speed() * delta_time});
    window_.set_view(view_);
}

#ifdef DEBUG
void GameField::draw(Window& window) const {
    ImageContainer::draw(window);
    window.draw(border_);
}
#endif

void GameField::move_view(const sf::Vector2f& offset) {
    view_.move(offset);
    window_.set_view(view_);
}

void GameField::zoom(float value) {
    view_.zoom(value);
    window_.set_view(view_);
}

bool GameField::is_in_bounds(const Transformable& obj, float margin) const {
    // return abs(view_.getCenter().x - obj.pos().x) <= view_.getSize().x / 2 + margin &&
    //        abs(view_.getCenter().y - obj.pos().y) <= view_.getSize().y / 2 + margin;
    return get_bounds(margin).intersects(obj.get_bounds());
}

sf::FloatRect GameField::get_bounds(float margin) const {
    return sf::FloatRect{left() - margin, view_top() - margin, width() + margin, view_.getSize().y + margin};
}

sf::FloatRect GameField::get_bounds() const {
    return get_bounds(0);
}

void GameField::set_image(std::shared_ptr<sf::Texture>&& texture) {
    image_ = std::make_unique<SpriteObject>(std::move(texture));
}
