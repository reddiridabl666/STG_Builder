#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <string_view>

namespace rng {
size_t get_uint(size_t left, size_t right);

size_t get_uint(size_t limit);

float get_float(float left, float right);

float get_float(float limit);

sf::Vector2f vec(const sf::FloatRect& rect);

std::pair<float, float> parse(std::string_view input);
}  // namespace rng
