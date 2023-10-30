#include "SideMenu.hpp"

#include "StatDisplayFactory.hpp"

namespace engine {
SideMenu::SideMenu(const Window& window, const sf::Vector2f& offset, float gap, float player_gap,
                   const sf::FloatRect& screen_pos, std::shared_ptr<sf::Texture>&& bg, const nl::json& settings)
    : bg_(std::move(bg)), settings_(settings), offset_(offset), gap_(gap), player_gap_(player_gap) {
    initialize_view(window, screen_pos);
    prev_pos_ = bg_.pos() + offset_;
}

SideMenu::SideMenu(const Window& window, std::shared_ptr<sf::Texture>&& bg, const SideMenuProps& props)
    : SideMenu(window, props.offset, props.gap, props.player_gap, props.size, std::move(bg), props.stats) {}

void SideMenu::draw(Window& window) {
    auto init_view = window.get_view();
    window.set_view(view_);
    bg_.draw(window);
    for (auto& player : player_stats_) {
        for (auto& [_, stat] : player) {
            stat->draw(window);
        }
    }
    window.set_view(init_view);
}

void SideMenu::add_player(const GameObject& player, assets::Manager& assets) {
    PlayerStats stats;
    float prev_height = -gap_;

    for (const auto& [key, value] : settings_.items()) {
        auto stat = value.at("value").get<std::string>();
        auto ui_elem = StatDisplayFactory::create(player.props().get(stat), value, assets);
        ui_elem->set_pos(prev_pos_ + sf::Vector2f{0, prev_height + gap_});

        prev_height = ui_elem->get_size().y;
        prev_pos_ = ui_elem->pos();

        stats.emplace(stat, std::move(ui_elem));
    }

    player_stats_.push_back(std::move(stats));
    prev_pos_ += sf::Vector2f{0, player_gap_};
}

void SideMenu::set_bg(std::shared_ptr<sf::Texture>&& bg) {
    bg_.set_texture(std::move(bg));
}

void SideMenu::clear() {
    prev_pos_ = bg_.pos() + offset_;
    player_stats_.clear();
}

void SideMenu::erase(size_t id) {
    player_stats_.erase(player_stats_.begin() + id);
    // TODO:: add layout fixes
}

void SideMenu::initialize_view(const Window& window, const sf::FloatRect& screen_pos) {
    auto window_size = window.get_size();
    view_.setViewport(screen_pos);

    view_.setSize(window_size.x * screen_pos.width, window_size.y * screen_pos.height);

    // view_.setCenter(
    //     screen_pos.left * window_size.x + 0.5 * view_.getSize().x,
    //     (1 - screen_pos.top) * window_size.y + 0.5 * view_.getSize().y
    // );

    bg_.set_pos(view_.getCenter() - view_.getSize() / 2);
}

void SideMenu::update_layout(const Window& window, const SideMenuProps& props) {
    if (view_.getViewport() != props.size) {
        initialize_view(window, props.size);
    }

    offset_ = props.offset;
    gap_ = props.gap;
    player_gap_ = props.player_gap;

    prev_pos_ = bg_.pos() + offset_;
    float prev_height = -gap_;

    for (auto& player : player_stats_) {
        for (auto& [_, stat] : player) {
            stat->set_pos(prev_pos_ + sf::Vector2f{0, prev_height + gap_});
            prev_height = stat->get_size().y;
            prev_pos_ = stat->pos();
        }
    }
}
}  // namespace engine
