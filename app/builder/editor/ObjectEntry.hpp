#pragma once

#include <optional>
#include <string>
#include <unordered_set>

#include "FuncInfo.hpp"
#include "GameObject.hpp"
#include "Json.hpp"
#include "Player.hpp"
#include "ui/elements/Stats.hpp"
#include "ui/elements/StringPoint.hpp"

namespace ui {

struct ObjectEntry {
  protected:
    const static inline std::string kDefaultActivityStart = std::to_string(GameObject::kDefaultActivityStart);

    struct VisualDelta {
        StringPoint pos;
        int rotation;
    };

  public:
    virtual std::optional<VisualDelta> draw(const std::string& obj_types) = 0;
    virtual nl::json to_json() const = 0;
    virtual void set_pos(const sf::Vector2f&) = 0;

    virtual ~ObjectEntry() = default;
};

struct BasicData {
    BasicData(const std::unordered_set<std::string>& excluded) : stats(excluded) {}

    Stats& get_stats() {
        return stats;
    }

  protected:
    std::string type = "";
    int type_id = 0;
    StringPoint pos = {};
    int rotation = 0;

    Stats stats;
};

struct CommonEntry : public ObjectEntry, public BasicData {
  private:
    const static inline std::unordered_set<std::string> kExcluded = {"type",  "",    "activity_start", "move",
                                                                     "lives", "pos", "rotation"};

  public:
    CommonEntry() : BasicData(kExcluded) {}

    std::optional<VisualDelta> draw(const std::string& obj_types) override;

    nl::json to_json() const override;

    void set_pos(const sf::Vector2f& new_pos) override {
        pos = new_pos;
    }

    Stats& get_stats() {
        return stats;
    }

  private:
    std::string activity_start = kDefaultActivityStart;

    FuncInfo move;
    FuncInfo lives;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CommonEntry, type, activity_start, pos, rotation, move, lives)
};

struct PlayerEntry : public ObjectEntry, public BasicData {
  private:
    const static inline std::unordered_set<std::string> kExcluded = {"type", "pos", "rotation"};

  public:
    PlayerEntry() : BasicData(kExcluded) {}

    std::optional<VisualDelta> draw(const std::string& obj_types) override;

    nl::json to_json() const override;

    void set_pos(const sf::Vector2f& new_pos) override {
        pos = new_pos;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(PlayerEntry, type, pos, rotation, opts)

  private:
    PlayerOptions opts;
};
}  // namespace ui
