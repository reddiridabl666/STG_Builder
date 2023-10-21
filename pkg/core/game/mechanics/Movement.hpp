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

    virtual sf::Vector2f operator()(GameObject& obj, float delta) = 0;
    virtual Type type() const = 0;
    virtual bool moves_with_field() const = 0;
    virtual operator bool() const = 0;
    virtual std::unique_ptr<Rule> clone() const = 0;
    virtual ~Rule() = default;
};

struct Func : public Rule {
  public:
    Func() : type_(Type::Unknown), func_(nullptr) {}

    template <typename T>
    Func(Type type, T&& func, bool moves_with_field = false)
        : type_(type), func_(std::forward<T>(func)), moves_with_field_(moves_with_field) {}

    sf::Vector2f operator()(GameObject& obj, float delta) override {
        return func_(obj, delta);
    }

    operator bool() const override {
        return bool(func_);
    }

    Type type() const override {
        return type_;
    }

    bool moves_with_field() const override {
        return moves_with_field_;
    }

    std::unique_ptr<Rule> clone() const override {
        return std::make_unique<Func>(type_, func_, moves_with_field_);
    }

  private:
    using update = std::function<sf::Vector2f(const GameObject& obj, float delta)>;

    Type type_;
    update func_;

    bool moves_with_field_ = false;
};

enum class Repeat {
    Repeat,
    Stop,
    StopAtLast
};

struct Multi : public Rule {
  public:
    Multi(std::vector<std::unique_ptr<Rule>>&& funcs, std::vector<float>&& times,
          Repeat repeat = Repeat::Repeat);

    sf::Vector2f operator()(GameObject& obj, float delta) override;

    operator bool() const override;

    Type type() const override;

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

std::unique_ptr<Func> linear(float x = 0, float y = -1);

std::unique_ptr<Func> circular(sf::Vector2f center, float);

std::unique_ptr<Func> user_control(int user_num = 1, const KeyControls& keys = kDefaultKeyControls,
                                   const JoyControls& joy = kDefaultJoyControls);

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
}  // namespace movement
