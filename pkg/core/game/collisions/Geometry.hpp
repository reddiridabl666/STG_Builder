#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/box.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/range.hpp>
#include <concepts>
#include <initializer_list>

namespace geom = boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_2D(sf::Vector2f, float, geom::cs::cartesian, x, y)

template <typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <Numeric T>
struct Box {
  public:
    using point_type = sf::Vector2<T>;

    Box() = default;
    Box(const sf::Rect<T>& rect) : rect_(rect) {}

    sf::Vector2<T> top_left() const;
    sf::Vector2<T> bottom_right() const;
    T width() const;
    T height() const;

  private:
    sf::Rect<T> rect_;
};

using FloatBox = Box<float>;

BOOST_GEOMETRY_REGISTER_BOX(FloatBox, sf::Vector2f, top_left, bottom_right)

template <Numeric T>
inline sf::Vector2<T> Box<T>::top_left() const {
    return sf::Vector2<T>{rect_.top, rect_.left};
}

template <Numeric T>
inline sf::Vector2<T> Box<T>::bottom_right() const {
    return sf::Vector2<T>{rect_.top + rect_.height, rect_.left + rect_.width};
}

template <Numeric T>
inline T Box<T>::width() const {
    return rect_.width;
}

template <Numeric T>
inline T Box<T>::height() const {
    return rect_.height;
}

template <Numeric T, size_t N>
struct Polygon {
  public:
    using point_container = std::array<sf::Vector2<T>, N>;

    Polygon(const point_container& points) : points_(points) {
        points_[N - 1] = points_[0];
    }

    const point_container& points() const {
        return points_;
    }

    point_container& points() {
        return points_;
    }

  private:
    point_container points_;
};

template <typename T>
using Rect = Polygon<T, 5>;

using FloatRect = Rect<float>;

namespace boost {
template <>
struct range_iterator<FloatRect> {
    typedef FloatRect::point_container::iterator type;
};

template <>
struct range_const_iterator<FloatRect> {
    typedef FloatRect::point_container::const_iterator type;
};
}  // namespace boost

inline FloatRect::point_container::iterator range_begin(FloatRect& rect) {
    return rect.points().begin();
}

inline FloatRect::point_container::iterator range_end(FloatRect& rect) {
    return rect.points().end();
}

inline FloatRect::point_container::const_iterator range_begin(const FloatRect& rect) {
    return rect.points().begin();
}

inline FloatRect::point_container::const_iterator range_end(const FloatRect& rect) {
    return rect.points().end();
}

BOOST_GEOMETRY_REGISTER_RING(FloatRect)
