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

class LeaderboardsManager {
  public:
    struct Entry {
        std::string name = "player-name";
        size_t score = 0;
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
                entry.score = json.at(1).get<size_t>();
                entry.time = json.at(2).get<time_t>();
            } catch (...) {
            }
        }
    };

    void add_entry(const std::string& name, size_t score) {
        scores_.emplace(name, score, std::time(nullptr));
    }

    void dump() {
        json::create(leaderboards_path_, scores_);
    }

    const auto& entries() const {
        return scores_;
    }

    using list = std::set<Entry, std::greater<Entry>>;

  private:
    std::set<Entry, std::greater<Entry>> scores_;
    std::filesystem::path leaderboards_path_;
};

class LeaderboardsView : public Form<VerticalLayout> {
  public:
    LeaderboardsView(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& num, std::unique_ptr<Text>&& name,
                     std::unique_ptr<Text>&& score, std::unique_ptr<Text>&& day, std::unique_ptr<Button>&& back);

    void set(const LeaderboardsManager::list&, assets::Manager& assets);

    struct Entry : public HorizontalLayout {
        Entry(size_t id, const LeaderboardsManager::Entry& entry, const TextProps& text_props_,
              const nl::json& bg_props_, assets::Manager& assets);
    };

  private:
    TextProps text_props_;
    nl::json bg_props_;

    HorizontalLayout header_;
};
}  // namespace engine
