#include "Leaderboards.hpp"

#include <string>

#include "ButtonFactory.hpp"
#include "DisplayableFactory.hpp"
#include "DummyDisplayable.hpp"
#include "GameBus.hpp"
#include "TextFactory.hpp"

namespace engine {
LeaderboardsView::LeaderboardsView(Window& window, std::unique_ptr<Displayable>&& bg,
                                   std::unique_ptr<Displayable>&& header_bg, EntryText&& text,
                                   std::unique_ptr<Button>&& back,
                                   std::unique_ptr<ScrollableVerticalLayout>&& boards_view, float offset_x,
                                   float offset_y)
    : scrollable_(boards_view.get()) {
    layout_ = VerticalLayout{
        std::move(bg),
        offset_y,
        0,
        std::make_unique<HorizontalLayout>(std::move(header_bg), offset_x, std::move(text.num), std::move(text.name),
                                           std::move(text.score), std::move(text.when)),
        std::move(boards_view),
        std::move(back),
    };
    set_pos(window.get_center() - get_size() / 2);
}

LeaderboardsView::Entry::Entry(std::unique_ptr<Displayable>&& bg, EntryText&& text, float offset)
    : HorizontalLayout{
          std::move(bg), offset, std::move(text.num), std::move(text.name), std::move(text.score), std::move(text.when),
      } {}

namespace {
LeaderboardsStore::list get_scores(const std::filesystem::path& leaderboards_path) {
    auto leaderboards = [&leaderboards_path] {
        auto res = json::read(leaderboards_path);
        if (res) {
            return *res;
        }
        json::create(leaderboards_path, nl::json::array());
        return nl::json::array();
    }();
    return leaderboards.get<LeaderboardsStore::list>();
}
}  // namespace

LeaderboardsManager::LeaderboardsManager(const std::filesystem::path& leaderboards_path, const nl::json& ui_info)
    : store_(get_scores(leaderboards_path)), leaderboards_path_(leaderboards_path), ui_info_(ui_info) {}

void LeaderboardsManager::create_view(Window& window, assets::Manager& assets) {
    auto text_props = ui_info_.value("text_style", TextProps{});
    auto offset_x = ui_info_.value("offset_x", 25.f);
    auto offset_y = ui_info_.value("offset_y", 25.f);

    auto back_btn = ButtonFactory::create_unique(ui_info_.value("back", "Back"),
                                                 ui_info_.value("button_style", nl::json::object()), assets);
    back_btn->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    view_ = std::make_unique<LeaderboardsView>(
        window, DisplayableFactory::create(ui_info_.value("bg", nl::json::object()), assets.textures()),
        DisplayableFactory::create(ui_info_.value("header_bg", nl::json::object()), assets.textures()),
        LeaderboardsView::EntryText{
            text_props,
            ui_info_.value("position", "Num"),
            ui_info_.value("name", "Name"),
            ui_info_.value("score", "Score"),
            ui_info_.value("date", "When"),
            assets.fonts(),
        },
        std::move(back_btn), make_boards_view(window, offset_x, offset_y, text_props, assets), offset_x, offset_y);
}

void LeaderboardsManager::scroll_to(const LeaderboardsStore::Entry& entry) {
    auto [begin, end] = store_.entries().equal_range(entry);
    for (auto it = begin; it != end; ++it) {
        if (it->name == entry.name && it->time == entry.time) {
            auto scroll_value = std::distance(store_.entries().begin(), it);
            view_->scroll(scroll_value);
        }
    }
}

std::unique_ptr<ScrollableVerticalLayout> LeaderboardsManager::make_boards_view(Window& window, float offset_x,
                                                                                float offset_y, TextProps& text_props,
                                                                                assets::Manager& assets) {
    std::vector<std::unique_ptr<Displayable>> items;
    items.reserve(store_.entries().size());

    size_t id = 1;

    for (auto& entry : store_.entries()) {
        fmt::println("id: {}, score: {}", std::to_string(id), std::to_string(entry.score));
        auto items_bg = ui_info_.value("item_bg", nl::json::object());

        // clang-format off
        items.push_back(
            std::make_unique<LeaderboardsView::Entry>(
                DisplayableFactory::create(items_bg, assets.textures()),
                LeaderboardsView::EntryText{
                    text_props,
                    std::to_string(id),
                    entry.name,
                    std::to_string(entry.score),
                    utils::day_to_str(entry.time),
                    assets.fonts(),
                },
                offset_x
        ));
        // clang-format on
        ++id;
    }

    return std::make_unique<ScrollableVerticalLayout>(
        window, DisplayableFactory::create(ui_info_.value("table_bg", nl::json::object()), assets.textures()),
        std::move(items), 5, offset_y);
}

LeaderboardsView::EntryText::EntryText(TextProps& text, const std::string& num, const std::string& name,
                                       const std::string& score, const std::string& when, assets::Fonts& fonts)
    : num(TextFactory::create_unique(text.with(num), fonts)),
      name(TextFactory::create_unique(text.with(name), fonts)),
      score(TextFactory::create_unique(text.with(score), fonts)),
      when(TextFactory::create_unique(text.with(when), fonts)) {}

}  // namespace engine
