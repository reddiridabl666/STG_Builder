#pragma once

#include <ctime>
#include <functional>
#include <memory>
#include <set>
#include <string>

#include "Button.hpp"
#include "DisplayableFactory.hpp"
#include "Form.hpp"
#include "HorizontalLayout.hpp"
#include "Json.hpp"
#include "ScrollableVerticalLayout.hpp"
#include "Text.hpp"

namespace engine {

class LeaderboardsStore {
  public:
    struct Entry;
    using list = std::multiset<Entry, std::greater<Entry>>;

    LeaderboardsStore(list&& scores) : scores_(std::move(scores)) {}

    struct Entry {
        std::string name = "player-name";
        long score = 0;
        time_t time = 0;

        auto operator<=>(const Entry& other) const {
            return score <=> other.score;
        }

        friend void to_json(nl::json& json, const Entry& entry) {
            json[0] = entry.name;
            json[1] = entry.score;
            json[2] = entry.time;
        }

        friend void from_json(const nl::json& json, Entry& entry) {
            try {
                entry.name = json.at(0).get<std::string>();
                entry.score = json.at(1).get<long>();
                entry.time = json.at(2).get<time_t>();
            } catch (...) {
            }
        }
    };

    Entry add_entry(const std::string& name, size_t score) {
        return *scores_.emplace(name, score, std::time(nullptr));
    }

    const auto& entries() const {
        return scores_;
    }

  private:
    list scores_;
};

class LeaderboardsView : public Form<VerticalLayout> {
  public:
    struct EntryText {
        EntryText(TextProps& text, const std::string& num, const std::string& name, const std::string& score,
                  const std::string& when, assets::Fonts& fonts);

        std::unique_ptr<Text> num;
        std::unique_ptr<Text> name;
        std::unique_ptr<Text> score;
        std::unique_ptr<Text> when;
    };

    LeaderboardsView(Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Displayable>&& header_bg,
                     EntryText&& text, std::unique_ptr<Button>&& back,
                     std::unique_ptr<ScrollableVerticalLayout>&& boards_view, float offset_x, float offset_y);

    void scroll(int by) {
        scrollable_->scroll(by);
    }

    struct Entry : public HorizontalLayout {
        Entry(std::unique_ptr<Displayable>&& bg, EntryText&& text, float offset);
    };

  private:
    ScrollableVerticalLayout* scrollable_;
};

class LeaderboardsManager {
  public:
    LeaderboardsManager(const std::filesystem::path& leaderboards_path, const nl::json& ui_info);

    void create_view(Window& window, assets::Manager& assets);

    void draw(Window& window) {
        if (view_) {
            view_->draw(window);
        }
    }

    LeaderboardsStore::Entry add_entry(const std::string& name, size_t score) {
        const auto& entry = store_.add_entry(name, score);
        dump();
        return entry;
    }

    void dump() {
        json::create(leaderboards_path_, store_.entries());
    }

    void scroll_to(const LeaderboardsStore::Entry& entry);

  private:
    std::unique_ptr<ScrollableVerticalLayout> make_boards_view(Window& window, float offset_x, float offset_y,
                                                               TextProps& text_props, assets::Manager& assets);

    LeaderboardsStore store_;
    std::unique_ptr<LeaderboardsView> view_;
    std::filesystem::path leaderboards_path_;
    nl::json ui_info_;
};
}  // namespace engine
