#pragma once

#include <string>
#include <vector>

#include "Json.hpp"

namespace ui {

struct PatternInput {
    PatternInput() : pos("line"), move("none") {}

    void draw();

    struct Count {
        void draw();

      private:
        size_t left = 0;
        long long right = -1;

        std::string buffer;

        friend void from_json(const nl::json& json, Count& cnt);
        friend void to_json(nl::json& json, const Count& cnt);
    };

    struct Func {
      public:
        Func() = default;
        Func(const std::string& type) : type(type) {}

        void draw(const std::vector<std::string>& types, const std::unordered_map<std::string, nl::json>& args_by_type);

      private:
        std::string type;
        int id;
        std::unordered_map<std::string, nl::json> args;

        friend void from_json(const nl::json& json, Func& cnt);
        friend void to_json(nl::json& json, const Func& cnt);
    };

  private:
    std::string bullet;
    Count count;

    Func pos;
    Func move;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(PatternInput, bullet, pos, count, move)
};
}  // namespace ui
