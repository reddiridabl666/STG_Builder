#include "SideMenu.hpp"

namespace engine {
SideMenu::SideMenu(const Window& window, const sf::FloatRect& screen_pos, std::shared_ptr<sf::Texture>&& bg)
    : bg_(std::move(bg)) {
    auto window_size = window.get_size();
    view_.setViewport(screen_pos);

    view_.setSize(window_size.x * screen_pos.width, window_size.y * screen_pos.height);

    // view_.setCenter(
    //     screen_pos.left * window_size.x + 0.5 * view_.getSize().x,
    //     (1 - screen_pos.top) * window_size.y + 0.5 * view_.getSize().y
    // );

    bg_.set_pos(view_.getCenter() - view_.getSize() / 2);
}

void SideMenu::draw(Window& window) {
    window.set_view(view_);
    bg_.draw(window);
    for (auto& player : player_stats_) {
        for (auto& [_, stat] : player) {
            stat->draw(window);
        }
    }
    window.set_default_view();
}
}  // namespace engine
