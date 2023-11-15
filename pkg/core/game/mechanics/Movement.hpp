#pragma once

#include <SFML/System/Vector2.hpp>
#include <functional>
#include <string>
#include <unordered_map>

#include "Controls.hpp"
#include "FuncInfo.hpp"
#include "Json.hpp"

class GameObject;

namespace movement {

struct Rule {
    enum class Type {
        Velocity,
        Pos,
        Unknown
    };

    struct Result {
        Type type;
        union {
            sf::Vector2f pos;
            sf::Vector2f velocity;
        };
    };

    virtual Result operator()(const GameObject& obj, float delta) = 0;
    virtual bool moves_with_field() const = 0;
    virtual operator bool() const = 0;
    virtual void init(const GameObject&) {}
    virtual std::unique_ptr<Rule> clone() const = 0;
    virtual ~Rule() = default;
};

struct Func : public Rule {
  public:
    Func() : func_(nullptr) {}

    template <typename T>
    Func(T&& func, bool moves_with_field = false) : func_(std::forward<T>(func)), moves_with_field_(moves_with_field) {}

    Result operator()(const GameObject& obj, float delta) override {
        return func_(obj, delta);
    }

    operator bool() const override {
        return bool(func_);
    }

    bool moves_with_field() const override {
        return moves_with_field_;
    }

    std::unique_ptr<Rule> clone() const override {
        return std::make_unique<Func>(func_, moves_with_field_);
    }

  private:
    using update = std::function<Result(const GameObject& obj, float delta)>;

    update func_;
    bool moves_with_field_ = false;
};

std::unique_ptr<Rule> linear(const sf::Vector2f& velocity = {0, -1});

std::unique_ptr<Rule> circular(const sf::Vector2f& radius, float speed);

std::unique_ptr<Rule> user_control(int user_num = 1, const KeyControls& keys = kDefaultKeyControls,
                                   const JoyControls& joy = kDefaultJoyControls);

enum class Repeat {
    Repeat,
    Stop,
    StopAtLast
};

struct Multi : public Rule {
  public:
    Multi(std::vector<std::unique_ptr<Rule>>&& funcs, std::vector<float>&& times, Repeat repeat = Repeat::Repeat);

    Result operator()(const GameObject& obj, float delta) override;

    operator bool() const override;

    bool moves_with_field() const override;

    std::unique_ptr<Rule> clone() const override;

  private:
    std::vector<std::unique_ptr<Rule>> funcs_;
    std::vector<float> times_;

    Repeat repeat_;
    size_t current_ = 0;
    float current_time_ = 0;
};

inline std::unique_ptr<Rule> no_op() {
    return std::make_unique<Func>();
}

inline const char* const types[4] = {"none", "linear", "circular", "user_control"};

inline const std::unordered_map<std::string, nl::json> args = {
    {"none", nl::json::object()},
    {"linear", {{"x", 0}, {"y", 0}}},
    {"circular", {{"center", sf::Vector2f{}}, {"speed", 0}}},
    {"user_control", nl::json::object()},
};

// clang-format off
#define REPEAT_TO_STR \
    {Repeat::Repeat, "repeat"}, \
    {Repeat::Stop, "stop"}, \
    {Repeat::StopAtLast, "last"},

NLOHMANN_JSON_SERIALIZE_ENUM(Repeat, {
    REPEAT_TO_STR
})
// clang-format on

struct MultiInfo {
    Repeat repeat;
    std::vector<TimedFuncInfo> rules;
};

void from_json(const nl::json&, MultiInfo&);
void to_json(nl::json&, const MultiInfo&);

#undef REPEAT_TO_STR
}  // namespace movement
