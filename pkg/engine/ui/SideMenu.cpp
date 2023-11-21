#include "SideMenu.hpp"

#include "StatDisplayFactory.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

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
        for (auto& stat : player) {
            stat.ui->draw(window);
        }
    }
    window.set_view(init_view);
}

void SideMenu::update(const PlayerList& players) {
    for (size_t i = 0; i < players.size(); ++i) {
        if (!players[i]) {
            continue;
        }

        for (auto& stat : player_stats_[i]) {
            stat.ui->update(players[i]->props().get(stat.key));
        }
    }
}

void SideMenu::add_player(const GameObject& player, assets::Manager& assets) {
    PlayerStats stats;
    float prev_height = -gap_;

    for (const auto& [key, value] : settings_.items()) {
        auto stat = value.at("value").get<std::string>();
        auto ui_elem = StatDisplayFactory::create(player.props().get(stat), value, assets);
        if (!ui_elem) {
#ifdef DEBUG
            LOG(fmt::format("Error creating GameUI, got: {}", value.dump(4)));
#endif
            continue;
        }

        prev_pos_.y += prev_height + gap_;
        ui_elem->set_pos(prev_pos_);

        prev_height = ui_elem->get_size().y;

        stats.push_back(PlayerStat{
            .key = stat,
            .ui = std::move(ui_elem),
        });
    }

    player_stats_.push_back(std::move(stats));
    prev_pos_.y += player_gap_;
}

void SideMenu::update_item(size_t id, const PlayerList& players, assets::Manager& assets, const nl::json& updated) {
    settings_[id] = updated;

    size_t player_id = 0;
    for (auto& player_ui : player_stats_) {
        player_ui[id].key = updated.at("value").get<std::string>();

        float value = players[player_id]->props().get(player_ui[id].key);

        auto ui = StatDisplayFactory::create(value, updated, assets);
        if (!ui) {
#ifdef DEBUG
            LOG(fmt::format("Error creating GameUI, got: {}", updated.dump(4)));
#endif
            ++player_id;
            continue;
        }

        player_ui[id].ui = std::move(ui);
        ++player_id;
    }

    update_layout();
}

void SideMenu::set_bg(std::shared_ptr<sf::Texture>&& bg) {
    bg_.set_texture(std::move(bg));
}

void SideMenu::clear() {
    prev_pos_ = bg_.pos() + offset_;
    player_stats_.clear();
}

void SideMenu::erase_player(size_t id) {
    player_stats_.erase(player_stats_.begin() + id);
    update_layout();
}

void SideMenu::erase_item(size_t id) {
    for (auto& player : player_stats_) {
        player.erase(player.begin() + id);
    }
    update_layout();
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

    update_layout();
}

void SideMenu::add_item(const PlayerList& players, assets::Manager& assets, const nl::json& item) {
    settings_.push_back(item);
    auto key = item.at("value").get<std::string>();

    for (size_t i = 0; i < player_stats_.size(); ++i) {
        auto ui = StatDisplayFactory::create(players[i]->props().get(key), item, assets);
        if (!ui) {
#ifdef DEBUG
            LOG(fmt::format("Error creating GameUI, got: {}", item.dump(4)));
#endif
        }
        player_stats_[i].push_back(PlayerStat{
            .key = key,
            .ui = std::move(ui),
        });
    }
}

void SideMenu::update_layout() {
    prev_pos_ = bg_.pos() + offset_;

    for (auto& player : player_stats_) {
        float prev_height = -gap_;

        for (auto& stat : player) {
            prev_pos_.y += prev_height + gap_;
            stat.ui->set_pos(prev_pos_);
            prev_height = stat.ui->get_size().y;
        }

        prev_pos_.y += player_gap_;
    }
}
}  // namespace engine