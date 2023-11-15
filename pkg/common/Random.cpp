#include "Random.hpp"

#include <fmt/core.h>

#include <random>

namespace rng {
static std::mt19937 gen(std::random_device{}());

size_t get_uint(size_t left, size_t right) {
    std::uniform_int_distribution<size_t> dist(left, right);
    return dist(gen);
}

size_t get_uint(size_t limit) {
    return get_uint(0, limit);
}

float get_float(float left, float right) {
    std::uniform_real_distribution<float> dist(left, right);
    return dist(gen);
}

float get_float(float limit) {
    return get_float(0, limit);
}

sf::Vector2f vec(const sf::FloatRect& rect) {
    return sf::Vector2f{
        get_float(rect.left, rect.left + rect.width),
        get_float(rect.top, rect.top + rect.height),
    };
}

std::pair<float, float> parse(std::string_view input) {
    input.remove_prefix(sizeof("random(") - 1);
    float first = std::stof(std::string(input));
    float second = std::stof(std::string(input.substr(input.find(",") + 1, input.find(")"))));

    fmt::println("random args: {}, {}", first, second);

    return std::pair(first, second);
}
}  // namespace rng
