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

    struct Changes {
        bool pos = false;
        bool rotation = false;
        bool type = false;
    };

  public:
    virtual Changes draw(const std::string& obj_types) = 0;
    virtual nl::json to_json() const = 0;
    virtual void set_pos(const sf::Vector2f&) = 0;
    virtual const std::string& get_type() const = 0;
    virtual const StringPoint& get_pos() const = 0;
    virtual int get_rotation() const = 0;

    virtual ~ObjectEntry() = default;
};

struct BasicData : virtual public ObjectEntry {
    BasicData(const std::unordered_set<std::string>& excluded) : stats(excluded) {}

    Stats& get_stats() {
        return stats;
    }

    void set_pos(const sf::Vector2f& new_pos) override {
        pos = new_pos;
    }

    const std::string& get_type() const override {
        return type;
    }

    const StringPoint& get_pos() const override {
        return pos;
    }

    int get_rotation() const override {
        return rotation;
    }

  protected:
    std::string type = "";
    int type_id = 0;
    StringPoint pos = {};
    int rotation = 0;

    Stats stats;
};

struct CommonEntry : virtual public ObjectEntry, public BasicData {
  private:
    const static inline std::unordered_set<std::string> kExcluded = {"type",  "",    "activity_start", "move",
                                                                     "lives", "pos", "rotation"};

  public:
    CommonEntry() : BasicData(kExcluded) {}

    Changes draw(const std::string& obj_types) override;

    nl::json to_json() const override;

  private:
    std::string activity_start = kDefaultActivityStart;

    movement::MultiInfo move;
    FuncInfo lives;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CommonEntry, type, activity_start, pos, move, rotation, lives)
};

struct PlayerEntry : virtual public ObjectEntry, public BasicData {
  private:
    const static inline std::unordered_set<std::string> kExcluded = {"type", "pos", "rotation", "opts"};

  public:
    PlayerEntry() : BasicData(kExcluded) {}

    Changes draw(const std::string& obj_types) override;

    nl::json to_json() const override;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(PlayerEntry, type, pos, rotation, opts)

  private:
    PlayerOptions opts;
};
}  // namespace ui
