#include "Player.hpp"

#include "TextFactory.hpp"

PlayerMarker::PlayerMarker(Text&& text, const std::shared_ptr<GameObject>& player, const sf::Vector2f& offset)
    : text_(std::move(text)), offset_(offset), parent_(player) {
    auto contents = text_.contents();
    auto str_id = std::to_string(static_cast<int>(player->props().get(kPlayerNum)) + 1);
    contents.replace(contents.find("{}"), sizeof("{}") - 1, str_id);
    text_.set_contents(contents);
    text_.set_origin(text_.get_size() / 2);
}

void PlayerMarker::draw(Window& window) {
    if (parent_.expired()) {
        return;
    }

    text_.set_pos(parent_.lock()->pos() + offset_);
    text_.draw(window);
}
